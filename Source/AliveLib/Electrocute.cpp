#include "stdafx.h"
#include "Electrocute.hpp"
#include "BaseAliveGameObject.hpp"
#include "ObjectIds.hpp"
#include "stdlib.hpp"
#include "VRam.hpp"
#include "Game.hpp"
#include "Function.hpp"

// Overwrites a pallete 8 colours at a time one per update
class PalleteOverwriter : public BaseGameObject
{
public:
    EXPORT PalleteOverwriter* ctor_4228D0(PSX_Point palXY, __int16 palDepth, __int16 colour)
    {
        BaseGameObject_ctor_4DBFA0(FALSE, 0);

        SetVTable(this, 0x544BC4); // vTbl_Class_544BC4

        field_4_typeId = Types::eType_44;

        gObjList_drawables_5C1124->Push_Back(this);

        field_20_pal_xy = palXY;
        field_24_pal_colours_count = palDepth;

        field_6_flags.Set(BaseGameObject::eDrawable);

        for (auto& palBufferEntry : field_B8_palBuffer)
        {
            palBufferEntry = colour;
        }

        field_CA_pal_w = 8;
        field_C8_pal_x_index = 1;
        field_CC_bFirstUpdate = 1;
        field_CE_bDone = FALSE;

        return this;
    }

    virtual void VUpdate() override
    {
        vUpdate_422A70();
    }

    virtual void VRender(int** pOrderingTable) override
    {
        vRender_422B30(pOrderingTable);
    }

    virtual void VScreenChanged() override
    {
        // Stayin' alive
    }

private:
    EXPORT void vUpdate_422A70()
    {
        if (field_CC_bFirstUpdate || field_CE_bDone)
        {
            // First time round or when done do nothing
            field_CC_bFirstUpdate = FALSE;
        }
        else
        {
            if (field_C8_pal_x_index == field_24_pal_colours_count - 1)
            {
                // Got to the end
                field_CE_bDone = TRUE;
            }
            else
            {
                field_C8_pal_x_index += 8;

                if (field_C8_pal_x_index >= field_24_pal_colours_count - 1)
                {
                    field_C8_pal_x_index = field_24_pal_colours_count - 1;
                }

                if (field_C8_pal_x_index + field_CA_pal_w >= field_24_pal_colours_count - 1)
                {
                    field_CA_pal_w = field_24_pal_colours_count - field_C8_pal_x_index;
                }
            }
        }
    }

    EXPORT void dtor_4229F0()
    {
        SetVTable(this, 0x544BC4); // vTbl_Class_544BC4
        gObjList_drawables_5C1124->Remove_Item(this);
        BaseGameObject_dtor_4DBEC0();
    }

    EXPORT PalleteOverwriter* vdtor_4229C0(signed int flags)
    {
        dtor_4229F0();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
        return this;
    }

    EXPORT void vRender_422B30(int** /*pOt*/)
    {
        if (!field_CE_bDone)
        {
            PSX_RECT rect = {};
            rect.y = field_20_pal_xy.field_2_y;
            rect.x = field_20_pal_xy.field_0_x + field_C8_pal_x_index;
            rect.w = field_CA_pal_w;
            rect.h = 1;
            PSX_LoadImage16_4F5E20(&rect, reinterpret_cast<BYTE*>(&field_B8_palBuffer[0]));
        }
    }

private:
    PSX_Point field_20_pal_xy;
    short field_24_pal_colours_count;
    // pad
    int field_28_not_used[36]; // TODO: Probably something used in PSX but not PC?
    __int16 field_B8_palBuffer[8];
    __int16 field_C8_pal_x_index;
    __int16 field_CA_pal_w;
    __int16 field_CC_bFirstUpdate;
    __int16 field_CE_bDone;
};
ALIVE_ASSERT_SIZEOF(PalleteOverwriter, 0xD0);

BaseGameObject* Electrocute::VDestructor(signed int flags)
{
    return vdtor_4E6060(flags);
}

void Electrocute::VUpdate()
{
    vUpdate_4E6240();
}

void Electrocute::VScreenChanged()
{
    vScreenChanged_4E65E0();
}

void Electrocute::VSub_4E6150()
{
    vSub_4E6150();
}

int Electrocute::VSub_4E6630()
{
    return vSub_4E6630();
}

EXPORT Electrocute* Electrocute::ctor_4E5E80(BaseAliveGameObject* pTargetObj, __int16 b1, __int16 bKillTarget)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);

    SetVTable(this, 0x548100); // vTbl_Class_548100
    field_4_typeId = Types::eType_150;

    field_20_target_obj_id = pTargetObj->field_8_object_id;
    field_44_state = 0;
    field_3C_b1 = b1;
    field_2C_bKillTarget = bKillTarget;
    field_2E_count = b1 ? 3 : 2;
    field_40_pPalData = nullptr;

    switch (pTargetObj->field_4_typeId)
    {
    case Types::eFlyingSlig_54:
    case Types::eGlukkon_67:
    case Types::eType_Abe_69:
    case Types::eSlig_125:
        field_40_pPalData = reinterpret_cast<WORD*>(malloc_non_zero_4954F0(sizeof(WORD) * pTargetObj->field_20_animation.field_90_pal_depth));
        Pal_Copy_483560(
            pTargetObj->field_20_animation.field_8C_pal_vram_x,
            pTargetObj->field_20_animation.field_90_pal_depth,
            field_40_pPalData,
            &field_4C_pal_rect);
        break;
    default:
        break;
    }

    // Note: Real game may leave a ptr un-inited depending on the count
    // we just do them all because its easier and safer.
    for (auto& pPalOverwriter : field_30_pPalOverwriters)
    {
        pPalOverwriter = nullptr;
    }

    return this;
}

EXPORT Electrocute* Electrocute::vdtor_4E6060(signed int flags)
{
    dtor_4E6090();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

EXPORT void Electrocute::dtor_4E6090()
{
    SetVTable(this, 0x548100); // vTbl_Class_548100

    for (int i = 0; i < field_2E_count; i++)
    {
        if (field_30_pPalOverwriters[i])
        {
            field_30_pPalOverwriters[i]->VDestructor(1);
        }
    }

    field_20_target_obj_id = -1;

    if (field_40_pPalData)
    {
        Mem_Free_495560(field_40_pPalData);
    }

    // NOTE: omitted vtable vTbl_IClass_548128
    BaseGameObject_dtor_4DBEC0();
}

EXPORT void Electrocute::vScreenChanged_4E65E0()
{
    BaseAliveGameObject* pTargetObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_20_target_obj_id));
    // If the map has changed or target we are tracking has died then..
    if (gMap_5C3030.field_22 != gMap_5C3030.Get_Path_Unknown_480710() || (pTargetObj && pTargetObj->field_6_flags.Get(BaseGameObject::eDead)))
    {
        VSub_4E6150();
    }
}

EXPORT void Electrocute::vUpdate_4E6240()
{
    NOT_IMPLEMENTED();
    // standalone hack
    field_6_flags.Set(BaseGameObject::eDead);
}

EXPORT void Electrocute::vSub_4E6150()
{
    NOT_IMPLEMENTED();
}

EXPORT int Electrocute::vSub_4E6630()
{
    NOT_IMPLEMENTED();
    return 0;
}
