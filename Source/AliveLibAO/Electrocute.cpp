#include "stdafx_ao.h"
#include "Function.hpp"
#include "Electrocute.hpp"
#include "Psx.hpp"
#include "BaseGameObject.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

START_NS_AO

class PalleteOverwriter : public BaseGameObject
{
public:
    EXPORT PalleteOverwriter* ctor_416FF0(PSX_Point palXY, __int16 palDepth, __int16 colour)
    {
        ctor_487E10(1);

        SetVTable(this, 0x4BA9E8);

        field_4_typeId = Types::ePalOverwriter_29;

        gObjList_drawables_504618->Push_Back(this);

        field_10_pal_xy = palXY;
        field_14_pal_colours_count = palDepth;

        field_6_flags.Set(Options::eDrawable_Bit4);
        
        for (auto palBufferEntry : field_A8_palBuffer)
        {
            palBufferEntry = colour;
        }

        field_BA_pal_w = 8;
        field_B8_pal_x_index = 1;
        field_BC_bFirstUpdate = 1;
        field_BE_bDone = FALSE;

        return this;
    }

    EXPORT BaseGameObject* dtor_4170B0()
    {
        SetVTable(this, 0x4BA9E8);
        gObjList_drawables_504618->Remove_Item(this);
        return dtor_487DF0();
    }

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return Vdtor_417200(flags);
    }

    EXPORT PalleteOverwriter* Vdtor_417200(signed int flags)
    {
        dtor_4170B0();
        if (flags & 1)
        {
            ao_delete_free_447540(this);
        }
        return this;
    }

    virtual void VScreenChanged() override
    {
        // Stayin' alive
    }

    virtual void VRender(int** ppOt) override
    {
        VRender_4171A0(ppOt);
    }

    EXPORT void VRender_4171A0(int** /*ppOt*/)
    {
        if (!field_BE_bDone)
        {
            PSX_RECT rect = {};
            rect.y = field_10_pal_xy.field_2_y;
            rect.x = field_10_pal_xy.field_0_x + field_B8_pal_x_index;
            rect.w = field_BA_pal_w;
            rect.h = 1;
            PSX_LoadImage16_4962A0(&rect, reinterpret_cast<BYTE*>(field_A8_palBuffer));
        }
    }

    virtual void VUpdate() override
    {
        VUpdate_417110();
    }

    EXPORT void VUpdate_417110()
    {
        if (field_BC_bFirstUpdate || field_BE_bDone)
        {
            // First time round or when done do nothing
            field_BC_bFirstUpdate = FALSE;
        }
        else
        {
            if (field_B8_pal_x_index == field_14_pal_colours_count - 1)
            {
                // Got to the end
                field_BE_bDone = TRUE;
            }
            else
            {
                field_B8_pal_x_index += 8;

                if (field_B8_pal_x_index >= field_14_pal_colours_count - 1)
                {
                    field_B8_pal_x_index = field_14_pal_colours_count - 1;
                }

                if (field_BA_pal_w + field_B8_pal_x_index >= field_14_pal_colours_count - 1)
                {
                    field_BA_pal_w = field_14_pal_colours_count - field_B8_pal_x_index;
                }
            }
        }
    }

    PSX_Point field_10_pal_xy;
    short field_14_pal_colours_count;
    // pad
    int field_18_not_used[36]; // TODO: Probably something used in PSX but not PC?
    __int16 field_A8_palBuffer[8];
    __int16 field_B8_pal_x_index;
    __int16 field_BA_pal_w;
    __int16 field_BC_bFirstUpdate;
    __int16 field_BE_bDone;
};
ALIVE_ASSERT_SIZEOF(PalleteOverwriter, 0xC0);

END_NS_AO

