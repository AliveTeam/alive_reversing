#pragma once

#include <QUndoCommand>
#include <QList>
#include <QGraphicsItem>
#include "../../relive_api/TlvsRelive.hpp"

class EditorTab;
struct EditorCamera;
class CollisionObject;

class DeleteItemsCommand final : public QUndoCommand
{
public:
    DeleteItemsCommand(EditorTab* pTab, bool isCut, QList<QGraphicsItem*> graphicsItemsToDelete);

    ~DeleteItemsCommand();

    void undo() override;

    void redo() override;

private:
    std::vector<std::unique_ptr<CollisionObject>> mRemovedCollisions;
    struct DeletedMapObject final
    {
        std::unique_ptr<MapObjectBase> mRemovedMapObject;
        EditorCamera* mContainingCamera;
    };
    std::vector<DeletedMapObject> mRemovedMapObjects;

    bool mAdded = false;
    EditorTab* mTab = nullptr;
    QList<QGraphicsItem*> mGraphicsItemsToDelete;
};
