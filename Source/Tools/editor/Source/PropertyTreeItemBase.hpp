#pragma once

#include <QTreeWidgetItem>
#include "PropertyTreeWidget.hpp"

class PropertyTreeItemBase : public QTreeWidgetItem
{
public:
    using QTreeWidgetItem::QTreeWidgetItem;

    virtual QWidget* CreateEditorWidget(PropertyTreeWidget* pParent) = 0;

    virtual void Refresh() = 0;

    virtual const void* GetPropertyLookUpKey() const = 0;
};
