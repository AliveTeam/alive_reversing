#include "AboutDialog.hpp"
#include "ui_AboutDialog.h"
#include "FireWidget.hpp"
#include <QLayout>
#include <QTimer>
#include "relive_api.hpp"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::AboutDialog),
    mThread( nullptr )
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
    if ( mThread )
    {
        mThread->StopPlay();
        mThread->exit( 0 );
        mThread->wait(  );
        delete mThread;
        mThread = NULL;
    }
}

void AboutDialog::startMusic()
{
    if ( !mThread )
    {
        mThread = new ModThread( ":/about/rsc/about/tune.xm" );
        mThread->start();
    }
}
