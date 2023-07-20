#pragma once


static ::eLineTypes FromLine(const OGData::eLineTypes type)
{
    switch (type)
    {
        case OGData::eLineTypes::eFloor_0:
            return ::eLineTypes::eFloor_0;
        case OGData::eLineTypes::eWallLeft_1:
            return ::eLineTypes::eWallLeft_1;
        case OGData::eLineTypes::eWallRight_2:
            return ::eLineTypes::eWallRight_2;
        case OGData::eLineTypes::eCeiling_3:
            return ::eLineTypes::eCeiling_3;
        case OGData::eLineTypes::eBackgroundFloor_4:
            return ::eLineTypes::eBackgroundFloor_4;
        case OGData::eLineTypes::eBackgroundWallLeft_5:
            return ::eLineTypes::eBackgroundWallLeft_5;
        case OGData::eLineTypes::eBackgroundWallRight_6:
            return ::eLineTypes::eBackgroundWallRight_6;
        case OGData::eLineTypes::eBackgroundCeiling_7:
            return ::eLineTypes::eBackgroundCeiling_7;
        case OGData::eLineTypes::eTrackLine_8:
            return ::eLineTypes::eTrackLine_8;
        case OGData::eLineTypes::eArt_9:
            return ::eLineTypes::eArt_9;
        case OGData::eLineTypes::eBulletWall_10:
            return ::eLineTypes::eBulletWall_10;
        case OGData::eLineTypes::eMineCarFloor_11:
            return ::eLineTypes::eMineCarFloor_11;
        case OGData::eLineTypes::eMineCarWall_12:
            return ::eLineTypes::eMineCarWall_12;
        case OGData::eLineTypes::eMineCarCeiling_13:
            return ::eLineTypes::eMineCarCeiling_13;
        case OGData::eLineTypes::eBackgroundMineCarFloor_14:
            return ::eLineTypes::eBackgroundMineCarFloor_14;
        case OGData::eLineTypes::eBackgroundMineCarWall_15:
            return ::eLineTypes::eBackgroundMineCarWall_15;
        case OGData::eLineTypes::eBackgroundMineCarCeiling_16:
            return ::eLineTypes::eBackgroundMineCarCeiling_16;
        case OGData::eLineTypes::eFlyingObjectWall_17:
            return ::eLineTypes::eFlyingObjectWall_17;
        case OGData::eLineTypes::eBackgroundFlyingObjectWall_18:
            return ::eLineTypes::eBackgroundFlyingObjectWall_18;
        case OGData::eLineTypes::eDynamicCollision_32:
            return ::eLineTypes::eDynamicCollision_32;
        case OGData::eLineTypes::eBackgroundDynamicCollision_36:
            return ::eLineTypes::eBackgroundDynamicCollision_36;
    }
    ALIVE_FATAL("Bad line type value");
}

static void ToPathLine(PathLine& dst, const PathLineAO& src)
{
    dst.mRect = src.mRect;
    dst.mLineType = FromLine(src.mLineType);
    dst.mPrevious = static_cast<s16>(src.mPrevious);
    dst.mNext = static_cast<s16>(src.mNext);
    dst.mLineLength = 0; // TODO: Calculate for AO in the future
}

static void ToPathLine(PathLine& dst, const PathLineAE& src)
{
    dst.mRect = src.mRect;
    dst.mLineType = FromLine(src.mLineType);
    dst.mPrevious = src.mPrevious;
    dst.mNext = src.mNext;
    dst.mLineLength = src.mLineLength;
}

static void ConvertPathCollisions(nlohmann::json& j, const CollisionInfo& info, const std::vector<u8>& pathResource, bool isAo)
{
    const u8* pData = pathResource.data();
    const u8* pStart = pData + info.mCollisionOffset;

    if (isAo)
    {
        auto pCollisions = reinterpret_cast<const PathLineAO*>(pStart);
        for (u32 i = 0; i < info.mNumCollisionItems; i++)
        {
            PathLine tmp;
            ToPathLine(tmp, pCollisions[i]);
            j.push_back(tmp);
        }
    }
    else
    {
        auto pCollisions = reinterpret_cast<const PathLineAE*>(pStart);
        for (u32 i = 0; i < info.mNumCollisionItems; i++)
        {
            PathLine tmp;
            ToPathLine(tmp, pCollisions[i]);
            j.push_back(tmp);
        }
    }
}
