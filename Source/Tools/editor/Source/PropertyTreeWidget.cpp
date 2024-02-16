#include "PropertyTreeWidget.hpp"
#include "PropertyTreeItemBase.hpp"
#include "Model.hpp"
#include "ResizeableArrowItem.hpp"
#include "ResizeableRectItem.hpp"
#include "StringProperty.hpp"
#include "BasicTypeProperty.hpp"
#include "EnumProperty.hpp"
#include <QHeaderView>

class PropertyCreator final : public IReflector
{
public:
    PropertyCreator(QUndoStack& undoStack, MapObjectBase* pMapObject, IGraphicsItem* pGraphicsItem)
     : mUndoStack(undoStack), mMapObject(pMapObject), mGraphicsItem(pGraphicsItem)
    {

    }

    void Visit(const char* fieldName, relive::reliveScale& field) override
    {
        // TODO: Generic enum property
    }

    void Visit(const char* fieldName, u16& field) override
    {
        mCreatedProperties.append(new BasicTypeProperty(IntegerType::Int_U16, &field, fieldName, mUndoStack, mMapObject, mGraphicsItem));
    }

    void Visit(const char* fieldName, s16& field) override
    {
        mCreatedProperties.append(new BasicTypeProperty(IntegerType::Int_S16, &field, fieldName, mUndoStack, mMapObject, mGraphicsItem));
    }

    void Visit(const char* fieldName, u32& field) override
    {
        mCreatedProperties.append(new BasicTypeProperty(IntegerType::Int_U32, &field, fieldName, mUndoStack, mMapObject, mGraphicsItem));
    }

    void Visit(const char* fieldName, s32& field) override
    {
        mCreatedProperties.append(new BasicTypeProperty(IntegerType::Int_S32, &field, fieldName, mUndoStack, mMapObject, mGraphicsItem));
    }

    QList<QTreeWidgetItem*>& CreatedProperties()
    {
        return mCreatedProperties;
    }
private:
    QUndoStack& mUndoStack;
    MapObjectBase* mMapObject = nullptr;
    IGraphicsItem* mGraphicsItem = nullptr;
    QList<QTreeWidgetItem*> mCreatedProperties;
};


class PropertyFieldCollector final : public IReflector
{
public:

    void Visit(const char* fieldName, relive::reliveScale& field) override
    {
        // TODO:
    }

    void Visit(const char* , u16& field) override
    {
        mPropertyPointers.append(&field);
    }

    void Visit(const char* , s16& field) override
    {
        mPropertyPointers.append(&field);
    }

    void Visit(const char* , u32& field) override
    {
        mPropertyPointers.append(&field);
    }

    void Visit(const char* , s32& field) override
    {
        mPropertyPointers.append(&field);
    }

    QList<void*>& PropertyPointers()
    {
        return mPropertyPointers;
    }
private:
    QList<void*> mPropertyPointers;
};

PropertyTreeItemBase* PropertyTreeWidget::FindObjectPropertyByKey(const void* pKey)
{
    for (int i = 0; i < topLevelItemCount(); i++)
    {
        auto pItem = static_cast<PropertyTreeItemBase*>(topLevelItem(i));
        const void* itemKey = pItem->GetPropertyLookUpKey();
        if (itemKey == pKey)
        {
            return pItem;
        }
    }
    return nullptr;
}

void PropertyTreeWidget::Populate(Model& model, QUndoStack& undoStack, QGraphicsItem* pItem)
{
    auto pRect = qgraphicsitem_cast<ResizeableRectItem*>(pItem);
    if (pRect)
    {
        MapObjectBase* pMapObject = pRect->GetMapObject();
        PropertyCreator pc(undoStack, pMapObject, pRect);
        pMapObject->Visit(pc);
        insertTopLevelItems(0, pc.CreatedProperties());
    }

    auto pLine = qgraphicsitem_cast<ResizeableArrowItem*>(pItem);
    if (pLine)
    {
        Model::CollisionObject* pCollisionItem = pLine->GetCollisionItem();

        QList<QTreeWidgetItem*> items;
        items.append(new ReadOnlyStringProperty(nullptr, kIndent + "Id", &pCollisionItem->mId));

        //items.append(new BasicTypeProperty(undoStack, parent, "X1", pCollisionItem, &bt);

        // TODO line properties
        //AddProperties(model, undoStack, items, pCollisionItem->mProperties, pLine);
        insertTopLevelItems(0, items);
    }
}

void PropertyTreeWidget::DePopulate()
{
    clear();
}

void PropertyTreeWidget::Init()
{
    // Two columns, property and value
    setColumnCount(2);

    // Set the header text
    QStringList headerStrings;
    headerStrings.append("Property");
    headerStrings.append("Value");
    setHeaderLabels(headerStrings);

    setAlternatingRowColors(true);
    setStyleSheet("QTreeView::item { height:23px; font:6px; padding:0px; margin:0px; }");

    header()->resizeSection(0, 200);
    header()->resizeSection(1, 90);

    setUniformRowHeights(true);

    setRootIsDecorated(false);

    connect(this, &QTreeWidget::itemClicked, this, [&](QTreeWidgetItem* current, int col)
        {
            if (current && col == 1)
            {
                if (!itemWidget(current, 1))
                {
                    setItemWidget(current, 1, static_cast<PropertyTreeItemBase*>(current)->CreateEditorWidget(this));
                }
            }
        });
    connect(this, &QTreeWidget::currentItemChanged, this, [&](QTreeWidgetItem* current, QTreeWidgetItem* prev)
        {
            if (prev)
            {
                setItemWidget(prev, 1, nullptr);
            }

            if (current)
            {
                setItemWidget(current, 1, static_cast<PropertyTreeItemBase*>(current)->CreateEditorWidget(this));
            }
        });
}

void PropertyTreeWidget::Sync(IGraphicsItem* pItem)
{
    PropertyFieldCollector pfc;
    pItem->Visit(pfc);

    auto& props = pfc.PropertyPointers();
    for (auto& prop : props)
    {
        PropertyTreeItemBase* pTreeItem = FindObjectPropertyByKey(prop);
        if (pTreeItem)
        {
            pTreeItem->Refresh();
        }
    }
}

//void PropertyTreeWidget::AddProperties(Model& model, QUndoStack& undoStack, QList<QTreeWidgetItem*>& items, std::vector<UP_ObjectProperty>& props, IGraphicsItem* pGraphicsItem)
//{
    /*
    QTreeWidgetItem* parent = nullptr;
    for (UP_ObjectProperty& property : props)
    {
        if (property->mVisible)
        {
            switch (property->mType)
            {
            case ObjectProperty::Type::BasicType:
            {
                BasicType* pBasicType = model.FindBasicType(property->mTypeName);
                items.append(new BasicTypeProperty(undoStack, parent, kIndent + property->mName.c_str(), property.get(), pGraphicsItem, pBasicType));
            }
                break;

            case ObjectProperty::Type::Enumeration:
            {
                Enum* pEnum = model.FindEnum(property->mTypeName);
                items.append(new EnumProperty(undoStack, parent, property.get(), pGraphicsItem, pEnum));
            }
                break;
            }
        }
    }*/
//}
