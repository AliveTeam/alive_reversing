#ifndef ADDOBJECTDIALOG_HPP
#define ADDOBJECTDIALOG_HPP

#include <QDialog>


namespace Ui {
class AddObjectDialog;
}

class EditorTab;

class AddObjectDialog : public QDialog
{
    Q_OBJECT

public:
    AddObjectDialog(QWidget *parent, EditorTab* pTab);
    ~AddObjectDialog();

private slots:
    void on_txtSearch_textChanged(const QString &arg1);

    void on_buttonBox_accepted();

private:
    void PopulateListFiltered(QString filter);

    Ui::AddObjectDialog *ui;
    EditorTab* mTab = nullptr;
};


#endif // ADDOBJECTDIALOG_HPP
