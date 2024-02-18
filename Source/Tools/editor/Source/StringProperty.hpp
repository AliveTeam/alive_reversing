#pragma once

#include <QUndoCommand>
#include "PropertyTreeItemBase.hpp"

class PropertyTreeWidget;
class QUndoStack;

class ChangeStringPropertyCommand : public QUndoCommand
{
public:
    ChangeStringPropertyCommand(PropertyTreeWidget* pTreeWidget, std::string* pProperty, QString propertyName, QString oldValue, QString newValue);

    void undo() override;

    void redo() override;

private:
    PropertyTreeWidget* mTreeWidget = nullptr;
    std::string* mProperty = nullptr;
    QString mOldValue;
    QString mNewValue;
};

class ReadOnlyStringProperty : public PropertyTreeItemBase
{
public:
    ReadOnlyStringProperty(QTreeWidgetItem* pParent, QString propertyName, int* pProperty);

    virtual QWidget* CreateEditorWidget(PropertyTreeWidget* ) override
    {
        return nullptr;
    }

    void Refresh() override
    {
        setText(1, QString::number(*mProperty));
    }

    const void* GetPropertyLookUpKey() const override
    {
        return mProperty;
    }

private:
    int* mProperty = nullptr;
};

class StringProperty : public QObject, public PropertyTreeItemBase
{
    Q_OBJECT
public:
    StringProperty(QUndoStack& undoStack, QTreeWidgetItem* pParent, QString propertyName, std::string* pProperty);

    virtual QWidget* CreateEditorWidget(PropertyTreeWidget* pParent) override;

    virtual void Refresh() override;

    const void* GetPropertyLookUpKey() const override
    {
        return mProperty;
    }

private:
    std::string* mProperty = nullptr;
    QString mPrevValue;
    QUndoStack& mUndoStack;
};
