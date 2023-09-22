#pragma once

#include <QUndoCommand>
#include <QList>
#include <QGraphicsItem>
#include "Model.hpp"

class EditorTab;

class DeleteItemsCommand final : public QUndoCommand
{
public:
    DeleteItemsCommand(EditorTab* pTab, bool isCut, QList<QGraphicsItem*> graphicsItemsToDelete);

    ~DeleteItemsCommand();

    void undo() override;

    void redo() override;

private:
  //  std::vector<UP_CollisionObject> mRemovedCollisions;
    struct DeletedMapObject final
    {
      //  UP_MapObject mRemovedMapObject;
        Camera* mContainingCamera;
    };
    std::vector<DeletedMapObject> mRemovedMapObjects;

    bool mAdded = false;
    EditorTab* mTab = nullptr;
    QList<QGraphicsItem*> mGraphicsItemsToDelete;
};
