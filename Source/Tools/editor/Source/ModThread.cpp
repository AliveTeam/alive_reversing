#include "ModThread.hpp"
#include <QFile>

ModThread::ModThread( QString aFileName )
    : mModPlugFile( nullptr ),
      mAudioOutput( nullptr ),
      mAudioDev( nullptr ),
      mRunning( false ),
      mFileName( aFileName )
{
    moveToThread( this );
}

void ModThread::run()
{
    iChunkData.resize( 1024*24 );

    QFile file( mFileName );
    file.open( QIODevice::ReadOnly );
    if ( !file.isOpen() )
    {
        // Can't open input data.
        return;
    }

    iData = file.readAll();

    mModPlugFile = ModPlug_Load( iData.data(), iData.size() );
    if ( !mModPlugFile )
    {
        // Can't load/parse mod data.
        return;
    }

    iData.clear();

    QAudioFormat format;

    ModPlug_Settings settings;
    ModPlug_GetSettings( &settings );

    auto devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    for (auto& dev : devices)
    {
        auto name = dev.deviceName().toStdString();
        name = name;
    }

    QAudioDeviceInfo audioDeviceInfo = QAudioDeviceInfo::defaultOutputDevice();

    format.setSampleRate(settings.mFrequency);
    format.setChannelCount( settings.mChannels );
    format.setSampleSize( settings.mBits );
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    ModPlug_SetMasterVolume( mModPlugFile, 512 );
    ModPlug_Seek( mModPlugFile, 0 );

    QAudioFormat nearestFormat = audioDeviceInfo.nearestFormat(format);
    QAudioDeviceInfo info(audioDeviceInfo);
    if (!info.isFormatSupported(nearestFormat))
    {
        //qWarning()<<"raw audio format not supported by backend, cannot play audio.";
        return;
    }

    mAudioOutput = new QAudioOutput(nearestFormat, this );
    mAudioDev = mAudioOutput->start();
    if (!mAudioDev)
    {
        return;
    }
    mAudioOutput->setNotifyInterval( 70 );
    connect(mAudioOutput, SIGNAL(notify()), this, SLOT(PlayData()));

    mRunning = true;

    PlayData();

    exec();

    mAudioOutput->disconnect();
    mRunning = false;

    mAudioOutput->stop();
    delete mAudioOutput;
    ModPlug_Unload( mModPlugFile );
	
}

void ModThread::StopPlay()
{
    mRunning = false;
}

void ModThread::PlayData()
{
    if (!mRunning)
    {
        return;
    }

    int totalSize = ModPlug_Read( mModPlugFile, &iChunkData[0], iChunkData.size() );
    if ( totalSize == 0 )
    {
        ModPlug_Seek( mModPlugFile, 0 );
        totalSize = ModPlug_Read( mModPlugFile, &iChunkData[0], iChunkData.size() );
    }
    int dataRemaining = totalSize;
    int totalWrote = 0;
    while ( dataRemaining )
    {
        totalWrote += mAudioDev->write( reinterpret_cast< const char* > ( &iChunkData[totalWrote] ), dataRemaining );
        dataRemaining = totalSize - totalWrote;
        if ( mRunning == false )
        {
            break;
        }
    }

}

AudioOutputPrimer::AudioOutputPrimer()
{
    moveToThread(this);
}

void AudioOutputPrimer::run()
{

    auto devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    for (auto& dev : devices)
    {
        auto name = dev.deviceName().toStdString();
        name = name;
    }

    QAudioDeviceInfo audioDeviceInfo = QAudioDeviceInfo::defaultOutputDevice();

    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioFormat nearestFormat = audioDeviceInfo.nearestFormat(format);
    QAudioDeviceInfo info(audioDeviceInfo);
    if (!info.isFormatSupported(nearestFormat))
    {
        //qWarning()<<"raw audio format not supported by backend, cannot play audio.";
        done = true;
        return;
    }

    auto iAudioOutput = new QAudioOutput(info, nearestFormat, this);
    auto iAudioDev = iAudioOutput->start();
    if (!iAudioDev)
    {
        delete iAudioOutput;
        done = true;
        return;
    }

    iAudioOutput->disconnect();
    iAudioOutput->stop();
    delete iAudioOutput;
    done = true;
}
