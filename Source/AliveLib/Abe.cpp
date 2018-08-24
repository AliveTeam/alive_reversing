#include "stdafx.h"
#include "Abe.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Midi.hpp"
#include "Sfx.hpp"
#include "DebugHelpers.hpp"

SfxDefinition sAbeSFXList_555250[41] =
{
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 24u, 60u, 115u, 0, 0 },
    { 0u, 24u, 61u, 110u, 0, 0 },
    { 0u, 24u, 62u, 127u, 0, 0 },
    { 0u, 24u, 63u, 90u, 0, 0 },
    { 0u, 24u, 66u, 127u, 0, 0 },
    { 0u, 24u, 67u, 100u, 0, 0 },
    { 0u, 24u, 56u, 127u, 0, 0 },
    { 0u, 24u, 57u, 100u, 0, 0 },
    { 0u, 23u, 48u, 127u, 0, 0 },
    { 0u, 24u, 59u, 127u, 0, 0 },
    { 0u, 24u, 58u, 127u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 24u, 65u, 127u, 0, 0 },
    { 0u, 24u, 64u, 80u, 0, 0 },
    { 0u, 23u, 60u, 90u, 0, 0 },
    { 0u, 23u, 68u, 127u, 0, 0 },
    { 0u, 23u, 69u, 127u, 0, 0 },
    { 0u, 23u, 70u, 127u, 0, 0 },
    { 0u, 23u, 72u, 127u, 0, 0 },
    { 0u, 23u, 73u, 127u, 0, 0 },
    { 0u, 23u, 58u, 127u, 0, 0 },
    { 0u, 23u, 64u, 127u, 0, 0 },
    { 0u, 23u, 66u, 115u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 23u, 63u, 115u, 0, 0 },
    { 0u, 23u, 62u, 90u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 0, 0 }
};

int sAbeFrameOffsetTable_554B18[130] =
{
    58888, 58808, 59064, 58888, 58788, 58748, 58768, 59028, 58992, 58956, 58920, 
    59112, 270240, 269976, 270024, 269928, 269928, 270092, 270060, 269876, 271080, 
    271120, 269688, 269708, 269764, 270860, 270328, 271004, 271056, 270964, 
    270468, 269804, 270488, 270748, 270180, 270616, 270180, 270428, 270708, 270252, 
    270508, 270688, 270596, 270840, 270728, 270288, 270160, 270308, 270120, 270268, 
    270668, 270820, 270408, 270140, 269784, 269856, 270092, 270092, 270092, 270092, 
    270092, 270944, 271152, 271192, 271248, 57440, 57580, 57540, 57540, 57712, 
    57680, 49740, 49800, 49912, 49856, 20572, 20552, 20552, 20404, 20552, 20552, 20404, 
    20552, 20552, 8104, 5724, 16096, 16040, 16220, 16096, 16040, 32012, 32012, 32040, 
    32068, 32112, 32168, 32152, 32132, 11396, 6520, 17240, 17180, 58888, 31504, 31392, 
    31632, 31576, 31432, 24580, 24628, 6004, 9992, 10040, 19040, 19088, 8540, 8540, 8588, 
    8732, 8772, 22548, 22572, 22464, 22596, 22496, 11856, 11816, 11888, 28824
};

TintEntry sTintTable_Abe_554D20[15] =
{
    { 1, 102u, 102u, 102u },
    { 2, 102u, 102u, 80u },
    { 3, 120u, 90u, 120u },
    { 4, 102u, 70u, 90u },
    { 5, 120u, 102u, 82u },
    { 6, 102u, 102u, 102u },
    { 7, 102u, 70u, 90u },
    { 8, 102u, 102u, 102u },
    { 9, 102u, 102u, 102u },
    { 10, 102u, 102u, 102u },
    { 11, 120u, 90u, 120u },
    { 12, 120u, 102u, 82u },
    { 13, 102u, 102u, 102u },
    { 14, 120u, 90u, 80u },
    { -1, 102u, 102u, 102u }
};



signed int CC Abe::CreateFromSaveState_44D4F0(char * /*a1*/)
{
    NOT_IMPLEMENTED();
    return 216;
}

void Abe::VDestructor(signed int flags)
{
    dtor_44B350(flags);
}

void Abe::dtor_44B350(signed int /*flags*/)
{
    NOT_IMPLEMENTED();
}

void Abe::Knockback_44E700(__int16 /*a2*/, __int16 /*a3*/)
{
    NOT_IMPLEMENTED();
}

bool Abe::vsub_Kill_44BB50(BaseGameObject * /*otherObj*/)
{
    NOT_IMPLEMENTED();
    return false;
}

