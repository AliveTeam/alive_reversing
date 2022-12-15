#pragma once

#include "BaseGameObject.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "data_conversion/relive_tlvs.hpp"

enum class EReliveLevelIds : s16;

class Map;

class ShadowZone final : public BaseGameObject
{
public:
    static void MakeArray();
    static void FreeArray();

    ShadowZone(relive::Path_ShadowZone* pTlv, const Guid& tlvId);
    ~ShadowZone();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    void GetColourAmount(FP* pOut, s16 xpos, s16 ypos);
    static void ShadowZones_Calculate_Colour(s32 xpos, s32 ypos, Scale scale, s16* r, s16* g, s16* b);

    // Note: Completely inlined in AO
    s16 ApplysToScale(Scale scale);

    Guid mTlvId;
    s32 mCentreX = 0;
    s32 mCentreY = 0;
    s16 mCentreMidX = 0;
    s16 mCentreMidY = 0;
    s16 mMidX = 0;
    s16 mMidY = 0;
    relive::Path_ShadowZone::Scale mScale = relive::Path_ShadowZone::Scale::eBoth;
    FP mRed = {};
    FP mGreen = {};
    FP mBlue = {};
};
