#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "EffectBase.hpp"
#include "Layer.hpp"

namespace AO {

class Flash final : public EffectBase
{
public:
    EXPORT Flash* ctor_41A810(Layer layer, u8 r, u8 g, u8 b, s32 not_used = 1, TPageAbr abr = TPageAbr::eBlend_3, s32 time = 1);


    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT BaseGameObject* Vdtor_41A890(s32 flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_41A870();

    s32 field_68_flash_time;
};
ALIVE_ASSERT_SIZEOF(Flash, 0x6C);

} // namespace AO
