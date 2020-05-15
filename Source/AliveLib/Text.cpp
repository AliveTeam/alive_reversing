#include "stdafx.h"
#include "Text.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "FixedPoint.hpp"
#include "Psx.hpp"
#include "PsxDisplay.hpp"
#include "PsxRender.hpp"
#include "Input.hpp"
#include "Sound/Midi.hpp"
#include "Sound/Sound.hpp"
#include "PauseMenu.hpp" // pal_554474
#include "Sys.hpp"

BaseGameObject* Text::VDestructor(signed int flags)
{
    return vdtor_46AED0(flags);
}

void Text::VUpdate()
{
    // Empty
}

void Text::VRender(int** pOrderingTable)
{
    Render_46AFD0(pOrderingTable);
}

void Text::VScreenChanged()
{
    // Empty
}

EXPORT Text* Text::ctor_46ADA0(const char* pMessage, int renderCount, int bShadow)
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    SetVTable(this, 0x546148); // vTbl_Text_546148

    field_6_flags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    field_6_flags.Set(BaseGameObject::eDrawable_Bit4);

    field_4_typeId = Types::eText_87;

    gObjList_drawables_5C1124->Push_Back(this);

    field_20_font.ctor_433590(static_cast<int>((bShadow + 1) * strlen(pMessage)), pal_554474, &sFont1Context_5BC5C8);

    field_5C_xpos = static_cast<short>(field_20_font.MeasureWidth_433700(pMessage));
    field_5E_ypos = 0;

    field_58_k0 = 0; // never used?
    field_5A_k4 = 4; // never used?

    memcpy(field_68_txt_buffer, pMessage, strlen(pMessage) + 1);

    field_60_bShadow = static_cast<short>(bShadow);
    field_64_render_count = renderCount;

    return this;
}

EXPORT BaseGameObject* Text::vdtor_46AED0(signed int flags)
{
    dtor_46AF00();
    if (flags & 1)
    {
        alive_delete_free(this);
    }
    return this;
}

EXPORT void Text::dtor_46AF00()
{
    SetVTable(this, 0x546148); // vTbl_Text_546148
    gObjList_drawables_5C1124->Remove_Item(this);
    field_6_flags.Clear(BaseGameObject::eDrawable_Bit4);
    field_20_font.dtor_433540();
    BaseGameObject_dtor_4DBEC0();
}

EXPORT void Text::SetYPos_46AFB0(int /*not_used*/, __int16 ypos)
{
    field_5E_ypos = ypos;
}

