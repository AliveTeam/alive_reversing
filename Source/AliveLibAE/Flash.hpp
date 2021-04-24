#pragma once

#include "FunctionFwd.hpp"
#include "EffectBase.hpp"
#include "Layer.hpp"

class Flash : public EffectBase
{
public:
    EXPORT Flash* ctor_428570(Layer layer, u8 r, u8 g, u8 b, int /*not_used*/, TPageAbr abr, int time);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
private:
    EXPORT Flash* vdtor_4285F0(signed int flags);
    EXPORT void vUpdate_428640();
private:
    int field_78_flash_time;
};
ALIVE_ASSERT_SIZEOF(Flash, 0x7C);
