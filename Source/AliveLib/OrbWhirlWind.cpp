#include "stdafx.h"
#include "OrbWhirlWind.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "ScreenManager.hpp"
#include "BaseAliveGameObject.hpp"
#include "stdlib.hpp"

class Class_5480D4
{
public:
    EXPORT Class_5480D4* ctor_4E40C0(FP xpos, FP ypos, FP scale, __int16 a5)
    {
        SetVTable(&field_8_Anim, 0x544290);

        SetVTable(this, 0x5480D4);

        BYTE** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 312, TRUE, FALSE);
        field_108_res = ppRes;

        field_8_Anim.Init_40A030(1632, gObjList_animations_5C1A24, 0, 39, 21, ppRes, 1, 0, 0);

        field_B6 = a5; // Never used?

        field_8_Anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

        field_8_Anim.field_C_render_layer = 32;
        field_8_Anim.field_B_render_mode = 1;
        if (a5 == 1)
        {
            field_8_Anim.field_8_r = 0;
            field_8_Anim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
            field_8_Anim.field_9_g = 255;
            field_8_Anim.field_A_b = 32;
        }
        else
        {
            field_8_Anim.field_4_flags.Set(AnimFlags::eBit16_bBlending);
            field_8_Anim.field_8_r = 80;
            field_8_Anim.field_9_g = 80;
            field_8_Anim.field_A_b = 80;
        }

