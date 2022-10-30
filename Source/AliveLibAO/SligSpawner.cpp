#include "stdafx_ao.h"
#include "Function.hpp"
#include "SligSpawner.hpp"
#include "Slig.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "../AliveLibAE/stdlib.hpp"

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
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void SligSpawner::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (SwitchStates_Get(mSligSpawnerSwitchId))
    {
        auto pTlv = static_cast<relive::Path_Slig*>(gMap.VTLV_Get_At(
            mPathTlv.mTopLeftX,
            mPathTlv.mTopLeftY,
            mPathTlv.mTopLeftX,
            mPathTlv.mTopLeftY,
            ReliveTypes::eSligSpawner));

        if (pTlv)
        {
            relive_new Slig(pTlv, {});
        }

        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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
