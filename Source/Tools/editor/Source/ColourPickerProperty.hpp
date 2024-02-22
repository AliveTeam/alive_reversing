#pragma once

#include <QUndoCommand>
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

class ColourPickerProperty : public QObject, public PropertyTreeItemBase
{
    Q_OBJECT
public:
    ColourPickerProperty(PropertyTreeWidget* pParent, RGB16& pProperty, const char* pPropertyName, QUndoStack& undoStack, IGraphicsItem* pGraphicsItem);

    virtual QWidget* CreateEditorWidget(PropertyTreeWidget* pParent) override;

    virtual void Refresh() override;

    const void* GetPropertyLookUpKey() const override
    {
        return &mProperty;
    }

    virtual bool OpenInSeparateWindow() const override
    {
        return true;
    }

private:
    RGB16& mProperty;
    RGB16 mPrevValue;
    const char* mPropertyName = nullptr;
    QUndoStack& mUndoStack;
    IGraphicsItem* mGraphicsItem = nullptr;
};
