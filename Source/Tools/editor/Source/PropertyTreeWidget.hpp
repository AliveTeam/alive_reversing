#pragma once

#include <QTreeWidget>
#include <QCoreApplication>

class PropertyTreeItemBase;
class IGraphicsItem;
struct MapObject;
class CollisionObject;
class QGraphicsItem;
class QUndoStack;
class Model;

inline const QString kIndent("    ");

class ISyncPropertiesToTree
{
public:
    virtual ~ISyncPropertiesToTree() { }
    virtual void Sync(IGraphicsItem* pItem) = 0;
};


class PropertyTreeWidget : public ISyncPropertiesToTree, public QTreeWidget
{
    Q_DECLARE_TR_FUNCTIONS(PropertyTreeWidget)

public:
    using QTreeWidget::QTreeWidget;

    PropertyTreeItemBase* FindObjectPropertyByKey(const void* pKey);

    void Populate(QUndoStack& undoStack, QGraphicsItem* pItem);
    void DePopulate();

    void Init();

private:
    void Sync(IGraphicsItem* pItem) override;

};
