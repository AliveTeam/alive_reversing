#include "PropertyTreeWidget.hpp"
#include "PropertyTreeItemBase.hpp"
#include "Model.hpp"
#include "ResizeableArrowItem.hpp"
#include "ResizeableRectItem.hpp"
#include "StringProperty.hpp"
#include "BasicTypeProperty.hpp"
#include "EnumProperty.hpp"
#include <QHeaderView>

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
    auto pLine = qgraphicsitem_cast<ResizeableArrowItem*>(pItem);
    auto pRect = qgraphicsitem_cast<ResizeableRectItem*>(pItem);

    QList<QTreeWidgetItem*> items;
    QTreeWidgetItem* parent = nullptr;
    if (pRect)
    {
        MapObjectBase* pMapObject = pRect->GetMapObject();

        //items.append(new StringProperty(undoStack, parent, kIndent + "Name", &pMapObject->mName));

        for (u32 i = 0; i < pMapObject->mCount; i++)
        {
            items.append(new BasicTypeProperty(undoStack, parent, kIndent + pMapObject->mTypes[i].mFieldName, pRect));
        }



      //  AddProperties(model, undoStack, items, pMapObject->mProperties, pRect);
    }
    else if (pLine)
    {
        Model::CollisionObject* pCollisionItem = pLine->GetCollisionItem();

        items.append(new ReadOnlyStringProperty(parent, kIndent + "Id", &pCollisionItem->mId));

        //items.append(new BasicTypeProperty(undoStack, parent, "X1", pCollisionItem, &bt);

        // TODO line properties
        //AddProperties(model, undoStack, items, pCollisionItem->mProperties, pLine);
    }

    insertTopLevelItems(0, items);
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
    /*
    auto& props = pItem->GetProperties();
    for (auto& prop : props)
    {
        PropertyTreeItemBase* pTreeItem = FindObjectPropertyByKey(prop.get());
        if (pTreeItem)
        {
            pTreeItem->Refresh();
        }
    }*/
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
