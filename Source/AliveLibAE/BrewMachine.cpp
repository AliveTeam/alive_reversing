#include "stdafx.h"
#include "BrewMachine.hpp"
#include "Events.hpp"
#include "Function.hpp"

// TODO: Should be const but can't be due to mlgs hacks in Font obj
u8 fontPalette_550F08[32] = {
    0u,
    0u,
    1u,
    128u,
    1u,
    132u,
    32u,
    132u,
    33u,
    128u,
    32u,
    132u,
    33u,
    132u,
    101u,
    206u,
    101u,
    140u,
    140u,
    177u,
    19u,
    148u,
    100u,
    206u,
    101u,
    206u,
    215u,
    152u,
    20u,
    161u,
    24u,
    216u};

BrewMachine::BrewMachine(Path_BrewMachine* pTlv, s32 tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(AETypes::eBrewMachine_13);

    field_F4_font_context.LoadFontType_433400(2);
    field_104_font.ctor_433590(3, fontPalette_550F08, &field_F4_font_context);

    const AnimRecord& rec = AnimRec(AnimId::BrewMachine_Button);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1u);

    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kEvilFartResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID);
    Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID);

    mApplyShadows &= ~1u;
    field_140_tlvInfo = tlvInfo;
    field_20_animation.mRenderLayer = Layer::eLayer_Well_23;
    field_1E4_remaining_brew_count = pTlv->field_10_brew_count;

    const u8 savedBrewCount = pTlv->field_1_tlv_state;
    if (savedBrewCount == 0)
    {
        field_144_total_brew_count = field_1E4_remaining_brew_count;
    }
    else if (savedBrewCount > 30)
    {
        field_144_total_brew_count = 0;
    }
    else
    {
        field_144_total_brew_count = savedBrewCount;
    }

    field_13C_textX = FP_GetExponent((FP_FromInteger(pTlv->field_8_top_left.field_0_x + 5) - pScreenManager->field_20_pCamPos->field_0_x));
    field_13E_textY = FP_GetExponent((FP_FromInteger(pTlv->field_8_top_left.field_2_y + 10) - pScreenManager->field_20_pCamPos->field_4_y));
    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_1E6_cam_id = gMap.mCurrentCamera;
}

BrewMachine::~BrewMachine()
{
    Path::TLV_Reset(field_140_tlvInfo, -1, 0, 0);
    field_104_font.dtor_433540();
    field_F4_font_context.dtor_433510();
}

void BrewMachine::VUpdate()
{
    Path_BrewMachine* pTlv = static_cast<Path_BrewMachine*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(field_140_tlvInfo));
    if (field_144_total_brew_count > 0)
    {
        pTlv->field_1_tlv_state = static_cast<u8>(field_144_total_brew_count);
    }
    else
    {
        pTlv->field_1_tlv_state = 50;
    }

    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void BrewMachine::VRender(PrimHeader** ppOt)
{
    if (gMap.mCurrentCamera == field_1E6_cam_id)
    {
        char_type text[12] = {};
        sprintf(text, "%02d", field_144_total_brew_count);
        const s32 textWidth = field_104_font.MeasureTextWidth(text);
        s16 flickerAmount = 50;
        if (sDisableFontFlicker_5C9304)
        {
            flickerAmount = 0;
        }

        field_104_font.DrawString_4337D0(
            ppOt,
            text,
            field_13C_textX,
            field_13E_textY,
            TPageAbr::eBlend_1,
            1,
            0,
            Layer::eLayer_BeforeWell_22,
            127,
            127,
            127,
            0,
            FP_FromInteger(1),
            field_13C_textX + textWidth,
            flickerAmount);

        const s32 v5 = 5 * textWidth;
        pScreenManager->InvalidateRect_40EC90(
            PsxToPCX(field_13C_textX),
            field_13E_textY,
            PsxToPCX(8 * v5),
            16,
            pScreenManager->field_3A_idx);
    }

    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
}
