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

void Text::VUpdate()
{
    // Empty
}

void Text::VScreenChanged()
{
    // Empty
}

Text::Text(const char_type* pMessage, s32 renderCount, s32 bShadow)
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectFlags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);

    SetType(AETypes::eText_87);

    gObjList_drawables_5C1124->Push_Back(this);

    field_20_font.ctor_433590(static_cast<s32>((bShadow + 1) * strlen(pMessage)), pal_554474, &sFont1Context_5BC5C8);

    field_5C_xpos = static_cast<s16>(field_20_font.MeasureTextWidth(pMessage));
    field_5E_ypos = 0;

    field_58_k0_unused = 0; // never used?
    field_5A_k4_unused = 4; // never used?

    memcpy(field_68_txt_buffer, pMessage, strlen(pMessage) + 1);

    field_60_bShadow = static_cast<s16>(bShadow);
    field_64_render_count = renderCount;
}


Text::~Text()
{
    gObjList_drawables_5C1124->Remove_Item(this);
    mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
    field_20_font.dtor_433540();
}

void Text::SetYPos(s32 /*not_used*/, s16 ypos)
{
    field_5E_ypos = ypos;
}

void Text::VRender(PrimHeader** ppOt)
{
    const s16 ypos = field_5E_ypos + 100;
    const s32 xpos = (368 / 2) - (field_5C_xpos / 2);

    s32 drawRet1 = field_20_font.DrawString_4337D0(
        ppOt,
        field_68_txt_buffer,
        xpos,
        ypos,
        TPageAbr::eBlend_0,
        1,
        0,
        Layer::eLayer_Text_42,
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
        s32 drawRet2 = field_20_font.DrawString_4337D0(
            ppOt,
            field_68_txt_buffer,
            xpos + 2,
            ypos + 2,
            TPageAbr::eBlend_0,
            1,
            0,
            Layer::eLayer_Text_42,
            0,
            0,
            0,
            drawRet1,
            FP_FromDouble(1.0),
            640,
            0);

        field_20_font.DrawString_4337D0(
            ppOt,
            field_68_txt_buffer,
            xpos - 1,
            ypos - 1,
            TPageAbr::eBlend_0,
            1,
            0,
            Layer::eLayer_Text_42,
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
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
}

// ResourceManager::vLoadFile_StateMachine_464A70 will call with type 0 (Displays Oddworld Abe's Exoddus.. but why?).
// Movie::vUpdate_4E0030 will call with type 1 which does nothing (trying to display movie skip message when it can't be found?).
// MainMenuController::LoadDemo_Update_4D1040 will call with type 2 (trying to display demo skip message when it can't be found?).
// MainMenuController::ChangeScreenAndIntroLogic_4CF640 will call with type 3 (Shown on boot, says Abe's Exoddus).

// TODO: When above functions are reversed clean up this function to remove strange dead cases..
s8 Display_Full_Screen_Message_Blocking(s32 /*not_used*/, MessageType messageType)
{
    if (messageType == MessageType::eSkipMovie_1)
    {
        return 0;
    }

    auto pTextObj = ae_new<Text>(messageType == MessageType::eShortTitle_3 ? "    Abe's Exoddus    " : "       Oddworld Abe's Exoddus        ", 1, 0);

    Text* pTextObj2 = nullptr;
    switch (messageType)
    {
        case MessageType::eLongTitle_0:
            // Never read ?
            //dword_55C128 = -1;
            break;

        case MessageType::eShortTitle_3:
            // Do nothing
            break;

        // Dead due to early return ??
        case MessageType::eSkipMovie_1:
            pTextObj2 = ae_new<Text>("or esc to skip the movie", 1, 0);
            if (pTextObj2)
            {
                pTextObj2->SetYPos(0, 30);
            }
            break;

        case MessageType::eSkipDemo_2:
            pTextObj2 = ae_new<Text>("or esc to skip the demo", 1, 0);
            if (pTextObj2)
            {
                pTextObj2->SetYPos(0, 30);
            }
            break;
    }

    const PSX_RECT rect = {0, 0, 640, 240};
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

    if (SND_Seq_Table_Valid())
    {
        SND_StopAll();
    }

    u32 displayForMsecs = SYS_GetTicks() + 1000;

    if (messageType == MessageType::eShortTitle_3)
    {
        displayForMsecs += 4000;
    }

    s8 bQuitViaEnterOrTimeOut = 1;
    if (SYS_GetTicks() < displayForMsecs)
    {
        bool waitReturn = true;
        while (!Input_IsVKPressed_4EDD40(VK_RETURN))
        {
            // User quit
            if (messageType != MessageType::eLongTitle_0 && Input_IsVKPressed_4EDD40(VK_ESCAPE))
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

    if (SND_Seq_Table_Valid())
    {
        GetSoundAPI().SND_Restart();
    }

    SYS_EventsPump_494580();
    PSX_ClearImage_4F5BD0(&rect, 0, 0, 0);
    PSX_DrawSync_4F6280(0);
    Add_Dirty_Area_4ED970(0, 0, 640, 240);
    ResourceManager::Reclaim_Memory_49C470(500000);
    sbDisplayRenderFrame_55EF8C = 0;
    gPsxDisplay_5C1130.PSX_Display_Render_OT_41DDF0();

    if (pTextObj)
    {
        gBaseGameObjects->Remove_Item(pTextObj);
        delete pTextObj;
    }

    if (pTextObj2)
    {
        gBaseGameObjects->Remove_Item(pTextObj2);
        delete pTextObj2;
    }

    return bQuitViaEnterOrTimeOut;
}
