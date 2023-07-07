#pragma once

#include "PropertyTreeItemBase.hpp"
#include <QUndoCommand>
#include <QComboBox>

struct Enum;
class QUndoStack;

struct EnumPropertyChangeData
{
    EnumPropertyChangeData(Enum* pEnum, int oldIdx, int newIdx)
        : mEnum(pEnum), mOldIdx(oldIdx), mNewIdx(newIdx)
    {

    }
    Enum* mEnum = nullptr;
    int mOldIdx = 0;
    int mNewIdx = 0;
};

class ChangeEnumPropertyCommand : public QUndoCommand
{
public:
    ChangeEnumPropertyCommand(LinkedProperty linkedProperty, EnumPropertyChangeData propertyData);

    void undo() override;

    void redo() override;

private:
    LinkedProperty mLinkedProperty;
    EnumPropertyChangeData mPropertyData;
};

class EnumProperty final : public QObject, public PropertyTreeItemBase
{
    Q_OBJECT
public:
    EnumProperty(QUndoStack& undoStack, QTreeWidgetItem* pParent, ObjectProperty* pProperty, IGraphicsItem* pGraphicsItem, Enum* pEnum);

    QWidget* CreateEditorWidget(PropertyTreeWidget* pParent) override;

    void Refresh() override;

    const void* GetPropertyLookUpKey() const override
    {
        return mProperty;
    }


private:
    void SetPropertyIndexFromEnumValue();

    QUndoStack& mUndoStack;
    ObjectProperty* mProperty = nullptr;
    IGraphicsItem* mGraphicsItem = nullptr;
    Enum* mEnum = nullptr;
    int mOldIdx = -1;

    QComboBox* mCombo = nullptr;
};
