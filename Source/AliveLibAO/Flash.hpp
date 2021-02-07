#pragma once

#include "FunctionFwd.hpp"
#include "EffectBase.hpp"
#include "Layer.hpp"

namespace AO {

class Flash : public EffectBase
{
public:
    EXPORT Flash* ctor_41A810(Layer layer, unsigned __int8 r, unsigned __int8 g, unsigned __int8 b, int not_used, TPageAbr abr, int time);


    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BaseGameObject* Vdtor_41A890(signed int flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_41A870();

    int field_68_flash_time;
};
ALIVE_ASSERT_SIZEOF(Flash, 0x6C);

}

