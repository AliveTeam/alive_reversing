#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/MapWrapper.hpp"

namespace relive
{
    struct Path_LevelLoader;
}

class LevelLoader final : public BaseGameObject
{
public:
    LevelLoader(relive::Path_LevelLoader* pTlv, const Guid& tlvId);

    virtual void VUpdate() override;

private:
    s16 mSwitchId = 0;
    enum class States : s16
    {
        eIdle_0 = 0,
        eFadingOut_1 = 1,
        eDone_2 = 2,
    };
    States mState = States::eIdle_0;
    EReliveLevelIds mDestLevel = EReliveLevelIds::eNone;
    s16 mDestPath = 0;
    s16 mDestCamera = 0;
    s16 mMovieId = 0;
    Guid mTlvId;
    Guid mFadeId;
};
