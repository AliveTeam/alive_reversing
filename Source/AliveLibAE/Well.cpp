#include "stdafx.h"
#include "Well.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/Leaf.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/Events.hpp"
#include "stdlib.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Map.hpp"
#include "Path.hpp"

static u32 sWellRndSeed = 4;

static s16 Well_NextRandom()
{
    const auto curRand = gRandomBytes[sWellRndSeed];
    sWellRndSeed++;

    if (sWellRndSeed > 255)
    {
        sWellRndSeed = 0;
    }

    return curRand;
}

Well::Well(relive::Path_WellBase* pTlv, FP xpos, FP ypos, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    mTlvInfo = tlvId;
    SetType(ReliveTypes::eWell);

    if (pTlv->mTlvType == ReliveTypes::eWellLocal)
    {
        WellLocal_Init(static_cast<relive::Path_WellLocal*>(pTlv), xpos, ypos);
    }
    else
    {
        WellExpress_Init(static_cast<relive::Path_WellExpress*>(pTlv), xpos, ypos);
    }
}

Well::~Well()
{
    if (mTlvInfo != Guid{})
    {
        Path::TLV_Reset(mTlvInfo);
    }
}

void Well::WellExpress_Init(relive::Path_WellExpress* pTlv, FP /*xpos*/, FP ypos)
{
    if (pTlv->mScale != relive::reliveScale::eFull)
    {
        mLeafScale = FP_FromDouble(0.5);
    }
    else
    {
        mLeafScale = FP_FromInteger(1);
    }

    mSwitchId = pTlv->mSwitchId;

    mExitX = FP_FromInteger(pTlv->mExitX) / FP_FromInteger(100);
    mExitY = FP_FromInteger(pTlv->mExitY) / FP_FromInteger(100);

    mEmitLeaves = pTlv->mEmitLeaves;
    if (mEmitLeaves)
    {
        PSX_Point abeSpawnPos = {};
        gMap.Get_Abe_Spawn_Pos(&abeSpawnPos);

        mLeafX = FP_FromInteger(pTlv->mLeafX);
        if (pTlv->mLeafX > 0)
        {
            mLeafX += FP_FromInteger(abeSpawnPos.x);
        }
        else
        {
            const s32 pos = (PsxToPCX(pTlv->Width()) / 2) + pTlv->mTopLeftX;
            mLeafX = FP_FromInteger(pos);
        }

        mLeafY = FP_FromInteger(pTlv->mLeafY);
        if (pTlv->mLeafY > 0)
        {
            mLeafY += FP_FromInteger(abeSpawnPos.y);
        }
        else
        {
            mLeafY = ypos;
        }
    }
}

void Well::WellLocal_Init(relive::Path_WellLocal* pTlv, FP /*xpos*/, FP ypos)
{
    if (pTlv->mScale != relive::reliveScale::eFull)
    {
        mLeafScale = FP_FromDouble(0.5);
    }
    else
    {
        mLeafScale = FP_FromInteger(1);
    }

    mSwitchId = pTlv->mSwitchId;

    mEmitLeaves = pTlv->mEmitLeaves;
    if (mEmitLeaves)
    {
        PSX_Point abeSpawnPos = {};
        gMap.Get_Abe_Spawn_Pos(&abeSpawnPos);
        mLeafX = FP_FromInteger(pTlv->mLeafX);

        if (pTlv->mLeafX > 0)
        {
            mLeafX += FP_FromInteger(abeSpawnPos.x);
        }
        else
        {
            const s32 pos = (PsxToPCX(pTlv->Width()) / 2) + pTlv->mTopLeftX;
            mLeafX = FP_FromInteger(pos);
        }

        mLeafY = FP_FromInteger(pTlv->mLeafY);
        if (pTlv->mLeafY > 0)
        {
            mLeafY += FP_FromInteger(abeSpawnPos.y);
        }
        else
        {
            mLeafY = ypos;
        }
    }
}

void Well::VScreenChanged()
{
    SetDead(true);
}

void Well::VUpdate()
{
    if (EventGet(Event::kEventDeathReset))
    {
        SetDead(true);
        // Reset well state when Abe dies.
        Path::TLV_Reset(mTlvInfo);
    }

    if (mEmitLeaves)
    {
        // Always on or has been enabled?
        if (mSwitchId == 0 || SwitchStates_Get(mSwitchId))
        {
            // Random chance of leaves emitting.
            if (Well_NextRandom() < 10)
            {
                relive_new Leaf(mLeafX,
                                          mLeafY,
                                          FP_FromInteger(2),
                                          FP_FromInteger(-20),
                                          mLeafScale);
            }
        }
    }
}
