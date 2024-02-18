#include "BoolProperty.hpp"
#include "IGraphicsItem.hpp"
#include <QDateTime>

void ChangeBoolPropertyCommand::undo()
{
    mLinkedProperty.mBoolValue = mPropertyData.mOldValue;
    mLinkedProperty.mTreeWidget->FindObjectPropertyByKey(&mLinkedProperty.mBoolValue)->Refresh();
    mLinkedProperty.mGraphicsItem->SyncInternalObject();
}

void ChangeBoolPropertyCommand::redo()
{
    mLinkedProperty.mBoolValue = !mPropertyData.mOldValue; // new value is inverse of the old one
    mLinkedProperty.mTreeWidget->FindObjectPropertyByKey(&mLinkedProperty.mBoolValue)->Refresh();

    mLinkedProperty.mGraphicsItem->SyncInternalObject();

}

void ChangeBoolPropertyCommand::UpdateText()
{
    setText(QString("Change property %1 from %2 to %3").arg(mLinkedProperty.mPropertyName, mPropertyData.mOldValue ? "true" : "false", !mPropertyData.mOldValue ? "true" : "false"));
}

BoolProperty::BoolProperty(bool& boolProperty, const char* pPropertyName, QUndoStack& undoStack, IGraphicsItem* pGraphicsItem)
    : PropertyTreeItemBase()
    , mBoolValue(boolProperty)
    , mPropertyName(pPropertyName)
    , mUndoStack(undoStack)
    , mGraphicsItem(pGraphicsItem)
{
    setText(0, mPropertyName); // TODO: Add text indent
    this->Refresh();
}

QWidget* BoolProperty::CreateEditorWidget(PropertyTreeWidget* pParent)
{
    mCheckBox = new QCheckBox(pParent);

    mOldValue = mBoolValue;
    Refresh();

    connect(mCheckBox, &QCheckBox::stateChanged, this, [pParent, this](int state)
        {
            const bool newValue = state == Qt::CheckState::Checked;
            if (mOldValue != newValue)
            {
                mUndoStack.push(new ChangeBoolPropertyCommand(
                    LinkedBoolProperty(this->mPropertyName, this->mBoolValue, pParent, this->mGraphicsItem),
                    BoolPropertyChangeData(this->mBoolValue, mOldValue)));
            }

            mOldValue = newValue;
        });

    connect(mCheckBox, &QCheckBox::destroyed, this, [this](QObject*)
        {
            this->mCheckBox = nullptr;
            Refresh();
        });


    return mCheckBox;
}

void BoolProperty::Refresh()
{
    if (mCheckBox)
    {
        setText(1, "");
        mCheckBox->setCheckState(mBoolValue ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    }
    else
    {
        setText(1, mBoolValue ? "true" : "false");
    }
}
