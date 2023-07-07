#ifndef TRANSPARENCYDIALOG_HPP
#define TRANSPARENCYDIALOG_HPP

#include <QDialog>
#include "EditorGraphicsScene.hpp"

namespace Ui {
class TransparencyDialog;
}

class EditorTab;

class TransparencyDialog : public QDialog
{
    Q_OBJECT

public:
    TransparencyDialog(EditorTab* pTab, QWidget *parent);
    ~TransparencyDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_sliderMapCollision_valueChanged(int value);

    void on_sliderCamera_valueChanged(int value);

    void on_sliderMapObjects_valueChanged(int value);

private:
    Ui::TransparencyDialog *ui;
    EditorTab* mTab = nullptr;
    TransparencySettings mOldSettings;
};

#endif // TRANSPARENCYDIALOG_HPP