        field_8_Anim.SetFrame_409D50(Math_RandomRange_496AB0(0, field_8_Anim.Get_Frame_Count_40AC70() - 1));
        SetActive_4E4340(0);
        field_B4_state = State::State_0_Start;
        field_B8 = Math_RandomRange_496AB0(0, 255);
        field_BC = 1;
        field_BE = Math_RandomRange_496AB0(9, 15);
        field_CC_xpos_mid = xpos;
        field_D0_ypos_mid = ypos + FP_FromInteger(Math_RandomRange_496AB0(-12, 12));
        field_E0_yMove = ypos - FP_FromInteger(16);
        field_D4_radiusX = FP_FromInteger(Math_RandomRange_496AB0(37, 43));
        field_D8_radiusY = FP_FromDouble(0.25) * field_D4_radiusX;
        field_C0_current_scale = scale;
        field_C4_randomized_scale = FP_FromInteger(Math_RandomRange_496AB0(7, 10)) / FP_FromInteger(10);
        field_A8_render_as_scale = (field_C0_current_scale * field_C4_randomized_scale);
        return this;
    }

    EXPORT int IsActive_4E4370()
    {
        return field_4_flags & 1;
    }

    EXPORT void Spin_4E4A10(FP xpos, FP ypos, FP scale, BaseGameObject* pObj)
    {
        field_DC = sGnFrame_5C1B84 + Math_RandomRange_496AB0(0, 16);
        field_B4_state = State::State_1_Spin;
        field_E4_pObj = pObj;
        field_B0 = (field_C0_current_scale * (field_E0_yMove - field_D0_ypos_mid)) / FP_FromInteger(16);
        field_EC_ypos = ypos;
        field_E8_xpos = xpos;
        field_F0_scale = scale;
    }

    EXPORT void ToStop_4E4AD0()
    {
        field_B4_state = State::State_4_Stop;
        field_DC = sGnFrame_5C1B84 + Math_RandomRange_496AB0(0, 32);
    }
    
    // WARNING: Not a derived type - keep virtuals in order for BC
    EXPORT virtual Class_5480D4* vdtor_4E42D0(char flags)
    {
        dtor_4E4300();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
        return this;
    }

    EXPORT virtual void VUpdate_4E4510()
    {
        switch (field_B4_state)
        {
        case State::State_0_Start:
            sub_4E4390(1);
            break;

        case State::State_1_Spin:
            if (static_cast<int>(sGnFrame_5C1B84) < field_DC + 16)
            {
                if (static_cast<int>(sGnFrame_5C1B84) >= field_DC)
                {
                    field_D0_ypos_mid += field_B0;
                    field_D4_radiusX -= FP_FromInteger(2);
                    field_D8_radiusY -= FP_FromInteger(1);
                }
                sub_4E4390(1);
            }
            else
            {
                if (field_E4_pObj && field_E4_pObj->field_6_flags.Get(BaseGameObject::eDead))
                {
                    ToStop_4E4AD0();
                }
                else
                {
                    field_FC = field_CC_xpos_mid;
                    field_F4 = field_CC_xpos_mid;
                    field_100 = field_D0_ypos_mid;
                    field_F8 = field_D0_ypos_mid;
                    field_C8 = (field_F0_scale - field_C0_current_scale) / FP_FromInteger(16);
                    field_DC = sGnFrame_5C1B84 + 16;
                    field_B4_state = State::State_2_FlyToTarget;
                    sub_4E4390(1);
                }
            }
            break;

        case State::State_2_FlyToTarget:
            if (field_E4_pObj && field_E4_pObj->field_6_flags.Get(BaseGameObject::eDead))
            {
                ToStop_4E4AD0();
            }
            else
            {
                FP v11 = {};
                FP v12 = {};
                if (field_E4_pObj)
                {
                    PSX_RECT bRect = {};
                    static_cast<BaseAliveGameObject*>(field_E4_pObj)->vGetBoundingRect_424FD0(&bRect, 1);

                    v11 = FP_FromInteger((bRect.x + bRect.w) / 2);
                    v12 = FP_FromInteger((bRect.y + bRect.h) / 2);
                }
                else
                {
                    v11 = field_E8_xpos;
                    v12 = field_EC_ypos;
                }

                if (static_cast<int>(sGnFrame_5C1B84) < field_DC)
                {
                    field_C0_current_scale += field_C8;
                    const FP v16 = FP_FromInteger(0xFFFF * field_DC + sGnFrame_5C1B84 + 16) / FP_FromInteger(16); // TODO : purpose of 0xFFFF * ??
                    field_FC =  ((v11 - field_F4) * v16) + field_F4;
                    field_100 = ((v12 - field_F8) * v16) + field_F8;
                    field_CC_xpos_mid = (FP_FromInteger(32) * field_C0_current_scale) *   Math_Sine_496DF0(FP_FromInteger(128) * v16) + field_FC;
                    field_D0_ypos_mid = (FP_FromInteger(32) * field_C0_current_scale) * Math_Cosine_496D60(FP_FromInteger(128) * v16) + field_100;
                }
                else
                {
                    field_FC = v11;
                    field_100 = v12;
                    field_CC_xpos_mid = v11;
                    field_D0_ypos_mid = v12;
                    field_B8 = 192;
                    field_D4_radiusX = FP_FromInteger(40);
                    field_AC = field_D4_radiusX / FP_FromInteger(32);
                    field_104 = field_C0_current_scale * FP_FromInteger(Math_RandomRange_496AB0(-16, 16));
                    field_DC = sGnFrame_5C1B84 + 32;
                    field_B4_state = State::State_3_SpinAtTarget;
                }
                sub_4E4390(1);
            }
            break;

        case State::State_3_SpinAtTarget:
            if (static_cast<int>(sGnFrame_5C1B84) >= field_DC)
            {
                SetActive_4E4340(1);
            }

            field_D0_ypos_mid = (field_104 *  Math_Cosine_496D60((FP_FromInteger(128) * FP_FromInteger(0xFFFF * field_DC + sGnFrame_5C1B84 + 32) / FP_FromInteger(32)))) + field_100; // TODO : purpose of 0xFFFF * ??
            field_D4_radiusX = field_D4_radiusX - field_AC;
            sub_4E4390(1);
            break;

        case State::State_4_Stop:
            if (static_cast<int>(sGnFrame_5C1B84) >= field_DC)
            {
                SetActive_4E4340(1);
            }
            sub_4E4390(0);
            break;

        default:
            return;
        }
    }

    EXPORT virtual void VRender_4E4B10(int** ppOt)
    {
        const FP x = std::min(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x,
                         pScreenManager_5BB5F4->field_20_pCamPos->field_0_x + FP_FromInteger(367));

        const FP w = std::max(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x,
                         pScreenManager_5BB5F4->field_20_pCamPos->field_0_x + FP_FromInteger(367));

        const FP y = std::min(pScreenManager_5BB5F4->field_20_pCamPos->field_4_y,
                          pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + FP_FromInteger(239));
          
        const FP h = std::max(pScreenManager_5BB5F4->field_20_pCamPos->field_4_y,
                         pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + FP_FromInteger(239));

        if (field_A0 >= x && field_A0 <= w)
        {
            if (field_A4 + FP_FromInteger(5) >= y && field_A4 + FP_FromInteger(5) <= h)
            {
                field_8_Anim.field_14_scale = field_A8_render_as_scale;
                const FP xpos = field_A0 - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x;
                const FP ypos = field_A4 - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + FP_FromInteger(5);

                field_8_Anim.vRender_40B820(
                    FP_GetExponent(xpos),
                    FP_GetExponent(ypos),
                    ppOt,
                    0,
                    0);

                PSX_RECT r = {};
                field_8_Anim.Get_Frame_Rect_409E10(&r);
                pScreenManager_5BB5F4->InvalidateRect_40EC90(r.x, r.y, r.w, r.h, pScreenManager_5BB5F4->field_3A_idx);
            }
        }
    }

