#include "stdafx.h"
#include "Well.hpp"
#include "Function.hpp"
#include "Leaf.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "Path.hpp"
#include "../relive_lib/Events.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"

ALIVE_VAR(1, 0x563aa0, u32, sWellRndSeed_563AA0, 4);

Well::Well(Path_WellBase* pTlv, FP xpos, FP ypos, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    field_20_tlvInfo = tlvInfo;
    SetType(ReliveTypes::eWell);

    if (pTlv->mTlvType32 == TlvTypes::LocalWell_8)
    {
        WellLocal_Init(static_cast<Path_WellLocal*>(pTlv), xpos, ypos);
    }
    else
    {
        WellExpress_Init(static_cast<Path_WellExpress*>(pTlv), xpos, ypos);
    }
}

void Well::WellExpress_Init(Path_WellExpress* pTlv, FP /*xpos*/, FP ypos)
{
    if (pTlv->field_0_scale != Scale_short::eFull_0)
    {
        field_28_scale = FP_FromDouble(0.5);
    }
    else
    {
        field_28_scale = FP_FromInteger(1);
    }

    field_24_switch_id = pTlv->field_2_switch_id;

    field_2C_exit_x = FP_FromInteger(pTlv->field_18_exit_x) / FP_FromInteger(100);
    field_30_exit_y = FP_FromInteger(pTlv->field_1A_exit_y) / FP_FromInteger(100);

    field_3C_bEmitLeaves = pTlv->field_2C_bEmit_leaves;
    if (field_3C_bEmitLeaves == Choice_short::eYes_1)
    {
        PSX_Point abeSpawnPos = {};
        gMap.Get_Abe_Spawn_Pos(&abeSpawnPos);

        field_34_leaf_xpos = FP_FromInteger(pTlv->field_2E_leaf_x);
        if (pTlv->field_2E_leaf_x > 0)
        {
            field_34_leaf_xpos += FP_FromInteger(abeSpawnPos.x);
        }
        else
        {
            const s32 pos = (PsxToPCX(pTlv->mBottomRight.x - pTlv->mTopLeft.x) / 2) + pTlv->mTopLeft.x;
            field_34_leaf_xpos = FP_FromInteger(pos);
        }

        field_38_leaf_ypos = FP_FromInteger(pTlv->field_30_leaf_y);
        if (pTlv->field_30_leaf_y > 0)
        {
            field_38_leaf_ypos += FP_FromInteger(abeSpawnPos.y);
        }
        else
        {
            field_38_leaf_ypos = ypos;
        }
    }
}

void Well::WellLocal_Init(Path_WellLocal* pTlv, FP /*xpos*/, FP ypos)
{
    if (pTlv->field_0_scale != Scale_short::eFull_0)
    {
        field_28_scale = FP_FromDouble(0.5);
    }
    else
    {
        field_28_scale = FP_FromInteger(1);
    }

    field_24_switch_id = pTlv->field_2_switch_id;

    field_3C_bEmitLeaves = pTlv->field_20_bEmit_leaves;
    if (field_3C_bEmitLeaves == Choice_short::eYes_1)
    {
        PSX_Point abeSpawnPos = {};
        gMap.Get_Abe_Spawn_Pos(&abeSpawnPos);
        field_34_leaf_xpos = FP_FromInteger(pTlv->field_22_leaf_x);

        if (pTlv->field_22_leaf_x > 0)
        {
            field_34_leaf_xpos += FP_FromInteger(abeSpawnPos.x);
        }
        else
        {
            const s32 pos = (PsxToPCX(pTlv->mBottomRight.x - pTlv->mTopLeft.x) / 2) + pTlv->mTopLeft.x;
            field_34_leaf_xpos = FP_FromInteger(pos);
        }

        field_38_leaf_ypos = FP_FromInteger(pTlv->field_24_leaf_y);
        if (pTlv->field_24_leaf_y > 0)
        {
            field_38_leaf_ypos += FP_FromInteger(abeSpawnPos.y);
        }
        else
        {
            field_38_leaf_ypos = ypos;
        }
    }
}

Well::~Well()
{
    if (field_20_tlvInfo != -1)
    {
        Path::TLV_Reset(field_20_tlvInfo, -1, 0, 0);
    }
}

void Well::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

static s16 Well_NextRandom()
{
    const auto curRand = sRandomBytes_546744[sWellRndSeed_563AA0];
    sWellRndSeed_563AA0++;

    if (sWellRndSeed_563AA0 > 255)
    {
        sWellRndSeed_563AA0 = 0;
    }

    return curRand;
}

void Well::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        // Reset well state when Abe dies.
        Path::TLV_Reset(field_20_tlvInfo, -1, 0, 0);
    }

    if (field_3C_bEmitLeaves == Choice_short::eYes_1)
    {
        // Always on or has been enabled?
        if (field_24_switch_id == 0 || SwitchStates_Get(field_24_switch_id))
        {
            // Random chance of leaves emitting.
            if (Well_NextRandom() < 10)
            {
                relive_new Leaf(field_34_leaf_xpos,
                                          field_38_leaf_ypos,
                                          FP_FromInteger(2),
                                          FP_FromInteger(-20),
                                          field_28_scale);
            }
        }
    }
}
