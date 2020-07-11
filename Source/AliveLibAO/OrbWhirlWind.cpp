#include "stdafx_ao.h"
#include "Function.hpp"
#include "OrbWhirlWind.hpp"
#include "Animation.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "ScreenManager.hpp"
#include "BaseAliveGameObject.hpp"

START_NS_AO

ALIVE_VAR(1, 0x9F30A0, int, gOrbWhirlWind_instace_count_9F30A0, 0);

class OrbWhirlWindParticle
{
public:
    EXPORT OrbWhirlWindParticle* ctor_48BC10(FP xpos, FP ypos, FP scale)
    {
        SetVTable(&field_8_anim, 0x4BA2B8);

        SetVTable(this, 0x4BD7B8);

        BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 312, 1, 0);
        field_8_anim.Init_402D20(1492, gObjList_animations_505564, 0, 38, 21, ppRes, 1, 0, 0);

        field_8_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
        
        field_8_anim.field_C_layer = 32;
        field_8_anim.field_B_render_mode = 1;

        field_8_anim.field_8_r = 80;
        field_8_anim.field_9_g = 80;
        field_8_anim.field_A_b = 80;

        field_8_anim.SetFrame_402AC0(Math_RandomRange_450F20(0, field_8_anim.Get_Frame_Count_403540() - 1));
        field_4_flags &= ~1u;
        field_B4_state = 0;
        field_B8 =  Math_RandomRange_450F20(0, 255);
        field_BC = 1;
        field_BE = Math_RandomRange_450F20(9, 15);
        field_CC_xpos_mid = xpos;
        field_D0_ypos_mid = ypos + FP_FromInteger(Math_RandomRange_450F20(-12, 12));
        field_E0_yMove = ypos -FP_FromInteger(16);
        field_D4_radiusX = FP_FromInteger(Math_RandomRange_450F20(37, 43));
        field_D8_radiosY = FP_FromDouble(0.25) * field_D4_radiusX;
        field_C0_scale = scale;
        field_C4_randomized_scale = FP_FromInteger( Math_RandomRange_450F20(7, 10)) / FP_FromInteger(10);
        field_A8_render_as_scale = field_C0_scale * field_C4_randomized_scale;
        return this;
    }


    // WARNING: Not a derived type - keep virtuals in order for BC
    virtual EXPORT OrbWhirlWindParticle* Vdtor_48C510(signed int flags)
    {
        SetVTable(this, 0x4BD7B8);

        field_8_anim.vCleanUp();

        if (flags & 1)
        {
            ao_delete_free_447540(this);
        }
        return this;
    }

    virtual EXPORT void VUpdate_48BF00()
    {
        switch (field_B4_state)
        {
        case 0:
            sub_48BDC0(1);
            break;

        case 1:
            if (static_cast<int>(gnFrameCount_507670) < field_DC + 16)
            {
                if (static_cast<int>(gnFrameCount_507670) >= field_DC)
                {
                    field_D0_ypos_mid += field_B0;
                    field_D4_radiusX -= FP_FromInteger(2);
                    field_D8_radiosY -= FP_FromInteger(1);
                }
                sub_48BDC0(1);
            }
            else
            {
                if (!field_E4_pObj || field_E4_pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3))
                {
                    ToStop();
                }
                else
                {
                    field_F8 = field_CC_xpos_mid;
                    field_F0 = field_CC_xpos_mid;
                    field_FC = field_D0_ypos_mid;
                    field_F4 = field_D0_ypos_mid;
                    field_C8 = (field_E4_pObj->field_BC_sprite_scale - field_C0_scale) / FP_FromInteger(16);
                    field_DC = gnFrameCount_507670 + 16;
                    field_B4_state = 2;
                    sub_48BDC0(1);
                }
            }
            break;

        case 2:
            if (!field_E4_pObj || field_E4_pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3))
            {
                ToStop();
            }
            else
            {
                PSX_RECT v44 = {};
                field_E4_pObj->VGetBoundingRect(&v44, 1);

                const FP v19 = FP_FromInteger((v44.x + v44.w) / 2);
                const FP v20 = FP_FromInteger((v44.y + v44.h) / 2);

                if (static_cast<int>(gnFrameCount_507670) < field_DC)
                {
                    field_C0_scale += field_C8;
                    const FP v25 = FP_FromInteger(16 - (field_DC + gnFrameCount_507670)) / FP_FromInteger(16);
                    field_F8 = ((v19 - field_F0) * v25) + field_F0;
                    field_FC = ((v20 - field_F4) * v25) + field_F4;
                    field_CC_xpos_mid = field_F8 + ((FP_FromInteger(32) * field_C0_scale) *  Math_Sine_451110(FP_GetExponent(FP_FromInteger(128) * v25) & 0xFF));
                    field_D0_ypos_mid = field_FC + ((FP_FromInteger(32) * field_C0_scale) *  Math_Cosine_4510A0(FP_GetExponent(FP_FromInteger(128) * v25) & 0xFF));
                }
                else
                {
                    field_F8 = v19;
                    field_FC = v20;
                    field_CC_xpos_mid = v19;
                    field_D0_ypos_mid = v20;
                    field_B8 = 192;
                    field_D4_radiusX = FP_FromInteger(40);
                    field_AC = field_D4_radiusX / FP_FromInteger(32);
                    field_100 = field_C0_scale * FP_FromInteger(Math_RandomRange_450F20(-16, 16));
                    field_DC = gnFrameCount_507670 + 32;
                    field_B4_state = 3;
                }
                sub_48BDC0(1);
            }
            break;

        case 3:
            if (static_cast<int>(gnFrameCount_507670) >= field_DC)
            {
                SetActive(IsActive() ? 0 : 1);
            }
            field_D0_ypos_mid = (field_100 *  Math_Cosine_4510A0(
                FP_GetExponent(
                    FP_FromInteger(128) * 
                    FP_FromInteger(32 - (field_DC + gnFrameCount_507670)) / FP_FromInteger(32)) & 0xFF)) + field_FC;
            field_D4_radiusX -= field_AC;
            sub_48BDC0(1);
            break;

        case 4:
            if (static_cast<int>(gnFrameCount_507670) >= field_DC)
            {
                SetActive(IsActive() ? 0 : 1);
            }
            sub_48BDC0(0);
            break;

        default:
            return;
        }
    }

    EXPORT void SetActive(unsigned __int8 active)
    {
        if (active)
        {
            field_4_flags |= 1;
        }
        else
        {
            field_4_flags &= ~1;
        }
    }

    virtual EXPORT void VRender_48C430(int** ppOt)
    {
        field_8_anim.field_14_scale = field_A8_render_as_scale;

        const short xpos = FP_GetExponent(FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos)
            + field_A0
            - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x);

        const short ypos = FP_GetExponent(FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos + 5)
            + field_A4
            - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y);

        field_8_anim.VRender_403AE0(
            xpos,
            ypos,
            ppOt,
            0,
            0);

        PSX_RECT rect = {};
        field_8_anim.Get_Frame_Rect_402B50(&rect);
        pScreenManager_4FF7C8->InvalidateRect_406E40(
            rect.x,
            rect.y,
            rect.w,
            rect.h,
            pScreenManager_4FF7C8->field_2E_idx);
    }

    // NOTE: inlined
    int IsActive()
    {
        return field_4_flags & 1;
    }

    // NOTE: inlined
    void ToStop()
    {
        field_B4_state = 4;// ToStop
        field_DC = gnFrameCount_507670 + Math_RandomRange_450F20(0, 32);
    }

    EXPORT void sub_48BDC0(__int16 a2)
    {
        field_B8 += field_BC;

        if (a2 && field_BC <= field_BE && !(gnFrameCount_507670 % 3))
        {
            field_BC++;
        }
        else if (!a2 && field_BC >= 1 && !(gnFrameCount_507670 % 3))
        {
            field_BC--;
            field_D4_radiusX += FP_FromInteger(4);
        }

        field_A0 = ((field_C0_scale * field_D4_radiusX) *  Math_Sine_451110(static_cast<BYTE>(field_B8))) + field_CC_xpos_mid;
        field_A4 = ((field_C0_scale * field_D8_radiosY) * Math_Cosine_4510A0(static_cast<BYTE>(field_B8))) + field_D0_ypos_mid;
        field_A8_render_as_scale = (field_C0_scale * field_C4_randomized_scale);
        if (field_C0_scale > FP_FromDouble(0.599))
        {
            field_8_anim.field_C_layer = 32;
        }
        else
        {
            field_8_anim.field_C_layer = 13;
        }
    }

    void Spin(FP xpos, FP ypos, BaseAliveGameObject* pObj)
    {
        field_DC = gnFrameCount_507670 + Math_RandomRange_450F20(0, 16);
        field_B4_state = 1;
        field_E4_pObj = pObj;

        field_B0 = (field_C0_scale * field_E0_yMove - field_D0_ypos_mid) / FP_FromInteger(16);
        field_E8_xpos = xpos;
        field_EC_ypos = ypos;
    }

    int field_4_flags;
    Animation field_8_anim;
    FP field_A0;
    FP field_A4;
    FP field_A8_render_as_scale;
    FP field_AC;
    FP field_B0;
    __int16 field_B4_state;
    __int16 field_B6;
    int field_B8;
    __int16 field_BC;
    __int16 field_BE;
    FP field_C0_scale;
    FP field_C4_randomized_scale;
    FP field_C8;
    FP field_CC_xpos_mid;
    FP field_D0_ypos_mid;
    FP field_D4_radiusX;
    FP field_D8_radiosY;
    int field_DC;
    FP field_E0_yMove;
    BaseAliveGameObject* field_E4_pObj;
    FP field_E8_xpos;
    FP field_EC_ypos;
    FP field_F0;
    FP field_F4;
    FP field_F8;
    FP field_FC;
    FP field_100;
};
ALIVE_ASSERT_SIZEOF(OrbWhirlWindParticle, 0x104);

