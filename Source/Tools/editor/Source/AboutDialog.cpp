#include "AboutDialog.hpp"
#include "ui_AboutDialog.h"
#include "FireWidget.hpp"
#include <QLayout>
#include <QTimer>
#include <QMediaPlayer>
#include <QUrl>
#include "relive_api.hpp"

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
    if (mPlayer)
    {
        mPlayer->stop();
        delete mPlayer;
        mPlayer = nullptr;
    }
}

void AboutDialog::startMusic()
{
    if (!mPlayer)
    {
        mPlayer = new QMediaPlayer();
        mPlayer->setMedia(QUrl("qrc:/about/rsc/about/tune.ogg"));
        connect(mPlayer, &QMediaPlayer::mediaStatusChanged,
                this, [&](QMediaPlayer::MediaStatus status) 
                {
                    if (status == QMediaPlayer::EndOfMedia) 
                    {
                        mPlayer->setPosition(0);
                        mPlayer->play();
                    }
                });
        mPlayer->play();
    }
}