private:
    EXPORT void dtor_4E4300()
    {
        field_8_Anim.vCleanUp_40C630();
        ResourceManager::FreeResource_49C330(field_108_res);
    }

    EXPORT void sub_4E4390(__int16 a2)
    {
        field_B8 += field_BC;

        if (a2)
        {
            if (field_BC <= field_BE && !(static_cast<int>(sGnFrame_5C1B84) % 3))
            {
                field_BC++;
            }
        }
        else if (field_BC >= 1 && !(static_cast<int>(sGnFrame_5C1B84) % 3))
        {
            field_BC--;
            field_D4_radiusX = field_D4_radiusX + FP_FromInteger(4);
        }

        field_A0 = ((field_C0_current_scale * field_D4_radiusX) * Math_Sine_496DD0(static_cast<BYTE>(field_B8))) + field_CC_xpos_mid;
        field_A4 = ((field_C0_current_scale * field_D8_radiusY) * Math_Cosine_496CD0(static_cast<BYTE>(field_B8))) + field_D0_ypos_mid;
        field_A8_render_as_scale = field_C0_current_scale * field_C4_randomized_scale;
        if (field_C0_current_scale > FP_FromDouble(0.599)) // TODO: Check VS 39321
        {
            field_8_Anim.field_C_render_layer = 32;
        }
        else
        {
            field_8_Anim.field_C_render_layer = 13;
        }
    }

    EXPORT void SetActive_4E4340(unsigned __int8 active)
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


private:
    __int16 field_4_flags;
    __int16 field_6;
    Animation field_8_Anim;
    FP field_A0;
    FP field_A4;
    FP field_A8_render_as_scale;
    FP field_AC;
    FP field_B0;

    enum class State : __int16
    {
        State_0_Start = 0,
        State_1_Spin = 1,
        State_2_FlyToTarget = 2,
        State_3_SpinAtTarget = 3,
        State_4_Stop = 4,
    };

    State field_B4_state;

    __int16 field_B6;
    int field_B8;
    __int16 field_BC;
    __int16 field_BE;
    FP field_C0_current_scale;
    FP field_C4_randomized_scale;
    FP field_C8;
    FP field_CC_xpos_mid;
    FP field_D0_ypos_mid;
    FP field_D4_radiusX;
    FP field_D8_radiusY;
    int field_DC;
    FP field_E0_yMove;
    BaseGameObject* field_E4_pObj;
    FP field_E8_xpos;
    FP field_EC_ypos;
    FP field_F0_scale;
    FP field_F4;
    FP field_F8;
    FP field_FC;
    FP field_100;
    FP field_104;
    BYTE** field_108_res;
};
ALIVE_ASSERT_SIZEOF(Class_5480D4, 0x10C);

BaseGameObject* OrbWhirlWind::VDestructor(signed int flags)
{
    return vdtor_4E3D50(flags);
}

void OrbWhirlWind::VUpdate()
{
    return vUpdate_4E3E20();
}

