#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "SligSpawner.hpp"
#include "Slig.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Path.hpp"

namespace AO {

SligSpawner::SligSpawner(relive::Path_TLV* pTlv, relive::Path_Slig_Data* pTlvData, const Guid& tlvId)
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
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (SwitchStates_Get(mSligSpawnerSwitchId))
    {
        auto pTlv = static_cast<relive::Path_Slig*>(gMap.VTLV_Get_At_Of_Type(
            mPathTlv.mTopLeftX,
            mPathTlv.mTopLeftY,
            mPathTlv.mTopLeftX,
            mPathTlv.mTopLeftY,
            ReliveTypes::eSligSpawner));

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
        Path::TLV_Reset(mTlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 1);
    }
}

} // namespace AO
