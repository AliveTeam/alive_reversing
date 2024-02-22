#pragma once

#include <QUndoCommand>
#include <QLabel>
#include "PropertyTreeItemBase.hpp"
#include "../../../relive_lib/RGB16.hpp"

class PropertyTreeWidget;
class QUndoStack;

class ChangeColourPickerPropertyCommand : public QUndoCommand
{
public:
    ChangeColourPickerPropertyCommand(PropertyTreeWidget* pTreeWidget, RGB16& pProperty, QString propertyName, RGB16 oldValue, RGB16 newValue);

    void undo() override;

    void redo() override;

private:
    PropertyTreeWidget* mTreeWidget = nullptr;
    RGB16& mProperty;
    RGB16 mOldValue;
    RGB16 mNewValue;
};

class ColourPickerProperty final : public QObject, public PropertyTreeItemBase
{
    Q_OBJECT
public:
    ColourPickerProperty(PropertyTreeWidget* pParent, RGB16& pProperty, const char* pPropertyName, QUndoStack& undoStack, IGraphicsItem* pGraphicsItem);

    QWidget* GetEditorWidget(PropertyTreeWidget* pParent) override;
    QWidget* GetPersistentEditorWidget(PropertyTreeWidget* pParent) override;
    bool HasBothWidgets() const override { return true; }

    void Refresh() override;

    const void* GetPropertyLookUpKey() const override
    {
        return &mProperty;
    }

private:
    RGB16& mProperty;
    RGB16 mPrevValue;
    const char* mPropertyName = nullptr;
    QUndoStack& mUndoStack;
    IGraphicsItem* mGraphicsItem = nullptr;
    QLabel* mLabel = nullptr;
};
