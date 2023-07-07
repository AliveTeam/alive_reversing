#pragma once

struct SnapSetting final
{
    bool mSnapX = false;
    bool mSnapY = false;
};

class SnapSettings final
{
public:
    const SnapSetting& MapObjectSnapping() const
    {
        return mMapObjectSnappingSettings;
    }

    SnapSetting& MapObjectSnapping()
    {
        return mMapObjectSnappingSettings;
    }

    SnapSetting& CollisionSnapping()
    {
        return mCollisionSnappingSettings;
    }

    const SnapSetting& CollisionSnapping() const
    {
        return mCollisionSnappingSettings;
    }

private:
    SnapSetting mMapObjectSnappingSettings;
    SnapSetting mCollisionSnappingSettings;
};

class IPointSnapper
{
public:
    virtual ~IPointSnapper() = default;
    virtual int SnapX(bool enabled, int x) = 0;
    virtual int SnapY(bool enabled, int y) = 0;

};