int Abe::sub_44B7B0()
{
#ifdef STUPID_FUN
    // THIS IS A HACK TO MAKE ABE POSSESS ANYTHING :D
    for (int baseObjIdx = 0; baseObjIdx < gBaseGameObject_list_BB47C4->Size(); baseObjIdx++)
    {
        BaseAliveGameObject* pBaseGameObject = reinterpret_cast<BaseAliveGameObject*>(gBaseGameObject_list_BB47C4->ItemAt(baseObjIdx));
        if (!pBaseGameObject)
        {
            break;
        }

        if (pBaseGameObject->field_4_typeId == 83)
        {
            pBaseGameObject->field_6_flags |= 4;
        }

        PSX_Point currentScreenCoords;
        gMap_5C3030.GetCurrentCamCoords_480680(&currentScreenCoords);
        if (pBaseGameObject != sActiveHero_5C1B68 && pBaseGameObject->field_6_flags & BaseGameObject::eIsBaseAliveGameObject &&
            pBaseGameObject->field_B8_xpos.GetExponent() > currentScreenCoords.field_0_x && pBaseGameObject->field_B8_xpos.GetExponent() < currentScreenCoords.field_0_x + 350
            && pBaseGameObject->field_BC_ypos.GetExponent() > currentScreenCoords.field_2_y && pBaseGameObject->field_BC_ypos.GetExponent() < currentScreenCoords.field_2_y + 240)
        {
            return (int)pBaseGameObject;
        }
    }
    return 0;
#else
    NOT_IMPLEMENTED();
#endif
}

void Abe::Load_Basic_Resources_44D460()
{
    NOT_IMPLEMENTED();
}

void Abe::Free_Resources_44D420()
{
    NOT_IMPLEMENTED();
}

BYTE ** Abe::StateToAnimResource_44AAB0(signed int state)
{
    NOT_IMPLEMENTED();
}

// TODO: Clean up
EXPORT void CC Abe_SFX_457EC0(unsigned __int8 idx, __int16 volume, int pitch, Abe *pHero)
{
    //DEV_CONSOLE_PRINTF("Abe SFX: %i", idx);

    switch (idx)
    {
    case 8u:
        if (pHero == sActiveHero_5C1B68 && gMap_5C3030.sCurrentLevelId_5C3030 == 10)
        {
            idx = 10;
        }
        goto LABEL_7;
    case 14u:
        if (pHero && pHero->field_CC_sprite_scale.GetDouble() == 0.5)
        {
            SND_SEQ_Play_4CAB10(0x10u, 1, 90, 90);
        }
        else
        {
            SND_SEQ_Play_4CAB10(0x10u, 1, 127, 127);
        }
        break;
    case 26u:
        if (pHero && pHero->field_CC_sprite_scale.GetDouble() == 0.5)
        {
            SND_SEQ_Play_4CAB10(0x12u, 1, 80, 80);
        }
        else
        {
            SND_SEQ_Play_4CAB10(0x12u, 1, 115, 115);
        }
        break;
    default:
    LABEL_7:
        if (!volume)
        {
            volume = (char)sAbeSFXList_555250[idx].field_3_default_volume;
        }
        if (!pHero)
        {
            goto LABEL_30;
        }
        if (pHero->field_CC_sprite_scale.GetDouble() == 0.5)
        {
            volume = 2 * volume / 3;
        }
        if (pHero != sActiveHero_5C1B68)
        {
            switch (gMap_5C3030.sub_4811A0(
                pHero->field_C2_lvl_number,
                pHero->field_C0_path_number,
                pHero->field_B8_xpos,
                pHero->field_BC_ypos))
            {
            case 0:
                SFX_SfxDefinition_Play_4CA420(&sAbeSFXList_555250[idx], volume, pitch, pitch);
                break;
            case 1:
            case 2:
                SFX_SfxDefinition_Play_4CA420(&sAbeSFXList_555250[idx], 2 * volume / 3, pitch, pitch);
                break;
            case 3:
                SFX_SfxDefinition_Play_4CA700(
                    &sAbeSFXList_555250[idx],
                    ((unsigned int)((unsigned __int64)(2863311532i64 * (signed __int16)volume) >> 32) >> 31)
                    + ((unsigned __int64)(2863311532i64 * (signed __int16)volume) >> 32),
                    2 * (signed __int16)volume / 9,
                    pitch,
                    pitch);
                break;
            case 4:
                SFX_SfxDefinition_Play_4CA700(
                    &sAbeSFXList_555250[idx],
                    ((unsigned int)((unsigned __int64)(1908874354i64 * (signed __int16)volume) >> 32) >> 31)
                    + ((signed int)((unsigned __int64)(1908874354i64 * (signed __int16)volume) >> 32) >> 1),
                    2 * (signed __int16)volume / 3,
                    pitch,
                    pitch);
                break;
            default:
                return;
            }
        }
        else
        {
        LABEL_30:
            SFX_SfxDefinition_Play_4CA420(&sAbeSFXList_555250[idx], volume, pitch, pitch);
        }
        return;
    }
}