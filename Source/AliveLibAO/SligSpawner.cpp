#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "SligSpawner.hpp"
#include "Slig.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Path.hpp"
#include "Map.hpp"

namespace AO {

SligSpawner::SligSpawner(relive::Path_TLV* pTlv, relive::Path_Slig* pTlvData, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eSligSpawner);

    mTlvInfo = tlvId;
    mPathTlv = *pTlv;

    mSpawnerFlags = 1;

    mSligSpawnerSwitchId = pTlvData->mSligSpawnerSwitchId;
}

void SligSpawner::VScreenChanged()
{
    SetDead(true);
}

void SligSpawner::VUpdate()
{
    if (EventGet(Event::kEventDeathReset))
    {
        SetDead(true);
    }

    if (SwitchStates_Get(mSligSpawnerSwitchId))
    {
        auto pTlv = gMap.VTLV_Get_At_Of_Type(
            mPathTlv.mTopLeftX,
            mPathTlv.mTopLeftY,
            mPathTlv.mTopLeftX,
            mPathTlv.mTopLeftY,
            ReliveTypes::eSligSpawner).GetTlv<relive::Path_Slig>();

        if (pTlv)
        {
            relive_new Slig(pTlv, {});
        }

        SetDead(true);
        mSpawnerFlags = 0;
    }
}

SligSpawner::~SligSpawner()
{
    if (mSpawnerFlags)
    {
        Path::TLV_Reset(mTlvInfo);
    }
    else
    {
        Path::TLV_Delete(mTlvInfo);
    }
}

} // namespace AO
