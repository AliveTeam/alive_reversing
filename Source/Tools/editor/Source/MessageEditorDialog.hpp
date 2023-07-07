#ifndef MESSAGEEDITORDIALOG_HPP
#define MESSAGEEDITORDIALOG_HPP

#include <QDialog>

namespace Ui {
class MessageEditorDialog;
}

class Model;
class EditorTab;

class MessageEditorDialog : public QDialog
{
    Q_OBJECT

public:
    MessageEditorDialog(QWidget *parent, EditorTab* pTab, Model& model, bool isLedMsgs);
    ~MessageEditorDialog();

private slots:
    void on_btnDeleteAll_clicked();

    void on_btnDeleteSelected_clicked();

    void on_btnUpdate_clicked();

    void on_btnAdd_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_listWidget_itemSelectionChanged();

private:
    bool MsgContainsValidChars();
    std::vector<std::string>& GetMsgs();

    Ui::MessageEditorDialog *ui;
    EditorTab* mTab = nullptr;
    Model& mModel;
    bool mIsLCDScreenMsgs;
};

#endif // MESSAGEEDITORDIALOG_HPP
