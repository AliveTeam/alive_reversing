#pragma once

#include <QDialog>
#include "ModThread.hpp"

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
    ModThread* mThread = nullptr;
    std::string mScrollText;
};
