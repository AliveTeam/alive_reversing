#include "CollisionConnect.hpp"

#include <utility>
#include "Model.hpp"

CollisionConnectCommand::CollisionConnectCommand(std::vector<CollisionConnectData> collisionConnectData):
        mCollisionConnectData(std::move(collisionConnectData))
{
    setText("Connected collisions");
}

void CollisionConnectCommand::undo()
{
    for (auto& joinDatum : mCollisionConnectData)
    {
        *joinDatum.mCurrentValue = joinDatum.mOldValue;
    }
}

void CollisionConnectCommand::redo()
{
    for (auto& joinDatum : mCollisionConnectData)
    {
        *joinDatum.mCurrentValue = joinDatum.mNewValue;
    }
}

std::vector<CollisionConnectData> CollisionConnectCommand::getConnectCollisionsChanges(const std::vector<ResizeableArrowItem*> &collisions)
{
    std::vector<CollisionConnectData> collisionConnectData;

    for (std::size_t i = 0; i < collisions.size(); i++)
    {
        auto& collision = collisions[i];
        auto collisionItem = collision->GetCollisionItem();
        int id = collisionItem->mId;

        int startX = collisionItem->X1();
        int startY = collisionItem->Y1();

        int endX = collisionItem->X2();
        int endY = collisionItem->Y2();


        for (std::size_t j = i + 1; j < collisions.size(); j++)
        {
            auto& otherCollision = collisions[j];
            auto otherCollisionItem = otherCollision->GetCollisionItem();
            int otherId = otherCollisionItem->mId;

            int otherStartX = otherCollisionItem->X1();
            int otherStartY = otherCollisionItem->Y1();

            int otherEndX = otherCollisionItem->X2();
            int otherEndY = otherCollisionItem->Y2();

            if (endX == otherStartX && endY == otherStartY)
            {
                collisionConnectData.emplace_back(
                    CollisionConnectData(&collisionItem->mLine.mNext, collisionItem->Next(), otherId)
                );

                collisionConnectData.emplace_back(
                        CollisionConnectData(&otherCollisionItem->mLine.mPrevious, otherCollisionItem->Previous(), id)
                );
            }

            if (startX == otherEndX && startY == otherEndY)
            {   
                collisionConnectData.emplace_back(
                    CollisionConnectData(&otherCollisionItem->mLine.mNext, otherCollisionItem->Next(), id)
                );
                collisionConnectData.emplace_back(
                    CollisionConnectData(&collisionItem->mLine.mPrevious, collisionItem->Previous(), otherId)
                );
            }

        }
    }

    return collisionConnectData;
}
