#pragma once

#include "FunctionFwd.hpp"
#include "EffectBase.hpp"
#include "Layer.hpp"

namespace AO {

class Flash : public EffectBase
{
public:
    EXPORT Flash* ctor_41A810(Layer layer, u8 r, u8 g, u8 b, int not_used = 1, TPageAbr abr = TPageAbr::eBlend_3, int time = 1);


    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BaseGameObject* Vdtor_41A890(signed int flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_41A870();

    int field_68_flash_time;
};
ALIVE_ASSERT_SIZEOF(Flash, 0x6C);

}

