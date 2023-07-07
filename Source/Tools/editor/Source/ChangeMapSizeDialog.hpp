#ifndef CHANGEMAPSIZEDIALOG_HPP
#define CHANGEMAPSIZEDIALOG_HPP

#include <QDialog>

namespace Ui {
class ChangeMapSizeDialog;
}

class EditorTab;

class ChangeMapSizeDialog : public QDialog
{
    Q_OBJECT

public:
    ChangeMapSizeDialog(QWidget *parent, EditorTab* pTab);
    ~ChangeMapSizeDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ChangeMapSizeDialog *ui;
    EditorTab* mTab = nullptr;
};

#endif // CHANGEMAPSIZEDIALOG_HPP
