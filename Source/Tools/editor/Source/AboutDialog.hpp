#pragma once

#include <QDialog>

namespace Ui
{
    class AboutDialog;
}

class AboutDialog final : public QDialog
{
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();
private:
    void stopMusic();
    void startMusic();
private slots:
    void DoText();
private:
    Ui::AboutDialog *ui;
    std::string mScrollText;
    class QMediaPlayer* mPlayer = nullptr;
};
