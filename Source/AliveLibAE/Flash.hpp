#pragma once

#include "FunctionFwd.hpp"
#include "EffectBase.hpp"
#include "Layer.hpp"

class Flash : public EffectBase
{
public:
    EXPORT Flash* ctor_428570(Layer layer, unsigned __int8 r, unsigned __int8 g, unsigned __int8 b, int /*not_used*/, unsigned __int8 abr, int time);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
private:
    EXPORT Flash* vdtor_4285F0(signed int flags);
    EXPORT void vUpdate_428640();
private:
    int field_78_flash_time;
};
ALIVE_ASSERT_SIZEOF(Flash, 0x7C);
