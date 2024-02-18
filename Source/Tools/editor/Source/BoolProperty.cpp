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

BoolProperty::BoolProperty(PropertyTreeWidget* pParent, bool& boolProperty, const char* pPropertyName, QUndoStack& undoStack, IGraphicsItem* pGraphicsItem)
    : PropertyTreeItemBase()
    , mBoolValue(boolProperty)
    , mPropertyName(pPropertyName)
    , mUndoStack(undoStack)
    , mGraphicsItem(pGraphicsItem)
{
    // Save the value before the user changes it
    mOldValue = mBoolValue;

    mCheckBox = new QCheckBox(pParent);
    setText(0, mPropertyName);

    // Sync checkbox to correct state
    Refresh();

    connect(mCheckBox, &QCheckBox::toggled, this, [pParent, this](bool checked)
        {
            const bool newValue = checked;
            if (mOldValue != newValue)
            {
                mUndoStack.push(new ChangeBoolPropertyCommand(
                    LinkedBoolProperty(this->mPropertyName, this->mBoolValue, pParent, this->mGraphicsItem),
                    BoolPropertyChangeData(this->mBoolValue, mOldValue)));
            }

            mOldValue = newValue;
        });
}

QWidget* BoolProperty::CreateEditorWidget(PropertyTreeWidget* /*pParent*/)
{
    // Returning a persistent widget
    return mCheckBox;
}

void BoolProperty::Refresh()
{
    // Don't fire a signal when we set the value else it will trigger another undo action
    // which will call Refresh which will trigger another undo action which... you get the point.
    const QSignalBlocker blocker(mCheckBox);
    mCheckBox->setChecked(mBoolValue);
}
