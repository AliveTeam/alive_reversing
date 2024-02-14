#pragma once

#include <QTreeWidgetItem>
#include "PropertyTreeWidget.hpp"
#include "BigSpinBox.hpp"

class IGraphicsItem;

struct LinkedProperty
{
    LinkedProperty(MapObjectBase* mapObject, u32 propertyIdx, PropertyTreeWidget* pTreeWidget, IGraphicsItem* pGraphicsItem)
        : mMapObject(mapObject), mPropertyIdx(propertyIdx), mTreeWidget(pTreeWidget), mGraphicsItem(pGraphicsItem)
    {

    }

    MapObjectBase* mMapObject = nullptr;
    u32 mPropertyIdx =0;
    PropertyTreeWidget* mTreeWidget = nullptr;
    IGraphicsItem* mGraphicsItem = nullptr;
};

class PropertyTreeItemBase : public QTreeWidgetItem
{
public:
    using QTreeWidgetItem::QTreeWidgetItem;

    virtual QWidget* CreateEditorWidget(PropertyTreeWidget* pParent) = 0;

    virtual void Refresh() = 0;

    virtual const void* GetPropertyLookUpKey() const = 0;
};
