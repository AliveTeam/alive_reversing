#include "stdafx.h"
#include "ExplosionSet.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/Events.hpp"
#include "Explosion.hpp"
#include "SwitchStates.hpp"
#include "FallingItem.hpp"
#include "Grid.hpp"

ALIVE_VAR(1, 0x5BBF68, ExplosionSet*, pExplosionSet_5BBF68, nullptr);
ALIVE_VAR(1, 0x5C1BB6, s16, bEnabled_5C1BB6, FALSE);

ExplosionSet::ExplosionSet()
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eExplosionSet);

    if (pExplosionSet_5BBF68)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        pExplosionSet_5BBF68 = this;
        mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);
        field_50_scale = FP_FromInteger(1);
        field_40 = 0;
        field_42 = 1;
        field_44_start_delay = 0;
        field_46_spacing_multiplicator = 0;
        bEnabled_5C1BB6 = FALSE;
        field_5C_flags.Clear(Flags_5C::eBit3_Active);
        gObjListDrawables->Push_Back(this);
    }
}

void ExplosionSet::Start()
{
    bEnabled_5C1BB6 = 1;
}

void ExplosionSet::Init(relive::Path_ExplosionSet* pTlv)
{
    field_48_tlv_rect.x = pTlv->mTopLeft.x;
    field_48_tlv_rect.y = pTlv->mTopLeft.y;
    field_48_tlv_rect.w = pTlv->mBottomRight.x - pTlv->mTopLeft.x;
    field_48_tlv_rect.h = pTlv->mBottomRight.y - pTlv->mTopLeft.y;

    if (pTlv->mScale != Scale_short::eFull_0)
    {
        field_50_scale = FP_FromDouble(0.5);
    }

    field_44_start_delay = pTlv->mStartDelay;

    field_5C_flags.Set(Flags_5C::eBit1_spawn_assets, pTlv->mSpawnAssets == Choice_short::eYes_1);
    field_5C_flags.Set(Flags_5C::eBit2_flipX, pTlv->mStartDirection == XDirection_short::eRight_1);
    field_56_asset_interval = pTlv->mAssetInterval;
    field_58_grid_spacing = FP_GetExponent(FP_FromInteger(pTlv->mGridSpacing) * ScaleToGridSize(field_50_scale));
    field_5A_increasing_grid_spacing = FP_GetExponent(FP_FromInteger(pTlv->mIncreasingGridSpacing) * ScaleToGridSize(field_50_scale));
    field_54_switch_id = pTlv->mSwitchId;

    if (!bEnabled_5C1BB6)
    {
        bEnabled_5C1BB6 = static_cast<s16>(pTlv->mStartEnabled);
    }

    field_5C_flags.Set(Flags_5C::eBit3_Active);
    field_46_spacing_multiplicator = 0;
}

ExplosionSet::~ExplosionSet()
{
    gObjListDrawables->Remove_Item(this);
    pExplosionSet_5BBF68 = nullptr;
}

void ExplosionSet::VScreenChanged()
{
    if (gMap.mCurrentLevel == gMap.mNextLevel && gMap.mCurrentPath == gMap.mNextPath)
    {
        field_5C_flags.Clear(Flags_5C::eBit3_Active);
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

struct Point2 final
{
    s8 x, y;
};

const Point2 stru_550F38[12] = {
    {0, -2},
    {-2, 0},
    {2, -2},
    {-2, 2},
    {0, -3},
    {-3, 0},
    {3, -3},
    {-3, 3},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0}};

void ExplosionSet::VRender(PrimHeader** ppOt)
{
    if (bEnabled_5C1BB6)
    {
        Prim_ScreenOffset* pScreenOff = &field_20[gPsxDisplay.mBufferIndex];

        PSX_Pos16 point = {};
        point.x = stru_550F38[field_40].x;
        point.y = stru_550F38[field_40].y;
        if (gPsxDisplay.mBufferIndex)
        {
            point.y += 256;
        }

        InitType_ScreenOffset(pScreenOff, &point);
        OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_0), &pScreenOff->mBase);

        /* constant 1, so always false, dead code.
        if (!dword_55EF94)
        {
            pScreenManager->InvalidateRectCurrentIdx(0, 0, 640, 240);
        }*/
    }
}

void ExplosionSet::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (bEnabled_5C1BB6)
    {
        field_40 += field_42;

        if (field_40 >= 7 || field_40 <= 0)
        {
            field_42 = -field_42;
        }

        if (field_5C_flags.Get(Flags_5C::eBit3_Active) && field_5C_flags.Get(Flags_5C::eBit1_spawn_assets))
        {
            if (field_44_start_delay > 0)
            {
                field_44_start_delay--;
                return;
            }

            s16 xpos = 0;
            if (field_5C_flags.Get(Flags_5C::eBit2_flipX))
            {
                xpos = field_48_tlv_rect.w + field_48_tlv_rect.x - (field_46_spacing_multiplicator * field_5A_increasing_grid_spacing) - field_58_grid_spacing;
                if (xpos <= field_48_tlv_rect.x)
                {
                    xpos = field_48_tlv_rect.w + field_48_tlv_rect.x - field_58_grid_spacing;
                    field_46_spacing_multiplicator = 0;
                }
            }
            else
            {
                xpos = field_58_grid_spacing + field_48_tlv_rect.x + (field_46_spacing_multiplicator * field_5A_increasing_grid_spacing);
                if (xpos >= field_48_tlv_rect.x + field_48_tlv_rect.w)
                {
                    xpos = field_48_tlv_rect.x + field_58_grid_spacing;
                    field_46_spacing_multiplicator = 0;
                }
            }

            relive_new FallingItem(xpos, field_48_tlv_rect.y, field_50_scale < FP_FromInteger(1), 0, 0, 1, 0);

            field_46_spacing_multiplicator++;
            field_44_start_delay = field_56_asset_interval;

            if (gMap.mCurrentLevel == EReliveLevelIds::eMines && Math_RandomRange(1, 5) >= 4)
            {
                const FP explodeX = FP_FromInteger(Math_RandomRange(field_48_tlv_rect.y + 20, field_48_tlv_rect.y + 230));
                const FP explodeY = FP_FromInteger(Math_RandomRange(field_48_tlv_rect.x, xpos));
                relive_new Explosion(explodeY, explodeX, field_50_scale, 0);
            }
        }
    }
    else
    {
        if (field_5C_flags.Get(Flags_5C::eBit3_Active))
        {
            if (field_54_switch_id > 0)
            {
                if (SwitchStates_Get(field_54_switch_id))
                {
                    bEnabled_5C1BB6 = 1;
                }
            }
        }
    }
}
