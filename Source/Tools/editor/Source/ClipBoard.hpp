#pragma once

#include <QList>
#include <QGraphicsItem>
#include <QUndoCommand>
#include "Model.hpp"
#include "SelectionSaver.hpp"
#include "EditorGraphicsScene.hpp"

class EditorTab;
class Model;
class ClipBoard;
class ResizeableRectItem;
class ResizeableArrowItem;

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
        UP_MapObjectBase mPastedMapObject;
        Model::Camera* mContainingCamera;
    };
    std::vector<PastedMapObject> mMapObjects;
    std::vector<ResizeableRectItem*> mMapGraphicsObjects;

    std::vector<Model::UP_CollisionObject> mCollisions;
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

    std::vector<UP_MapObjectBase> CloneMapObjects(QPoint* pos) const;
    std::vector<Model::UP_CollisionObject> CloneCollisions(QPoint* pos) const;
private:
    GameType mSourceGame;

    std::vector<Model::UP_CollisionObject> mCollisions;
    std::vector<UP_MapObjectBase> mMapObjects;
};
