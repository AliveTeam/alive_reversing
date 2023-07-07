#pragma once

#include <QThread>
#include <QIODevice>
#include <vector>
#include <QtMultimedia/QAudioOutput>
#include <atomic>

#ifndef MODPLUG_STATIC
#define MODPLUG_STATIC
#endif
#include <modplug.h>


class AudioOutputPrimer final : public QThread
{
    Q_OBJECT
public:
    AudioOutputPrimer();
    void run() override;

    std::atomic<bool> done{ false };
};

class ModThread final : public QThread
{
    Q_OBJECT
public:
    ModThread( QString aFileName );
    void StopPlay();
private:
    virtual void run() override;
private slots:
    void PlayData();
private:
    QByteArray iData;
    ModPlugFile* mModPlugFile;
    std::vector< unsigned char > iChunkData;
    QAudioOutput* mAudioOutput;
    QIODevice* mAudioDev;
    std::atomic<bool> mRunning;
    QString mFileName;
};
