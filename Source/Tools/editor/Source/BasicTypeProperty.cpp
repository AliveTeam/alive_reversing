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
    mLinkedProperty.mProperty->mBasicTypeValue = mPropertyData.mOldValue;
    mLinkedProperty.mTreeWidget->FindObjectPropertyByKey(mLinkedProperty.mProperty)->Refresh();
    mLinkedProperty.mGraphicsItem->SyncInternalObject();
}

void ChangeBasicTypePropertyCommand::redo()
{
    mLinkedProperty.mProperty->mBasicTypeValue = mPropertyData.mNewValue;
    mLinkedProperty.mTreeWidget->FindObjectPropertyByKey(mLinkedProperty.mProperty)->Refresh();
    mLinkedProperty.mGraphicsItem->SyncInternalObject();
}

bool ChangeBasicTypePropertyCommand::mergeWith(const QUndoCommand* command)
{
    if (command->id() == id())
    {
        auto pOther = static_cast<const ChangeBasicTypePropertyCommand*>(command);
        if (mLinkedProperty.mProperty == pOther->mLinkedProperty.mProperty)
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
    setText(QString("Change property %1 from %2 to %3").arg(mLinkedProperty.mProperty->mName.c_str(), QString::number(mPropertyData.mOldValue), QString::number(mPropertyData.mNewValue)));
}

BasicTypeProperty::BasicTypeProperty(QUndoStack& undoStack, QTreeWidgetItem* pParent, QString propertyName, ObjectProperty* pProperty, IGraphicsItem* pGraphicsItem, BasicType* pBasicType) : PropertyTreeItemBase(pParent, QStringList{ propertyName, QString::number(pProperty->mBasicTypeValue) }), mUndoStack(undoStack), mProperty(pProperty), mBasicType(pBasicType), mGraphicsItem(pGraphicsItem)
{

}

QWidget* BasicTypeProperty::CreateEditorWidget(PropertyTreeWidget* pParent)
{
    mSpinBox = new BigSpinBox(pParent);
    mSpinBox->setMax(mBasicType->mMaxValue);
    mSpinBox->setMin(mBasicType->mMinValue);
    mSpinBox->setValue(mProperty->mBasicTypeValue);

    mOldValue = mProperty->mBasicTypeValue;

    connect(mSpinBox, &BigSpinBox::valueChanged, this, [pParent, this](qint64 newValue, bool closeEditor)
        {
            if (mOldValue != newValue)
            {
                mUndoStack.push(new ChangeBasicTypePropertyCommand(
                    LinkedProperty(pParent, this->mProperty, this->mGraphicsItem),
                    BasicTypePropertyChangeData(this->mBasicType, this->mOldValue, newValue)));
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
    setText(1, QString::number(mProperty->mBasicTypeValue));

    if (mSpinBox)
    {
        mSpinBox->setValue(mProperty->mBasicTypeValue, false);
    }
}
