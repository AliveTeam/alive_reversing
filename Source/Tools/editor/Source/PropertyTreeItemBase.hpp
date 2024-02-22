#pragma once

#include <QTreeWidgetItem>
#include "PropertyTreeWidget.hpp"

class PropertyTreeItemBase : public QTreeWidgetItem
{
public:
    using QTreeWidgetItem::QTreeWidgetItem;

    virtual QWidget* CreateEditorWidget(PropertyTreeWidget* pParent) = 0;

    virtual void Refresh() = 0;

    // Return true so that the editor widget always exists instead of only
    // appears when the property tree row has focus.
    virtual bool PersistentEditorWidget() const { return false; }

    virtual bool OpenInSeparateWindow() const { return false; }

    virtual const void* GetPropertyLookUpKey() const = 0;
};
