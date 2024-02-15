#include "BasicTypeProperty.hpp"
#include "Model.hpp"
#include "IGraphicsItem.hpp"
#include <QDateTime>

static qint64 ReadInt(IntegerType intType, void* intPtr)
{
    switch (intType)
    {
        case IntegerType::Int_S16:
        return *reinterpret_cast<s16*>(intPtr);

        case IntegerType::Int_U16:
        return *reinterpret_cast<u16*>(intPtr);

        case IntegerType::Int_S32:
        return *reinterpret_cast<s32*>(intPtr);

        case IntegerType::Int_U32:
        return *reinterpret_cast<u32*>(intPtr);
    }

    return 0; // rip
}

static void WriteInt(IntegerType intType, void* intPtr, qint64 value)
{
    switch (intType)
    {
        case IntegerType::Int_S16:
        *reinterpret_cast<s16*>(intPtr) = static_cast<s16>(value);
        break;

        case IntegerType::Int_U16:
        *reinterpret_cast<u16*>(intPtr) = static_cast<u16>(value);
        break;

        case IntegerType::Int_S32:
        *reinterpret_cast<s32*>(intPtr) = static_cast<s32>(value);
        break;

        case IntegerType::Int_U32:
        *reinterpret_cast<u32*>(intPtr) = static_cast<u32>(value);
        break;
    }
}

ChangeBasicTypePropertyCommand::ChangeBasicTypePropertyCommand(LinkedProperty linkedProperty, BasicTypePropertyChangeData propertyData) 
    : mLinkedProperty(linkedProperty), mPropertyData(propertyData)
{
    UpdateText();
    mTimeStamp = QDateTime::currentMSecsSinceEpoch();
}

void ChangeBasicTypePropertyCommand::undo()
{
    WriteInt(mLinkedProperty.mIntegerType, mLinkedProperty.mIntPtr, mPropertyData.mOldValue);
    mLinkedProperty.mTreeWidget->FindObjectPropertyByKey(mLinkedProperty.mIntPtr)->Refresh();
    mLinkedProperty.mGraphicsItem->SyncInternalObject();
}

void ChangeBasicTypePropertyCommand::redo()
{
    WriteInt(mLinkedProperty.mIntegerType, mLinkedProperty.mIntPtr, mPropertyData.mNewValue);
    mLinkedProperty.mTreeWidget->FindObjectPropertyByKey(mLinkedProperty.mIntPtr)->Refresh();
    mLinkedProperty.mGraphicsItem->SyncInternalObject();

}

bool ChangeBasicTypePropertyCommand::mergeWith(const QUndoCommand* command)
{
    if (command->id() == id())
    {
        auto pOther = static_cast<const ChangeBasicTypePropertyCommand*>(command);
        if (mLinkedProperty.mIntPtr == pOther->mLinkedProperty.mIntPtr)
        {
            // Compare time stamps and only merge if dt <= 1 second
            if (abs(mTimeStamp - pOther->mTimeStamp) <= 1000)
            {
                mPropertyData.mNewValue = pOther->mPropertyData.mNewValue;
                mTimeStamp = QDateTime::currentMSecsSinceEpoch();
                UpdateText();
                return true;
            }
        }
    }
    return false;
}

void ChangeBasicTypePropertyCommand::UpdateText()
{
    setText(QString("Change property %1 from %2 to %3").arg(mLinkedProperty.mPropertyName, QString::number(mPropertyData.mOldValue), QString::number(mPropertyData.mNewValue)));
}

BasicTypeProperty::BasicTypeProperty(IntegerType intType, void* pInteger, const char* pPropertyName, QUndoStack& undoStack, MapObjectBase* pMapObject, IGraphicsItem* pGraphicsItem)
    : PropertyTreeItemBase((QTreeWidgetItem*)nullptr)
    , mIntType(intType)
    , mIntegerPtr(pInteger)
    , mPropertyName(pPropertyName)
    , mUndoStack(undoStack)
    , mMapObject(pMapObject)
    , mGraphicsItem(pGraphicsItem)
{
    setText(0, mPropertyName); // TODO: Add text indent
    this->Refresh();
}

QWidget* BasicTypeProperty::CreateEditorWidget(PropertyTreeWidget* pParent)
{
    mSpinBox = new BigSpinBox(pParent);


    switch(mIntType)
    {
        case IntegerType::Int_S16:
            mSpinBox->setMax(std::numeric_limits<s16>::max());
            mSpinBox->setMin(std::numeric_limits<s16>::min());
            break;

        case IntegerType::Int_U16:
            mSpinBox->setMax(std::numeric_limits<u16>::max());
            mSpinBox->setMin(std::numeric_limits<u16>::min());
            break;

        case IntegerType::Int_S32:
            mSpinBox->setMax(std::numeric_limits<s32>::max());
            mSpinBox->setMin(std::numeric_limits<s32>::min());
            break;

        case IntegerType::Int_U32:
            mSpinBox->setMax(std::numeric_limits<u32>::max());
            mSpinBox->setMin(std::numeric_limits<u32>::min());
            break;
    }
    mSpinBox->setValue(ReadInt(mIntType, mIntegerPtr));

    mOldValue = ReadInt(mIntType, mIntegerPtr);

    connect(mSpinBox, &BigSpinBox::valueChanged, this, [pParent, this](qint64 newValue, bool closeEditor)
        {
            if (mOldValue != newValue)
            {
                mUndoStack.push(new ChangeBasicTypePropertyCommand(
                    LinkedProperty(this->mMapObject, this->mPropertyName, this->mIntType, this->mIntegerPtr, pParent, this->mGraphicsItem),
                    BasicTypePropertyChangeData(this->mMapObject, this->mIntType, this->mIntegerPtr, this->mOldValue, newValue)));
            }

            mOldValue = newValue;

            if (closeEditor)
            {
                pParent->setItemWidget(this, 1, nullptr);
            }
        });

    connect(mSpinBox, &BigSpinBox::destroyed, this, [this](QObject*)
        {
            this->mSpinBox = nullptr;
        });

    return mSpinBox;
}

void BasicTypeProperty::Refresh()
{
    setText(1, QString::number(ReadInt(mIntType, mIntegerPtr)));

    if (mSpinBox)
    {
        mSpinBox->setValue(ReadInt(mIntType, mIntegerPtr), false);
    }
}
