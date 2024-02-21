#pragma once

#include <QUndoCommand>
#include "ResizeableArrowItem.hpp"

class CollisionObject;

struct CollisionConnectData
{
    CollisionConnectData(s16* mCurrentValue, int mOldValue, int mNewValue)
        : mCurrentValue(mCurrentValue)
        , mOldValue(mOldValue)
        , mNewValue(mNewValue)
    {

    }

    s16* mCurrentValue;
    s16 mOldValue;
    s16 mNewValue;
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

