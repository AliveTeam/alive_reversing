#include "stdafx.h"
#include "OrbWhirlWind.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "ScreenManager.hpp"
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

        field_B6 = a5;

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
        field_4 &= ~1u;
        field_B4 = 0;
        field_B8 = Math_RandomRange_496AB0(0, 255);
        field_BC = 1;
        field_BE = Math_RandomRange_496AB0(9, 15);
        field_CC = xpos;
        field_D0 = ypos + FP_FromInteger(Math_RandomRange_496AB0(-12, 12));
        field_E0 = ypos - FP_FromInteger(16);
        field_D4 = FP_FromInteger(Math_RandomRange_496AB0(37, 43));
        field_D8 = FP_FromDouble(0.25) * field_D4;
        field_C0 = scale;
        field_C4 = FP_FromInteger(Math_RandomRange_496AB0(7, 10)) / FP_FromInteger(10);
        field_A8 = (field_C0 * field_C4);
        return this;
    }

    EXPORT int sub_4E4370()
    {
        return field_4 & 1;
    }

    EXPORT void sub_4E4A10(FP xpos, FP ypos, FP scale, BaseGameObject* pObj)
    {
        field_DC = sGnFrame_5C1B84 + Math_RandomRange_496AB0(0, 16);
        field_B4 = 1;
        field_E4_pObj = pObj;
        field_B0 = (field_C0 * (field_E0 - field_D0)) / FP_FromInteger(16);
        field_EC_ypos = ypos;
        field_E8_xpos = xpos;
        field_F0_scale = scale;
    }

    EXPORT void sub_4E4AD0()
    {
        field_B4 = 4;
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

    EXPORT virtual void vSub_4E4510()
    {
        NOT_IMPLEMENTED();
    }

    EXPORT virtual void vSub_4E4B10(int** ppOt)
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
                field_8_Anim.field_14_scale = field_A8;
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
            field_D4 = field_D4 + FP_FromInteger(4);
        }

        field_A0 = ((field_C0 * field_D4) * Math_Sine_496DD0(static_cast<BYTE>(field_B8))) + field_CC;
        field_A4 = ((field_C0 * field_D8) * Math_Cosine_496CD0(static_cast<BYTE>(field_B8))) + field_D0;
        field_A8 = field_C0 * field_C4;
        if (field_C0 > FP_FromDouble(0.599)) // TODO: Check VS 39321
        {
            field_8_Anim.field_C_render_layer = 32;
        }
        else
        {
            field_8_Anim.field_C_render_layer = 13;
        }
    }

private:
    __int16 field_4;
    __int16 field_6;
    Animation field_8_Anim;
    FP field_A0;
    FP field_A4;
    FP field_A8;
    int field_AC;
    FP field_B0;
    __int16 field_B4;
    __int16 field_B6;
    int field_B8;
    __int16 field_BC;
    __int16 field_BE;
    FP field_C0;
    FP field_C4;
    int field_C8;
    FP field_CC;
    FP field_D0;
    FP field_D4;
    FP field_D8;
    int field_DC;
    FP field_E0;
    BaseGameObject* field_E4_pObj;
    FP field_E8_xpos;
    FP field_EC_ypos;
    FP field_F0_scale;
    int field_F4;
    int field_F8;
    int field_FC;
    int field_100;
    int field_104;
    BYTE** field_108_res;
};
ALIVE_ASSERT_SIZEOF(Class_5480D4, 0x10C);

BaseGameObject* OrbWhirlWind::VDestructor(signed int flags)
{
    return vdtor_4E3D50(flags);
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

void OrbWhirlWind::sub_4E3FD0(FP xpos, FP ypos, FP scale, BaseGameObject* pObj)
{
    for (int i = 0; i < field_28_obj_array_idx; i++)
    {
        if (field_2C_objArray[i])
        {
            if (!field_2C_objArray[i]->sub_4E4370())
            {
                field_2C_objArray[i]->sub_4E4A10(xpos, ypos, scale, pObj);
            }
        }
    }
    field_24 = 2;
}

void OrbWhirlWind::sub_4E4050()
{
    for (int i = 0; i < field_28_obj_array_idx; i++)
    {
        if (field_2C_objArray[i])
        {
            if (!field_2C_objArray[i]->sub_4E4370())
            {
                field_2C_objArray[i]->sub_4E4AD0();
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
            if (!field_2C_objArray[i]->sub_4E4370())
            {
                field_2C_objArray[i]->vSub_4E4B10(ppOt);
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
