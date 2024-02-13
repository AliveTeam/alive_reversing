#pragma once

#include "PropertyTreeItemBase.hpp"
#include <QUndoCommand>

struct BasicType;
class QUndoStack;
class BigSpinBox;

struct BasicTypePropertyChangeData
{
    BasicTypePropertyChangeData(BasicType* pBasicType, int oldValue, int newValue)
        : mBasicType(pBasicType), mOldValue(oldValue), mNewValue(newValue)
    {

    }
    BasicType* mBasicType = nullptr;
    int mOldValue = 0;
    int mNewValue = 0;
};

class ChangeBasicTypePropertyCommand final : public QUndoCommand
{
public:
    ChangeBasicTypePropertyCommand(LinkedProperty linkedProperty, BasicTypePropertyChangeData propertyData);

    void undo() override;

    void redo() override;

    int id() const override
    {
        return 1;
    }

    bool mergeWith(const QUndoCommand* command) override;

private:
    void UpdateText();
    LinkedProperty mLinkedProperty;
    BasicTypePropertyChangeData mPropertyData;
    qint64 mTimeStamp = 0;
};

class BasicTypeProperty final : public QObject, public PropertyTreeItemBase
{
    Q_OBJECT
public:
    BasicTypeProperty(QUndoStack& undoStack, QTreeWidgetItem* pParent, QString propertyName, IGraphicsItem* pGraphicsItem);

    QWidget* CreateEditorWidget(PropertyTreeWidget* pParent) override;

    const void* GetPropertyLookUpKey() const override
    {
      //  return mProperty;
        return nullptr;
    }

    void Refresh() override;

private:
    QUndoStack& mUndoStack;
   // ObjectProperty* mProperty = nullptr;
    IGraphicsItem* mGraphicsItem = nullptr;
   // BasicType* mBasicType = nullptr;
    int mOldValue = 0;
    BigSpinBox* mSpinBox = nullptr;
};