void OrbWhirlWind::ToSpin_48BAD0(FP xpos, FP ypos, BaseAliveGameObject* pObj)
{
    for (int i = 0; i < field_16_particleIdx; i++)
    {
        if (field_18_particles[i])
        {
            if (!field_18_particles[i]->IsActive())
            {
                field_18_particles[i]->Spin(xpos, ypos, pObj);
            }
        }
    }
    field_14 = 2;
}

void OrbWhirlWind::VRender_48BA90(int** ppOt)
{
    for (int i = 0; i < field_16_particleIdx; i++)
    {
        if (field_18_particles[i])
        {
            if (!field_18_particles[i]->IsActive())
            {
                field_18_particles[i]->VRender_48C430(ppOt);
            }
        }
    }
}

void OrbWhirlWind::VRender(int** ppOt)
{
    VRender_48BA90(ppOt);
}

void OrbWhirlWind::VUpdate_48B990()
{
    if (field_14)
    {
        if (field_14 == 2)
        {
            bool unknown = false;
            for (int i = 0; i < field_16_particleIdx; i++)
            {
                if (field_18_particles[i])
                {
                    if (!field_18_particles[i]->IsActive())
                    {
                        unknown = true;
                        break;
                    }
                }
            }

            if (!unknown)
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
        }
    }
    else
    {
        if (!(field_10 % 4))
        {
            auto pParticle = ao_new<OrbWhirlWindParticle>();
            if (pParticle)
            {
                pParticle->ctor_48BC10(
                    field_58_xpos_mid,
                    field_5C_ypos_mid,
                    field_60_scale);
            }
            field_18_particles[field_16_particleIdx++] = pParticle;

            if (field_16_particleIdx >= ALIVE_COUNTOF(field_18_particles))
            {
                field_14 = 1;
            }
        }
        ++field_10;
    }

    for (int i = 0; i < field_16_particleIdx; i++)
    {
        if (field_18_particles[i])
        {
            if (!field_18_particles[i]->IsActive())
            {
                field_18_particles[i]->VUpdate_48BF00();
            }
        }
    }
}