EXPORT void Text::Render_46AFD0(int** pOt)
{
    const __int16 ypos = field_5E_ypos + 100;
    const int xpos = (368 / 2) - (field_5C_xpos / 2);

    int drawRet1 = field_20_font.DrawString_4337D0(
        pOt,
        field_68_txt_buffer,
        xpos,
        ypos,
        0,
        1,
        0,
        42,
        210,
        150,
        80,
        0,
        FP_FromDouble(1.0),
        640,
        0);

    // Draw again with offsets on x/y to create a shadow effect
    if (field_60_bShadow)
    {
        int drawRet2 = field_20_font.DrawString_4337D0(
            pOt,
            field_68_txt_buffer,
            xpos + 2,
            ypos + 2,
            0,
            1,
            0,
            42,
            0,
            0,
            0,
            drawRet1,
            FP_FromDouble(1.0),
            640,
            0);

        field_20_font.DrawString_4337D0(
            pOt,
            field_68_txt_buffer,
            xpos - 1,
            ypos - 1,
            0,
            1,
            0,
            42,
            0,
            0,
            0,
            drawRet2,
            FP_FromDouble(1.0),
            640,
            0);
    }

    if (field_64_render_count > 0)
    {
        field_64_render_count--;
        if (field_64_render_count <= 0)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
}

// ResourceManager::vLoadFile_StateMachine_464A70 will call with type 0 (Displays Oddworld Abe's Exoddus.. but why?)
// Movie::vUpdate_4E0030 will call with type 1 which does nothing (trying to display movie skip message when it can't be found?)
// MainMenuController::tsub_LoadSave_4D1040 will call with type 2 (trying to display demo skip message when it can't be found?)
// MainMenuController::sub_4CF640 will call with type 3 (Shown on boot, says Abe's Exoddus)

// TODO: When above functions are reversed clean up this function to remove strange dead cases..
EXPORT char CC Display_Full_Screen_Message_Blocking_465820(int /*not_used*/, int messageType)
{
    if (messageType == 1)
    {
        return 0;
    }

    auto pTextObj = alive_new<Text>();
    if (pTextObj)
    {
        const char* pMsg = nullptr;
        if (messageType == 3)
        {
            pMsg = "    Abe's Exoddus    ";
        }
        else
        {
            // Therefore can only be type 2 or 0
            pMsg = "       Oddworld Abe's Exoddus        ";
        }
        pTextObj->ctor_46ADA0(pMsg, 1, 0);
    }

    Text* pTextObj2 = nullptr;
    switch (messageType)
    {
    case 0:
        // Never read ?
        //dword_55C128 = -1;
        break;

        // Dead due to early return ??
    case 1:
        pTextObj2 = alive_new<Text>();
        if (pTextObj2)
        {
            pTextObj2->ctor_46ADA0("or esc to skip the movie", 1, 0);
            pTextObj2->SetYPos_46AFB0(0, 30);
        }
        break;

    case 2:
        pTextObj2 = alive_new<Text>();
        if (pTextObj2)
        {
            pTextObj2->ctor_46ADA0("or esc to skip the demo", 1, 0);
            pTextObj2->SetYPos_46AFB0(0, 30);
        }
        break;
    }

    const PSX_RECT rect = { 0, 0, 640, 240 };
    PSX_ClearImage_4F5BD0(&rect, 0, 0, 0);
    SYS_EventsPump_494580();
    pTextObj->VRender(gPsxDisplay_5C1130.field_10_drawEnv[gPsxDisplay_5C1130.field_C_buffer_index].field_70_ot_buffer);

    if (pTextObj2)
    {
        pTextObj2->VRender(gPsxDisplay_5C1130.field_10_drawEnv[gPsxDisplay_5C1130.field_C_buffer_index].field_70_ot_buffer);
    }

    PSX_DrawSync_4F6280(0);
    Add_Dirty_Area_4ED970(0, 0, 640, 240);
    ResourceManager::Reclaim_Memory_49C470(500000);
    sbDisplayRenderFrame_55EF8C = 0;
    gPsxDisplay_5C1130.PSX_Display_Render_OT_41DDF0();

    if (SND_Seq_Table_Valid_4CAFE0())
    {
        SND_StopAll_4CB060();
    }

    DWORD displayForMsecs = SYS_GetTicks() + 1000;

    if (messageType == 3)
    {
        displayForMsecs += 4000;
    }

    char bQuitViaEnterOrTimeOut = 1;
    if (SYS_GetTicks() < displayForMsecs)
    {
        bool waitReturn = true;
        while (!Input_IsVKPressed_4EDD40(VK_RETURN))
        {
            // User quit
            if (messageType != 0 && Input_IsVKPressed_4EDD40(VK_ESCAPE))
            {
                bQuitViaEnterOrTimeOut = 0; // Nope, quitting via escape key

                // TODO: Never read?
                //dword_55C128 = sLevelId_dword_5CA408;

                // Wait for escape to come back up
                while (Input_IsVKPressed_4EDD40(VK_ESCAPE))
                {
                    SYS_EventsPump_494580();
                }
                waitReturn = false;
                break;
            }

            SYS_EventsPump_494580();

            if (SYS_GetTicks() >= displayForMsecs)
            {
                waitReturn = false;
                break;
            }
        }

        if (waitReturn)
        {
            // Wait for return to come back up, as we can only be here if it was pressed, didn't time out or escape wasn't pressed
            while (Input_IsVKPressed_4EDD40(VK_RETURN))
            {
                SYS_EventsPump_494580();
            }
        }
    }

    if (SND_Seq_Table_Valid_4CAFE0())
    {
        SND_Restart_4CB0E0();
    }

    SYS_EventsPump_494580();
    PSX_ClearImage_4F5BD0(&rect, 0, 0, 0);
    PSX_DrawSync_4F6280(0);
    Add_Dirty_Area_4ED970(0, 0, 640, 240);
    ResourceManager::Reclaim_Memory_49C470(500000);
    sbDisplayRenderFrame_55EF8C = 0;
    gPsxDisplay_5C1130.PSX_Display_Render_OT_41DDF0();
    gBaseGameObject_list_BB47C4->Remove_Item(pTextObj);

    if (pTextObj)
    {
        pTextObj->VDestructor(1);
    }

    if (pTextObj2)
    {
        gBaseGameObject_list_BB47C4->Remove_Item(pTextObj2);
        pTextObj2->VDestructor(1);
    }

    return bQuitViaEnterOrTimeOut;
}
