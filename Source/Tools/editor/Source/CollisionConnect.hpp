#pragma once

#include <QUndoCommand>
#include "ResizeableArrowItem.hpp"

struct CollisionConnectData
{
    CollisionConnectData(int mOldValue, int mNewValue) : mOldValue(mOldValue), mNewValue(mNewValue)
    {

    }

  //  ObjectProperty* mObjectProperty;
    int mOldValue;
    int mNewValue;
};

class CollisionConnectCommand final : public QUndoCommand
{
public:
    CollisionConnectCommand(std::vector<CollisionConnectData> collisionConnectData);

    void undo() override;

    void redo() override;

    static std::vector<CollisionConnectData> getConnectCollisionsChanges(const std::vector<ResizeableArrowItem *> &collisions);

private:
    std::vector<CollisionConnectData> mCollisionConnectData;

};

