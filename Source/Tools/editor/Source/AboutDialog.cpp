#include "AboutDialog.hpp"
#include "ui_AboutDialog.h"
#include "FireWidget.hpp"
#include <QLayout>
#include <QTimer>
#include <QFile>
#include <QDebug>
#include <SDL3/SDL.h>
#define OV_EXCLUDE_STATIC_CALLBACKS // we supply our own ov_callbacks, avoid unused static ones
#include <vorbis/vorbisfile.h>
#include <algorithm>
#include <cstring>
#include "relive_api.hpp"

namespace
{
    struct MemFile
    {
        const char* mData;
        size_t mSize;
        size_t mPos;
    };

    size_t MemRead(void* dst, size_t size, size_t count, void* handle)
    {
        auto* f = static_cast<MemFile*>(handle);
        const size_t n = std::min(size * count, f->mSize - f->mPos);
        std::memcpy(dst, f->mData + f->mPos, n);
        f->mPos += n;
        return size ? n / size : 0;
    }

    int MemSeek(void* handle, ogg_int64_t offset, int whence)
    {
        auto* f = static_cast<MemFile*>(handle);
        ogg_int64_t base = 0;
        switch (whence)
        {
            case SEEK_SET: base = 0; break;
            case SEEK_CUR: base = static_cast<ogg_int64_t>(f->mPos); break;
            case SEEK_END: base = static_cast<ogg_int64_t>(f->mSize); break;
            default: return -1;
        }
        const ogg_int64_t pos = base + offset;
        if (pos < 0 || pos > static_cast<ogg_int64_t>(f->mSize))
        {
            return -1;
        }
        f->mPos = static_cast<size_t>(pos);
        return 0;
    }

    long MemTell(void* handle)
    {
        return static_cast<long>(static_cast<MemFile*>(handle)->mPos);
    }

    bool DecodeOggToPcm(const QByteArray& ogg, std::vector<int16_t>& pcm, int& channels, int& rate)
    {
        MemFile mem{ogg.constData(), static_cast<size_t>(ogg.size()), 0};
        OggVorbis_File vf;
        const ov_callbacks callbacks{MemRead, MemSeek, nullptr, MemTell};
        if (ov_open_callbacks(&mem, &vf, nullptr, 0, callbacks) < 0)
        {
            return false;
        }

        const vorbis_info* info = ov_info(&vf, -1);
        if (!info)
        {
            ov_clear(&vf);
            return false;
        }
        channels = info->channels;
        rate = static_cast<int>(info->rate);

        int16_t buf[2048];
        int bitstream = 0;
        for (;;)
        {
            const long n = ov_read(&vf, reinterpret_cast<char*>(buf), sizeof(buf), SDL_BYTEORDER == SDL_BIG_ENDIAN, 2, 1, &bitstream);
            if (n == OV_HOLE)
            {
                continue;
            }
            if (n <= 0)
            {
                break;
            }
            pcm.insert(pcm.end(), buf, buf + n / sizeof(int16_t));
        }

        ov_clear(&vf);
        return true;
    }
}

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);


    FireWidget* f = new FireWidget( this );
    this->layout()->addWidget( f );

    this->startMusic();

    this->setMaximumSize( this->size() );
    this->setMinimumSize( this->size() );

    QString usingReliveApiV = tr("Using Relive API v");
    setWindowTitle(windowTitle() + " (" + usingReliveApiV + QString::number(ReliveAPI::GetApiVersion()) + ")");

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(DoText()));
    timer->start(40);

    mScrollText = "                                                                                                                                                                                                 Thanks and greetz to all the supporters of the relive project. It might have taken 10 years but we are BACK ;)";

}

void AboutDialog::DoText()
{
    char first = mScrollText[0];
    mScrollText = mScrollText.substr(1);
    mScrollText.append(1, first);

    ui->txtScroller->setText(mScrollText.c_str());
}

AboutDialog::~AboutDialog()
{
    stopMusic();
    delete ui;
}

void AboutDialog::stopMusic()
{
    if (mAudioStream)
    {
        // Destroying the stream also closes its device and waits for FeedAudio to finish.
        SDL_DestroyAudioStream(mAudioStream);
        mAudioStream = nullptr;
    }

    if (mSdlAudioInited)
    {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        mSdlAudioInited = false;
    }

    mPcm.clear();
    mPcm.shrink_to_fit();
    mPcmPos = 0;
}

void AboutDialog::FeedAudio(void* userdata, SDL_AudioStream* stream, int additionalAmount, int /*totalAmount*/)
{
    auto* self = static_cast<AboutDialog*>(userdata);
    while (additionalAmount > 0)
    {
        const size_t bytesLeft = (self->mPcm.size() - self->mPcmPos) * sizeof(int16_t);
        const int chunk = static_cast<int>(std::min<size_t>(static_cast<size_t>(additionalAmount), bytesLeft));
        if (chunk <= 0)
        {
            break;
        }

        SDL_PutAudioStreamData(stream, self->mPcm.data() + self->mPcmPos, chunk);
        self->mPcmPos += chunk / sizeof(int16_t);
        if (self->mPcmPos >= self->mPcm.size())
        {
            self->mPcmPos = 0;
        }
        additionalAmount -= chunk;
    }
}

void AboutDialog::startMusic()
{
    if (mAudioStream)
    {
        return;
    }

    QFile file(":/about/rsc/about/tune.ogg");
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "About tune resource missing";
        return;
    }

    int channels = 0;
    int rate = 0;
    if (!DecodeOggToPcm(file.readAll(), mPcm, channels, rate) || mPcm.empty())
    {
        qWarning() << "About tune failed to decode";
        return;
    }

    if (!SDL_InitSubSystem(SDL_INIT_AUDIO))
    {
        qWarning() << "About tune: SDL audio init failed:" << SDL_GetError();
        stopMusic();
        return;
    }
    mSdlAudioInited = true;

    SDL_AudioSpec spec = {};
    spec.format = SDL_AUDIO_S16;
    spec.channels = channels;
    spec.freq = rate;
    mAudioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, &AboutDialog::FeedAudio, this);
    if (!mAudioStream)
    {
        qWarning() << "About tune: SDL failed to open audio device:" << SDL_GetError();
        stopMusic();
        return;
    }

    SDL_ResumeAudioStreamDevice(mAudioStream);
}
