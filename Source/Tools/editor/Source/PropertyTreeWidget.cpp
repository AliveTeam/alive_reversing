#include "PropertyTreeWidget.hpp"
#include "PropertyTreeItemBase.hpp"
#include "ResizeableArrowItem.hpp"
#include "ResizeableRectItem.hpp"
#include "StringProperty.hpp"
#include "BasicTypeProperty.hpp"
#include "BoolProperty.hpp"
#include "EnumProperty.hpp"
#include <QHeaderView>
#include <QDebug>
#include "../../Tools/relive_api/TlvsRelive.hpp"

class PropertyCreator final : public IReflector
{
private:
    template<typename EnumType>
    void AddEnumProperty(const char* fieldName, EnumType& field)
    {
        auto property = new EnumProperty<std::remove_reference_t<decltype(field)>>(field, fieldName, mUndoStack, mGraphicsItem);
        property->Refresh();
        mCreatedProperties.append(property);
    }

public:
    PropertyCreator(PropertyTreeWidget* pTree, QUndoStack& undoStack, IGraphicsItem* pGraphicsItem)
     : mPropertyTree(pTree), mUndoStack(undoStack), mGraphicsItem(pGraphicsItem)
    {
    }

    void Visit(const char* fieldName, bool& field) override
    {
        mCreatedProperties.append(new BoolProperty(mPropertyTree, field, fieldName, mUndoStack, mGraphicsItem));
    }

    void Visit(const char* fieldName, relive::reliveScale& field) override
    {
        AddEnumProperty(fieldName, field);
    }

    void Visit(const char* fieldName, u16& field) override
    {
        mCreatedProperties.append(new BasicTypeProperty(IntegerType::Int_U16, &field, fieldName, mUndoStack, mGraphicsItem));
    }

    void Visit(const char* fieldName, s16& field) override
    {
        mCreatedProperties.append(new BasicTypeProperty(IntegerType::Int_S16, &field, fieldName, mUndoStack, mGraphicsItem));
    }

    void Visit(const char* fieldName, u32& field) override
    {
        mCreatedProperties.append(new BasicTypeProperty(IntegerType::Int_U32, &field, fieldName, mUndoStack, mGraphicsItem));
    }

    void Visit(const char* fieldName, s32& field) override
    {
        mCreatedProperties.append(new BasicTypeProperty(IntegerType::Int_S32, &field, fieldName, mUndoStack, mGraphicsItem));
    }

    QList<PropertyTreeItemBase*>& CreatedProperties()
    {
        return mCreatedProperties;
    }
private:
    PropertyTreeWidget* mPropertyTree = nullptr;
    QUndoStack& mUndoStack;
    IGraphicsItem* mGraphicsItem = nullptr;
    QList<PropertyTreeItemBase*> mCreatedProperties;
};


class PropertyFieldCollector final : public IReflector
{
public:
    void Visit(const char* fieldName, bool& field) override
    {
        AddField(fieldName, field);
    }

    void Visit(const char* fieldName, relive::reliveScale& field) override
    {
        AddField(fieldName, field);
    }

    void Visit(const char* fieldName, u16& field) override
    {
        AddField(fieldName, field);
    }

    void Visit(const char* fieldName, s16& field) override
    {
        AddField(fieldName, field);
    }

    void Visit(const char* fieldName, u32& field) override
    {
        AddField(fieldName, field);
    }

    void Visit(const char* fieldName, s32& field) override
    {
        AddField(fieldName, field);
    }

    QList<void*>& PropertyPointers()
    {
        return mPropertyPointers;
    }
private:
    template<typename FieldType>
    void AddField(const char*, FieldType& field)
    {
        mPropertyPointers.append(&field);
    }

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
     QList<PropertyTreeItemBase*> createdProperties;

    auto pRect = qgraphicsitem_cast<ResizeableRectItem*>(pItem);
    if (pRect)
    {
        MapObjectBase* pMapObject = pRect->GetMapObject();
        PropertyCreator pc(this, undoStack, pRect);
        pMapObject->Visit(pc);
        createdProperties = pc.CreatedProperties();
    }

    auto pLine = qgraphicsitem_cast<ResizeableArrowItem*>(pItem);
    if (pLine)
    {
        PropertyCreator pc(this, undoStack, pLine);
        pLine->Visit(pc);
        createdProperties = pc.CreatedProperties();
    }

    for (s32 i=0; i < createdProperties.count(); i++)
    {
        auto item = createdProperties[i];
        insertTopLevelItem(i, item);
        if (item->PersistentEditorWidget())
        {
            setItemWidget(item, 1, item->CreateEditorWidget(this));
        }
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


    connect(this, &QTreeWidget::itemPressed, this, [&](QTreeWidgetItem* current, int col)
        {
            if (current && col == 1)
            {
                if (!itemWidget(current, 1))
                {
                    auto pDerived = static_cast<PropertyTreeItemBase*>(current);
                    if (!pDerived->PersistentEditorWidget())
                    {
                        setItemWidget(current, 1, pDerived->CreateEditorWidget(this));
                    }
                }
            }
        });
    connect(this, &QTreeWidget::currentItemChanged, this, [&](QTreeWidgetItem* current, QTreeWidgetItem* prev)
        {
            if (prev)
            {
                auto pDerived = static_cast<PropertyTreeItemBase*>(prev);
                if (!pDerived->PersistentEditorWidget())
                {
                    setItemWidget(prev, 1, nullptr);
                }
            }

            if (current)
            {
                auto pDerived = static_cast<PropertyTreeItemBase*>(current);
                if (!pDerived->PersistentEditorWidget())
                {
                    setItemWidget(current, 1, static_cast<PropertyTreeItemBase*>(current)->CreateEditorWidget(this));
                }
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
