#pragma once

#include "PropertyTreeItemBase.hpp"
#include <QUndoCommand>
#include <QCheckBox>

class QUndoStack;

struct BoolPropertyChangeData
{
    BoolPropertyChangeData(bool& boolValue, bool oldValue)
        : mBoolValue(boolValue), mOldValue(oldValue)
    {

    }

    bool& mBoolValue;
    bool mOldValue = false;
};

// TODO: probably merge with BasicTypePropertyChangeData ??
struct LinkedBoolProperty
{
    LinkedBoolProperty(const char* propertyName, bool& boolValue, PropertyTreeWidget* pTreeWidget, IGraphicsItem* pGraphicsItem)
        : mPropertyName(propertyName), mBoolValue(boolValue), mTreeWidget(pTreeWidget), mGraphicsItem(pGraphicsItem)
    {

    }

    const char* mPropertyName = nullptr;
    bool& mBoolValue;
    PropertyTreeWidget* mTreeWidget = nullptr;
    IGraphicsItem* mGraphicsItem = nullptr;
};

class ChangeBoolPropertyCommand final : public QUndoCommand
{
public:
    ChangeBoolPropertyCommand(LinkedBoolProperty linkedProperty, BoolPropertyChangeData propertyData)
        : mLinkedProperty(linkedProperty), mPropertyData(propertyData)
    {
        UpdateText();
    }

    void undo() override;

    void redo() override;

private:
    void UpdateText();
    LinkedBoolProperty mLinkedProperty;
    BoolPropertyChangeData mPropertyData;
};

class BoolProperty final : public QObject, public PropertyTreeItemBase
{
    Q_OBJECT
public:

    BoolProperty(PropertyTreeWidget* pParent, bool& boolProperty, const char* pPropertyName, QUndoStack& undoStack, IGraphicsItem* pGraphicsItem);

    QWidget* CreateEditorWidget(PropertyTreeWidget* pParent) override;
    bool PersistentEditorWidget() const override { return true; }

    const void* GetPropertyLookUpKey() const override
    {
        return &mBoolValue;
    }

    void Refresh() override;

private:
    bool mOldValue = false;
    bool& mBoolValue;
    const char* mPropertyName = nullptr;
    QUndoStack& mUndoStack;
    IGraphicsItem* mGraphicsItem = nullptr;
    QCheckBox* mCheckBox = nullptr;
};
