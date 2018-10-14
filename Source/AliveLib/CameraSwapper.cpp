#include "stdafx.h"
#include "CameraSwapper.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "BackgroundMusic.hpp"
#include "MusicController.hpp"
#include "Sound.hpp"
#include "stdlib.hpp"
#include "ScreenManager.hpp"

void CameraSwapper::ctor_4E5000(BYTE** ppCamRes, CameraSwapEffects changeEffect, __int16 xpos, __int16 ypos)
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    SetVTable(this, 0x5480E4); // vTbl_CameraSwapper_5480E4
    field_4E_xpos_converted = (40 * xpos) / 23;
    field_50_ypos_converted = ypos;
    Init_4E50C0(ppCamRes, changeEffect);
}

void CameraSwapper::dtor_4E5790()
{
    SetVTable(this, 0x5480E4); // vTbl_CameraSwapper_5480E4

    sNum_CamSwappers_5C1B66--;

    if (field_34_pSubObject)
    {
        field_34_pSubObject->field_6_flags.Set(BaseGameObject::eDead);
    }

    if (sMap_word_5C311C)
    {
        gMap_5C3030.sub_480740(0);
        sMap_word_5C311C = 0;
    }

    BackgroundMusic::Play_4CB030();
    MusicController::EnableMusic_47FE10(1);
    Start_Sounds_For_Objects_In_Near_Cameras_4CBB60();
    BaseGameObject_dtor_4DBEC0();
}

void CameraSwapper::vdtor_4E4D90(signed int flags)
{
    dtor_4E5790();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
}

void CameraSwapper::Init_4E50C0(BYTE** ppCamRes, CameraSwapEffects changeEffect)
{
    NOT_IMPLEMENTED();

    field_6_flags.Set(BaseGameObject::eUpdateDuringCamSwap);

    field_4_typeId = Types::eCameraSwapper;

    field_34_pSubObject = nullptr;

    if (changeEffect == CameraSwapEffects::eEffect5 || changeEffect == CameraSwapEffects::eEffect9 || changeEffect == CameraSwapEffects::eEffect10)
    {
        field_30_ppCamRes = ppCamRes;
    }
    else
    {
        pScreenManager_5BB5F4->DecompressToVRam_40EF60(reinterpret_cast<WORD**>(ppCamRes));
    }

    sNum_CamSwappers_5C1B66++;

    if (sNum_CamSwappers_5C1B66 != 1)
    {
        // TODO: Missing flag changes

        // There can only be 1 active at a time
        return;
    }
    field_38_changeEffect = changeEffect;
    switch (changeEffect)
    {
    case CameraSwapEffects::eEffect0:
        pScreenManager_5BB5F4->InvalidateRect_Layer3_40EDB0(0, 0, 640, 240);
        field_6_flags.Set(BaseGameObject::eDead);
        field_34_pSubObject = nullptr;
        return;
        // TODO: Missing cases for other effects
    }
}

void CameraSwapper::VDestructor(signed int flags)
{
    vdtor_4E4D90(flags);
}