void OrbWhirlWind::VUpdate()
{
    VUpdate_48B990();
}

OrbWhirlWind* OrbWhirlWind::Vdtor_48C4F0(signed int flags)
{
    dtor_48B910();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* OrbWhirlWind::VDestructor(signed int flags)
{
    return Vdtor_48C4F0(flags);
}

BaseGameObject* OrbWhirlWind::dtor_48B910()
{
    SetVTable(this, 0x4BD7A0);
    gObjList_drawables_504618->Remove_Item(this);

    for (auto& obj : field_18_particles)
    {
        obj->Vdtor_48C510(1);
    }

    gOrbWhirlWind_instace_count_9F30A0--;
    return dtor_487DF0();
}

OrbWhirlWind* OrbWhirlWind::ctor_48B870(FP xpos, FP ypos, FP scale)
{
    ctor_487E10(1);

    SetVTable(this, 0x4BD7A0);

    field_4_typeId = Types::eNone_0;

    field_58_xpos_mid = xpos;
    field_5C_ypos_mid = ypos;
    field_60_scale = scale;

    field_14 = 0;
    field_10 = 0;

    gObjList_drawables_504618->Push_Back(this);
    field_6_flags.Set(Options::eDrawable_Bit4);

    field_16_particleIdx = 0;

    memset(field_18_particles, 0, sizeof(field_18_particles));

    gOrbWhirlWind_instace_count_9F30A0++;
    return this;
}

void OrbWhirlWind::ToStop_48BBB0()
{
    for (int i = 0; i < field_16_particleIdx; i++)
    {
        if (field_18_particles[i])
        {
            if (!field_18_particles[i]->IsActive())
            {
                field_18_particles[i]->ToStop();
            }
        }
    }
    field_14 = 2;
}

END_NS_AO

