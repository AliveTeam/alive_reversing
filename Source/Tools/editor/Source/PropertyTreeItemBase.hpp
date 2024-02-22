#pragma once

#include <QTreeWidgetItem>
#include "PropertyTreeWidget.hpp"

class PropertyTreeItemBase : public QTreeWidgetItem
{
public:
    using QTreeWidgetItem::QTreeWidgetItem;

    virtual QWidget* GetEditorWidget(PropertyTreeWidget* pParent) = 0;
    virtual QWidget* GetPersistentEditorWidget(PropertyTreeWidget* /*pParent*/) { return nullptr; }
    virtual bool HasBothWidgets() const { return false; }

    virtual void Refresh() = 0;
    virtual const void* GetPropertyLookUpKey() const = 0;
};
