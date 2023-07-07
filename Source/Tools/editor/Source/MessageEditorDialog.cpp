#include "MessageEditorDialog.hpp"
#include "ui_MessageEditorDialog.h"
#include "Model.hpp"
#include "EditorTab.hpp"
#include <QUndoCommand>
#include <QMessageBox>

class ChangeMessagesCommand final : public QUndoCommand
{
public:
    ChangeMessagesCommand(Model& model, const std::vector<std::string>& oldMsgs, const std::vector<std::string>& newMsgs, bool isLCDScreenMsgs)
        : mModel(model),
        mOldMsgs(oldMsgs),
        mNewMsgs(newMsgs),
        mIsLCDScreenMsgs(isLCDScreenMsgs)
    {
        if (mIsLCDScreenMsgs)
        {
            setText("Change LCDScreen messages");
        }
        else
        {
            setText("Change HintFly messages");
        }
    }

    void undo() override
    {
        if (mIsLCDScreenMsgs)
        {
            mModel.GetMapInfo().mLCDScreenMessages = mOldMsgs;
        }
        else
        {
            mModel.GetMapInfo().mHintFlyMessages = mOldMsgs;
        }
    }

    void redo() override
    {
        if (mIsLCDScreenMsgs)
        {
            mModel.GetMapInfo().mLCDScreenMessages = mNewMsgs;
        }
        else
        {
            mModel.GetMapInfo().mHintFlyMessages = mNewMsgs;
        }
    }

private:
    Model& mModel;
    const std::vector<std::string> mOldMsgs;
    const std::vector<std::string> mNewMsgs;
    bool mIsLCDScreenMsgs = false;
};

MessageEditorDialog::MessageEditorDialog(QWidget *parent, EditorTab* pTab, Model& model, bool isLCDScreenMsgs) :
    QDialog(parent, Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint),
    ui(new Ui::MessageEditorDialog),
    mTab(pTab),
    mModel(model),
    mIsLCDScreenMsgs(isLCDScreenMsgs)
{
    ui->setupUi(this);
    ui->listWidget->setAlternatingRowColors(true);
    ui->listWidget->setWordWrap(true);

    const auto& msgs = GetMsgs();
    for (const auto& msg : msgs)
    {
        ui->listWidget->addItem(msg.c_str());
    }
}

MessageEditorDialog::~MessageEditorDialog()
{
    delete ui;
}

void MessageEditorDialog::on_btnDeleteAll_clicked()
{
    ui->listWidget->clear();
}


void MessageEditorDialog::on_btnDeleteSelected_clicked()
{
    if (!ui->listWidget->selectedItems().empty())
    {
        delete ui->listWidget->takeItem(ui->listWidget->row(ui->listWidget->selectedItems().at(0)));
    }
}


void MessageEditorDialog::on_btnUpdate_clicked()
{
    if (!ui->txtMessage->toPlainText().isEmpty())
    {
        if (!ui->listWidget->selectedItems().empty())
        {
            if (!mIsLCDScreenMsgs)
            {
                ui->txtMessage->setText(ui->txtMessage->toPlainText().toUpper());
            }

            if (MsgContainsValidChars())
            {
                ui->listWidget->selectedItems().at(0)->setText(ui->txtMessage->toPlainText());
            }
        }
    }
}

bool MessageEditorDialog::MsgContainsValidChars()
{
    const std::string message = ui->txtMessage->toPlainText().toStdString();
    if (mIsLCDScreenMsgs)
    {
        // TODO: validate LCD letters
        return true;
    }
    else
    {
        const std::string validCharacters = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        bool ok = message.find_first_not_of(validCharacters) == std::string::npos;
        if (!ok)
        {
            QMessageBox::critical(this, "Error", QString("Your message contains unsupported characters. Supported characters are: ") + validCharacters.c_str());
            return false;
        }
        return true;
    }
}

void MessageEditorDialog::on_btnAdd_clicked()
{
    if (!ui->txtMessage->toPlainText().isEmpty())
    {
        if (!mIsLCDScreenMsgs)
        {
            ui->txtMessage->setText(ui->txtMessage->toPlainText().toUpper());
        }

        if (MsgContainsValidChars())
        {
            ui->listWidget->addItem(ui->txtMessage->toPlainText());
        }
    }
}

void MessageEditorDialog::on_buttonBox_accepted()
{
    const auto& oldMsgs = GetMsgs();
    std::vector<std::string> newMsgs;
    for (int i = 0; i < ui->listWidget->count(); i++)
    {
        newMsgs.emplace_back(ui->listWidget->item(i)->text().toStdString());
    }

    if (oldMsgs != newMsgs)
    {
        auto cmd = new ChangeMessagesCommand(mModel, oldMsgs, newMsgs, mIsLCDScreenMsgs);
        mTab->AddCommand(cmd);
    }
}


void MessageEditorDialog::on_buttonBox_rejected()
{
    // nothing to do
}

void MessageEditorDialog::on_listWidget_itemSelectionChanged()
{
    if (!ui->listWidget->selectedItems().empty())
    {
        ui->txtMessage->setText(ui->listWidget->selectedItems().at(0)->text());
        ui->lblSelectedMsgId->setText("Selected message id: " + QString::number(ui->listWidget->currentRow()));
    }
}

std::vector<std::string>& MessageEditorDialog::GetMsgs()
{
    if (mIsLCDScreenMsgs)
    {
        return mModel.GetMapInfo().mLCDScreenMessages;
    }
    else
    {
        return mModel.GetMapInfo().mHintFlyMessages;
    }
}
