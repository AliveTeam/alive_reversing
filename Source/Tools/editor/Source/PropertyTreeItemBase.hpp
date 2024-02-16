#pragma once

#include <QTreeWidgetItem>
#include "PropertyTreeWidget.hpp"

class IGraphicsItem;
enum class IntegerType;

struct LinkedProperty
{
    LinkedProperty(MapObjectBase* mapObject, const char* propertyName, IntegerType intType, void* pIntPtr, PropertyTreeWidget* pTreeWidget, IGraphicsItem* pGraphicsItem)
        : mMapObject(mapObject), mPropertyName(propertyName), mIntegerType(intType), mIntPtr(pIntPtr), mTreeWidget(pTreeWidget), mGraphicsItem(pGraphicsItem)
    {

    }

    MapObjectBase* mMapObject = nullptr;
    const char* mPropertyName = nullptr;
    IntegerType mIntegerType;
    void* mIntPtr = nullptr;
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
