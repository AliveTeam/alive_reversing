#include "stdafx_ao.h"
#include "ScreenManager.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"

#undef min
#undef max

START_NS_AO

ALIVE_VAR(1, 0x4FF7C8, ScreenManager*, pScreenManager_4FF7C8, nullptr);

Camera* Camera::ctor_4446E0()
{
    field_0_array.ctor_4043E0(10);
    field_30_flags &= ~1u;
    field_C_ppBits = nullptr;
    return this;
}

void Camera::dtor_444700()
{
    ResourceManager::FreeResource_455550(field_C_ppBits);

    for (int i = 0; i < field_0_array.Size(); i++)
    {
        BYTE** ppRes = field_0_array.ItemAt(i);
        if (!ppRes)
        {
            break;
        }

        ResourceManager::FreeResource_455550(ppRes);
        i = field_0_array.RemoveAt(i);
    }

    field_0_array.dtor_404440();
}


void CC Camera::On_Loaded_4447A0(Camera* pThis)
{
    pThis->field_30_flags |= 1u;
    pThis->field_C_ppBits = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Bits, pThis->field_10_resId, 1, 0);
}

void ScreenManager::MoveImage_406C40()
{
    PSX_RECT rect = {};
    rect.x = field_20_upos;
    rect.y = field_22_vpos;
    rect.h = 240;
    rect.w = 640;
    PSX_MoveImage_4961A0(&rect, 0, 0);
}

void ScreenManager::DecompressToVRam_407110(unsigned __int16** /*ppBits*/)
{
    NOT_IMPLEMENTED();
}

void ScreenManager::InvalidateRect_406CC0(int x, int y, signed int width, signed int height)
{
    InvalidateRect_406E40(x, y, width, height, field_2E_idx);
}

ScreenManager* ScreenManager::ctor_406830(BYTE** ppBits, FP_Point* pCameraOffset)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BA230);

    field_10_pCamPos = pCameraOffset;

    field_6_flags.Set(Options::eSurviveDeathReset_Bit9);
    field_6_flags.Set(Options::eUpdateDuringCamSwap_Bit10);

    Init_4068A0(ppBits);
    return this;
}

void ScreenManager::Init_4068A0(BYTE** /*ppBits*/)
{
    NOT_IMPLEMENTED();
}

BaseGameObject* ScreenManager::VDestructor(signed int flags)
{
    return vdtor_407290(flags);
}

void ScreenManager::UnsetDirtyBits_FG1_406EF0()
{
    memset(&field_58_20x16_dirty_bits[4], 0, sizeof(this->field_58_20x16_dirty_bits[4]));
    memset(&field_58_20x16_dirty_bits[5], 0, sizeof(this->field_58_20x16_dirty_bits[5]));
}

void ScreenManager::InvalidateRect_406E40(int x, int y, signed int width, signed int height, int idx)
{
    x = std::max(x, 0);
    y = std::max(y, 0);

    width = std::min(width, 639);
    height = std::min(height, 239);

    for (int tileX = x / 32; tileX <= width / 32; tileX++)
    {
        for (int tileY = y / 16; tileY <= height / 16; tileY++)
        {
            field_58_20x16_dirty_bits[idx].SetTile(tileX, tileY, true);
        }
    }
}

void ScreenManager::InvalidateRect_Layer3_406F20(int x, int y, int width, int height)
{
    InvalidateRect_406E40(x, y, width, height, 3);
}


void ScreenManager::InvalidateRect_406D80(int x, int y, signed int width, signed int height, int idx)
{
    InvalidateRect_406E40(x, y, width, height, idx + 4);
}

void ScreenManager::VScreenChanged()
{
    // Empty
}

void ScreenManager::VUpdate()
{
    // Empty
}

ScreenManager* ScreenManager::vdtor_407290(signed int /*flags*/)
{
    NOT_IMPLEMENTED();
    return this;
}

END_NS_AO
