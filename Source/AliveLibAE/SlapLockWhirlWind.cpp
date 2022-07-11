#include "stdafx.h"
#include "SlapLockWhirlWind.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "OrbWhirlWind.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Door.hpp"

s32 SlapLockWhirlWind::CreateFromSaveState(const u8* pBuffer)
{
    auto pSaveState = reinterpret_cast<const SlapLockWhirlWind_State*>(pBuffer);
    SwitchStates_Do_Operation(pSaveState->field_2_switch_id, SwitchOp::eSetTrue_0);
    return sizeof(SlapLockWhirlWind_State);
}

SlapLockWhirlWind::SlapLockWhirlWind(s16 doorNumber, s16 switchId, FP xpos, FP ypos, FP scale)
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eSlapLock_OrbWhirlWind);

    field_20_xpos = xpos;
    field_24_ypos = ypos;

    field_28_scale = scale;
    field_44_switch_id = switchId;

    bool bFoundTarget = false;
    for (s16 y = 0; y < sPathInfo->mCamsOnY; y++)
    {
        for (s16 x = 0; x < sPathInfo->mCamsOnX; x++)
        {
            Path_Door* pDoorTlv = static_cast<Path_Door*>(sPathInfo->Get_First_TLV_For_Offsetted_Camera(
                x - gMap.mCamIdxOnX,
                y - gMap.mCamIdxOnY));
            while (pDoorTlv)
            {
                if (pDoorTlv->mTlvType32 == TlvTypes::Door_5 && pDoorTlv->field_18_door_number == doorNumber)
                {
                    // For some reason once found we just keep on searching...
                    bFoundTarget = true;

                    field_2C_door_x = FP_FromInteger((pDoorTlv->mTopLeft.x + pDoorTlv->mBottomRight.x) / 2);
                    field_30_door_y = FP_FromInteger((pDoorTlv->mTopLeft.y + pDoorTlv->mBottomRight.y) / 2);

                    if (pDoorTlv->field_16_scale != Scale_short::eFull_0)
                    {
                        field_34_door_scale = FP_FromDouble(0.5);
                    }
                    else
                    {
                        field_34_door_scale = FP_FromInteger(1);
                    }

                    field_30_door_y -= (FP_FromInteger(40) * field_34_door_scale);
                }
                pDoorTlv = static_cast<Path_Door*>(sPathInfo->Next_TLV(pDoorTlv));
            }
        }
    }

    if (bFoundTarget)
    {
        auto pWhirlWind = relive_new OrbWhirlWind(xpos, ypos, scale, 1);
        if (pWhirlWind)
        {
            field_38_orb_whirlwind_id = pWhirlWind->mBaseGameObjectId;
        }
        field_3C_state = 0;
        field_40_timer = sGnFrame + 70;
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

s32 SlapLockWhirlWind::VGetSaveState(u8* pSaveBuffer)
{
    SlapLockWhirlWind_State* pSaveState = reinterpret_cast<SlapLockWhirlWind_State*>(pSaveBuffer);
    pSaveState->field_0_type = AETypes::eSlapLock_OrbWhirlWind_60;
    pSaveState->field_2_switch_id = field_44_switch_id;
    return sizeof(SlapLockWhirlWind_State);
}

void SlapLockWhirlWind::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        OrbWhirlWind* pWhirlWind = static_cast<OrbWhirlWind*>(sObjectIds.Find_Impl(field_38_orb_whirlwind_id));
        if (field_3C_state == 1)
        {
            if (!(static_cast<s32>(sGnFrame) % 10))
            {
                SFX_Play_Pitch(
                    SoundEffect::FlyingSpirit2_108,
                    static_cast<s16>(127 - (static_cast<s32>(sGnFrame) - field_40_timer) / 2),
                    4 * (sGnFrame - field_40_timer));
            }

            if (!pWhirlWind || pWhirlWind->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                SwitchStates_Do_Operation(field_44_switch_id, SwitchOp::eSetTrue_0);
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
        }
        else if (field_3C_state == 0)
        {
            if (!(static_cast<s32>(sGnFrame) % 10))
            {
                if (static_cast<s32>(sGnFrame) % 20)
                {
                    SfxPlayMono(SoundEffect::FlyingSpirit1_107, 0);
                }
                else
                {
                    SfxPlayMono(SoundEffect::FlyingSpirit2_108, 0);
                }
            }

            if (static_cast<s32>(sGnFrame) > field_40_timer)
            {
                if (pWhirlWind)
                {
                    pWhirlWind->ToSpin(field_2C_door_x, field_30_door_y, field_34_door_scale, 0);
                }
                field_3C_state = 1;
            }
        }
    }
}

s16 SlapLockWhirlWind::SwitchId() const
{
    return field_44_switch_id;
}
