#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

class CircularFade : public BaseAnimatedWithPhysicsGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;

    // New virtuals
    virtual int Vsub_4CE300(__int16 direction, char destroyOnDone);
    virtual int Vsub_4CE0B0();

    EXPORT CircularFade* ctor_4CE100(FP xpos, FP ypos, FP scale, __int16 direction, char destroyOnDone);
private:
    EXPORT void vRender_4CE3F0(int **pOt);
    EXPORT void vUpdate_4CE380();
    EXPORT int vsub_4CE300(__int16 direction, char destroyOnDone); // TODO: Likely no return
    EXPORT int vsub_4CE0B0();
    EXPORT void dtor_4CE080();
    EXPORT CircularFade* vdtor_4CE0D0(signed int flags);
private:
    // Never used ?
    int field_E4[4];
    enum Flags
    {
        eBit1_FadeIn = 0x1,
        eBit2_Done = 0x2,
        eBit3_DestroyOnDone = 0x4,
        eBit4_NeverSet = 0x8
    };
    BitField16<Flags> field_F4_flags;
    //__int16 field_F6; // pad?
    Prim_Tile field_F8[2];
    Prim_Tile field_120[2];
    Prim_Tile field_148[2];
    Prim_Tile field_170[2];
    Prim_SetTPage field_198_tPages[2];
    __int16 field_1B8_fade_colour;
    __int16 field_1BA_speed;
};
ALIVE_ASSERT_SIZEOF(CircularFade, 0x1BC);

EXPORT CircularFade* CC Make_Circular_Fade_4CE8C0(FP xpos, FP ypos, FP scale, __int16 direction, char destroyOnDone, char setBit8);
