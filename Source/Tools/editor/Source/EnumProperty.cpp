#include "EnumProperty.hpp"
#include "Model.hpp"
#include "IGraphicsItem.hpp"
#include <QComboBox>

ChangeEnumPropertyCommand::ChangeEnumPropertyCommand(LinkedProperty linkedProperty, EnumPropertyChangeData propertyData)
    : mLinkedProperty(linkedProperty), mPropertyData(propertyData)
{
    //setText(QString("Change property %1 from %2 to %3").arg(mLinkedProperty.mProperty->mName.c_str(), mPropertyData.mEnum->mValues[mPropertyData.mOldIdx].c_str(), mPropertyData.mEnum->mValues[mPropertyData.mNewIdx].c_str()));
}

void ChangeEnumPropertyCommand::undo()
{
    /*
    mLinkedProperty.mProperty->mEnumValue = mPropertyData.mEnum->mValues[mPropertyData.mOldIdx];
    mLinkedProperty.mTreeWidget->FindObjectPropertyByKey(mLinkedProperty.mProperty)->Refresh();
    mLinkedProperty.mGraphicsItem->SyncInternalObject();
    */
}

void ChangeEnumPropertyCommand::redo()
{
    /*
    mLinkedProperty.mProperty->mEnumValue = mPropertyData.mEnum->mValues[mPropertyData.mNewIdx];
    mLinkedProperty.mTreeWidget->FindObjectPropertyByKey(mLinkedProperty.mProperty)->Refresh();
    mLinkedProperty.mGraphicsItem->SyncInternalObject();
    */
}

EnumProperty::EnumProperty(QUndoStack& undoStack, QTreeWidgetItem* pParent, IGraphicsItem* pGraphicsItem, Enum* pEnum) 
    : PropertyTreeItemBase(pParent, QStringList{kIndent, "todo"})
    , mUndoStack(undoStack)
    , mGraphicsItem(pGraphicsItem)
    , mEnum(pEnum)
{

}

QWidget* EnumProperty::CreateEditorWidget(PropertyTreeWidget* pParent)
{
    /*
    mCombo = new QComboBox(pParent);
    for (auto& item : mEnum->mValues)
    {
        mCombo->addItem(item.c_str());
    }
    SetPropertyIndexFromEnumValue();
    Refresh();

    if (mOldIdx != -1)
    {
        mCombo->setCurrentIndex(mOldIdx);
    }

    connect(mCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [pParent, this](int index)
        {
            if (index != -1 && mOldIdx != index)
            {
                mUndoStack.push(new ChangeEnumPropertyCommand(
                    LinkedProperty(pParent, this->mProperty, this->mGraphicsItem),
                    EnumPropertyChangeData(this->mEnum, this->mOldIdx, index)));
            }
            mOldIdx = index;
        });

    connect(mCombo, &QComboBox::destroyed, this, [this](QObject*)
        {
            this->mCombo = nullptr;
        });
    return mCombo;
    */
    return nullptr;
}

void EnumProperty::Refresh()
{
    // Update the idx based on the enum value
    SetPropertyIndexFromEnumValue();

    // Update the text to match the enum value via its index
    //setText(1, mEnum->mValues[mOldIdx].c_str());

    if (mCombo)
    {
        // If the combo is open then set the correct item in the drop down
        mCombo->setCurrentIndex(mOldIdx);
    }
}

void EnumProperty::SetPropertyIndexFromEnumValue()
{
    /*
    mOldIdx = -1;
    int i = 0;
    for (auto& item : mEnum->mValues)
    {
        if (mProperty->mEnumValue == item.c_str())
        {
            mOldIdx = i;
            break;
        }
        i++;
    }*/

}
