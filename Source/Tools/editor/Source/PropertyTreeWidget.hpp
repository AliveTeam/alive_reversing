#pragma once

#include <QTreeWidget>
#include "Model.hpp"

class PropertyTreeItemBase;
class IGraphicsItem;
struct MapObject;
class CollisionObject;
class QGraphicsItem;
class QUndoStack;


inline const QString kIndent("    ");

class ISyncPropertiesToTree
{
public:
    virtual ~ISyncPropertiesToTree() { }
    virtual void Sync(IGraphicsItem* pItem) = 0;
};


class PropertyTreeWidget : public ISyncPropertiesToTree, public QTreeWidget
{
public:
    using QTreeWidget::QTreeWidget;

    PropertyTreeItemBase* FindObjectPropertyByKey(const void* pKey);

    void Populate(Model& model, QUndoStack& undoStack, QGraphicsItem* pItem);
    void DePopulate();

    void Init();

private:
    void Sync(IGraphicsItem* pItem) override;
    void AddProperties(Model& model, QUndoStack& undoStack, QList<QTreeWidgetItem*>& items, std::vector<UP_ObjectProperty>& props, IGraphicsItem* pGraphicsItem);

};