void OrbWhirlWind::VRender(int** pOrderingTable)
{
    return vRender_4E3F80(pOrderingTable);
}

OrbWhirlWind* OrbWhirlWind::ctor_4E3C90(FP xpos, FP ypos, FP scale, __int16 bUnknown)
{
    field_4_typeId = Types::eNone_0;

    BaseGameObject_ctor_4DBFA0(TRUE, 0);

    SetVTable(this, 0x5480B8); // vTbl_OrbWhirlWind_5480B8


    field_6C_xpos = xpos;
    field_70_ypos = ypos;
    field_74_scale = scale;

    field_24 = 0;
    field_20 = 0;

    field_26_bUnknown = bUnknown;
    gObjList_drawables_5C1124->Push_Back(this);
    field_6_flags.Set(BaseGameObject::eDrawable);

    field_28_obj_array_idx = 0;
    memset(field_2C_objArray, 0, sizeof(field_2C_objArray));

    return this;
}

void OrbWhirlWind::ToSpin_4E3FD0(FP xpos, FP ypos, FP scale, BaseGameObject* pObj)
{
    for (int i = 0; i < field_28_obj_array_idx; i++)
    {
        if (field_2C_objArray[i])
        {
            if (!field_2C_objArray[i]->IsActive_4E4370())
            {
                field_2C_objArray[i]->Spin_4E4A10(xpos, ypos, scale, pObj);
            }
        }
    }
    field_24 = 2;
}

void OrbWhirlWind::ToStop_4E4050()
{
    for (int i = 0; i < field_28_obj_array_idx; i++)
    {
        if (field_2C_objArray[i])
        {
            if (!field_2C_objArray[i]->IsActive_4E4370())
            {
                field_2C_objArray[i]->ToStop_4E4AD0();
            }
        }
    }
    field_24 = 2;
}

void OrbWhirlWind::vRender_4E3F80(int** ppOt)
{
    for (int i = 0; i < field_28_obj_array_idx; i++)
    {
        if (field_2C_objArray[i])
        {
            if (!field_2C_objArray[i]->IsActive_4E4370())
            {
                field_2C_objArray[i]->VRender_4E4B10(ppOt);
            }
        }
    }
}

void OrbWhirlWind::vUpdate_4E3E20()
{
    if (field_24)
    {
        if (field_24 == 2)
        {
            bool unknown = false;
            for (int i = 0; i < field_28_obj_array_idx; i++)
            {
                if (field_2C_objArray[i])
                {
                    if (!field_2C_objArray[i]->IsActive_4E4370())
                    {
                        unknown = true;
                        break;
                    }
                }
            }

            if (!unknown)
            {
                field_6_flags.Set(BaseGameObject::eDead);
            }
        }
    }
    else
    {
        if (!(field_20 % 4))
        {
            auto pObj = alive_new<Class_5480D4>();
            if (pObj)
            {
                pObj->ctor_4E40C0(
                    field_6C_xpos,
                    field_70_ypos,
                    field_74_scale,
                    field_26_bUnknown);
            }

            field_2C_objArray[field_28_obj_array_idx++] = pObj;

            if (field_28_obj_array_idx >= ALIVE_COUNTOF(field_2C_objArray))
            {
                field_24 = 1;
            }
        }

        ++field_20;
    }

    for (int i = 0; i < field_28_obj_array_idx; i++)
    {
        if (field_2C_objArray[i])
        {
            if (!field_2C_objArray[i]->IsActive_4E4370())
            {
                field_2C_objArray[i]->VUpdate_4E4510();
            }
        }
    }
}

void OrbWhirlWind::dtor_4E3D80()
{
    SetVTable(this, 0x5480B8);

    gObjList_drawables_5C1124->Remove_Item(this);
    for (auto& obj : field_2C_objArray)
    {
        if (obj)
        {
            obj->vdtor_4E42D0(1);
        }
    }
    BaseGameObject_dtor_4DBEC0();
}


OrbWhirlWind* OrbWhirlWind::vdtor_4E3D50(signed int flags)
{
    dtor_4E3D80();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}
