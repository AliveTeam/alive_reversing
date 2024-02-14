#include "BasicTypeProperty.hpp"
#include "Model.hpp"
#include "IGraphicsItem.hpp"
#include <QDateTime>

ChangeBasicTypePropertyCommand::ChangeBasicTypePropertyCommand(LinkedProperty linkedProperty, BasicTypePropertyChangeData propertyData) 
    : mLinkedProperty(linkedProperty), mPropertyData(propertyData)
{
    UpdateText();
    mTimeStamp = QDateTime::currentMSecsSinceEpoch();
}

void ChangeBasicTypePropertyCommand::undo()
{
    mLinkedProperty.mMapObject->SetBasicType(mLinkedProperty.mPropertyIdx, mPropertyData.mOldValue);
    mLinkedProperty.mTreeWidget->FindObjectPropertyByKey(mLinkedProperty.mMapObject->PropertyPtr(mLinkedProperty.mPropertyIdx))->Refresh();
    mLinkedProperty.mGraphicsItem->SyncInternalObject();
}

void ChangeBasicTypePropertyCommand::redo()
{
    mLinkedProperty.mMapObject->SetBasicType(mLinkedProperty.mPropertyIdx, mPropertyData.mNewValue);
    mLinkedProperty.mTreeWidget->FindObjectPropertyByKey(mLinkedProperty.mMapObject->PropertyPtr(mLinkedProperty.mPropertyIdx))->Refresh();
    mLinkedProperty.mGraphicsItem->SyncInternalObject();
}

bool ChangeBasicTypePropertyCommand::mergeWith(const QUndoCommand* command)
{
    if (command->id() == id())
    {
        auto pOther = static_cast<const ChangeBasicTypePropertyCommand*>(command);
        if (mLinkedProperty.mMapObject == pOther->mLinkedProperty.mMapObject && mLinkedProperty.mPropertyIdx == pOther->mLinkedProperty.mPropertyIdx)
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
    setText(QString("Change property %1 from %2 to %3").arg(mLinkedProperty.mMapObject->mTypes[mLinkedProperty.mPropertyIdx].mFieldName, QString::number(mPropertyData.mOldValue), QString::number(mPropertyData.mNewValue)));
}

BasicTypeProperty::BasicTypeProperty(QUndoStack& undoStack, QTreeWidgetItem* pParent, MapObjectBase* pMapObject, u32 propertyIdx, IGraphicsItem* pGraphicsItem)
    : PropertyTreeItemBase(pParent)
    , mMapObject(pMapObject)
    , mPropertyIdx(propertyIdx)
    , mUndoStack(undoStack)
    , mGraphicsItem(pGraphicsItem)
{
    setText(0, mMapObject->mTypes[mPropertyIdx].mFieldName); // TODO: Add text indent
    this->Refresh();
}

QWidget* BasicTypeProperty::CreateEditorWidget(PropertyTreeWidget* pParent)
{
    mSpinBox = new BigSpinBox(pParent);

    switch(mMapObject->mTypes[mPropertyIdx].mFieldType)
    {
        case relive::FieldType::Field_U16:
            mSpinBox->setMax(std::numeric_limits<u16>::max());
            mSpinBox->setMin(std::numeric_limits<u16>::min());
            break;

        case relive::FieldType::Field_S16:
            mSpinBox->setMax(std::numeric_limits<s16>::max());
            mSpinBox->setMin(std::numeric_limits<s16>::min());
            break;
    }


    mSpinBox->setValue(mMapObject->ReadBasicType(mPropertyIdx));

    mOldValue = mMapObject->ReadBasicType(mPropertyIdx);

    connect(mSpinBox, &BigSpinBox::valueChanged, this, [pParent, this](qint64 newValue, bool closeEditor)
        {
            if (mOldValue != newValue)
            {
                mUndoStack.push(new ChangeBasicTypePropertyCommand(
                    LinkedProperty(this->mMapObject, this->mPropertyIdx, pParent, this->mGraphicsItem),
                    BasicTypePropertyChangeData(this->mMapObject, this->mPropertyIdx, this->mOldValue, newValue)));
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
    setText(1, QString::number(mMapObject->ReadBasicType(mPropertyIdx)));

    if (mSpinBox)
    {
        mSpinBox->setValue(mMapObject->ReadBasicType(mPropertyIdx), false);
    }
}
