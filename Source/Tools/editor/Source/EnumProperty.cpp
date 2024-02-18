#include "../../relive_lib/Types.hpp"
#include "EnumProperty.hpp"
#include "IGraphicsItem.hpp"
#include <QComboBox>


ChangeEnumPropertyCommand::ChangeEnumPropertyCommand(LinkedEnumProperty linkedProperty, EnumPropertyChangeData propertyData)
    : mLinkedProperty(linkedProperty), mPropertyData(propertyData)
{
    setText(QString("Change property %1 from %2 to %3").arg(mLinkedProperty.mFieldName, mPropertyData.mOldEnumName, mPropertyData.mNewEnumName));
}

void ChangeEnumPropertyCommand::undo()
{
    auto pProperty = static_cast<EnumPropertyBase*>(mLinkedProperty.mTree->FindObjectPropertyByKey(mLinkedProperty.mEnumKey));
    pProperty->VSetEnumFromIdx(mPropertyData.mOldIdx);
    pProperty->Refresh();
    mLinkedProperty.mGraphicsItem->SyncInternalObject();
}

void ChangeEnumPropertyCommand::redo()
{
    auto pProperty = static_cast<EnumPropertyBase*>(mLinkedProperty.mTree->FindObjectPropertyByKey(mLinkedProperty.mEnumKey));
    pProperty->VSetEnumFromIdx(mPropertyData.mNewIdx);
    pProperty->Refresh();
    mLinkedProperty.mGraphicsItem->SyncInternalObject();
}

EnumPropertyBase::EnumPropertyBase(s32 oldIdx, void* pEnumValue, const char* fieldName, QUndoStack& undoStack, IGraphicsItem* pGraphicsItem)
    : PropertyTreeItemBase()
    , mOldIdx(oldIdx)
    , mEnumVale(pEnumValue)
    , mFieldName(fieldName)
    , mUndoStack(undoStack)
    , mGraphicsItem(pGraphicsItem)
{
    setText(0, mFieldName);
}

QWidget* EnumPropertyBase::CreateEditorWidget(PropertyTreeWidget* pParent)
{
    mCombo = new QComboBox(pParent);
    VPopulateEnumCombo(mCombo);
    Refresh();

    connect(mCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [pParent, this](int newIndex)
        {
            if (newIndex != -1 && mOldIdx != newIndex)
            {
                const auto savedOldIdx = mOldIdx;
                mOldIdx = newIndex;

                mUndoStack.push(new ChangeEnumPropertyCommand(
                    LinkedEnumProperty(pParent, mFieldName, mEnumVale, this->mGraphicsItem),
                    EnumPropertyChangeData(VGetEnumName(savedOldIdx), VGetEnumName(newIndex), savedOldIdx, newIndex)));
            }
        });

    connect(mCombo, &QComboBox::destroyed, this, [this](QObject*)
        {
            this->mCombo = nullptr;
        });
    return mCombo;
}

void EnumPropertyBase::Refresh()
{
    // Update the text to match the enum value via its index
    setText(1, VGetEnumName(mOldIdx));

    if (mCombo)
    {
        // If the combo is open then set the correct item in the drop down
        mCombo->setCurrentIndex(mOldIdx);
    }
}

