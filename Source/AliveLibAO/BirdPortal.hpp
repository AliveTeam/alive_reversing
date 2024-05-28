#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../relive_lib/GameObjects/IBirdPortal.hpp"

enum class EReliveLevelIds : s16;

namespace AO {

enum class LevelIds : s16;

class BirdPortal final : public IBirdPortal
{
public:
    BirdPortal(relive::Path_BirdPortal* pTlv, const Guid& tlvId);
    ~BirdPortal();
    
    virtual void VUpdate() override;

    virtual void VGiveShrykull(s16 bPlaySound) override;
    virtual void VExitPortal() override;
    virtual void VGetMapChange(EReliveLevelIds* level, u16* path, u16* camera, CameraSwapEffects* screenChangeEffect, u16* movieId) override;

private:
    s16 IsScaredAway();
};

} // namespace AO
