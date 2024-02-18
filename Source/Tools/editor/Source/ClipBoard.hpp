#pragma once

#include <QList>
#include <QGraphicsItem>
#include <QUndoCommand>
#include "SelectionSaver.hpp"
#include "EditorGraphicsScene.hpp"
#include "../../../relive_lib/GameType.hpp"

class EditorTab;
class Model;
class ClipBoard;
class ResizeableRectItem;
class ResizeableArrowItem;
struct EditorCamera;
class CollisionObject;

class PasteItemsCommand final : public QUndoCommand
{
public:
    PasteItemsCommand(EditorTab* pTab, ClipBoard& clipBoard);
    ~PasteItemsCommand();
    void redo() override;
    void undo() override;
private:
    EditorTab* mTab = nullptr;

    struct PastedMapObject final
    {
        std::unique_ptr<MapObjectBase> mPastedMapObject;
        EditorCamera* mContainingCamera;
    };
    std::vector<PastedMapObject> mMapObjects;
    std::vector<ResizeableRectItem*> mMapGraphicsObjects;

    std::vector<std::unique_ptr<CollisionObject>> mCollisions;
    std::vector<ResizeableArrowItem*> mCollisionGraphicsObjects;

    SelectionSaver mSelectionSaver;

    bool mPasted = false;
};

class ClipBoard final
{
public:
    void Set(const QList<QGraphicsItem*>& items, Model& model);
    bool IsEmpty() const;

    GameType SourceGame() const;

    std::vector<std::unique_ptr<MapObjectBase>> CloneMapObjects(QPoint* pos) const;
    std::vector<std::unique_ptr<CollisionObject>> CloneCollisions(QPoint* pos) const;
private:
    GameType mSourceGame;

    std::vector<std::unique_ptr<CollisionObject>> mCollisions;
    std::vector<std::unique_ptr<MapObjectBase>> mMapObjects;
};
