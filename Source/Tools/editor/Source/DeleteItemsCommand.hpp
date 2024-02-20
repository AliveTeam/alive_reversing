#pragma once

#include <QUndoCommand>
#include <QList>
#include <QGraphicsItem>
#include <QCoreApplication>
#include "../../relive_api/TlvsRelive.hpp"

class EditorTab;
struct EditorCamera;
class CollisionObject;

class DeleteItemsCommand final : public QUndoCommand
{
    Q_DECLARE_TR_FUNCTIONS(DeleteItemsCommand)

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
