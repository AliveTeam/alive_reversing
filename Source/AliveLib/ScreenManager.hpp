#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Psx.hpp"

#pragma pack(push)
#pragma pack(2)
class Camera // TODO: May actually just be "ResourceList" ?
{
public:
    Camera();
public:
    EXPORT void ctor_480DD0();
    EXPORT void dtor_480E00();

    DynamicArray field_0;
    void* field_C_pCamRes;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    __int16 field_30;
};
#pragma pack(pop)
ALIVE_ASSERT_SIZEOF(Camera, 0x32);

class ScreenManager : public BaseGameObject
{
public:
    // TODO
    EXPORT void sub_40EE10();

    EXPORT void sub_cam_vlc_40EF60(unsigned __int16** a2);
    EXPORT void MoveImage_40EB70();

    EXPORT void InvalidateRect_40EC90(int x, int y, signed int width, signed int height, int a6);
    EXPORT void InvalidateRect_40EC10(int x, int y, signed int width, signed int height);

    virtual void VDestructor(signed int flags) override;
    ScreenManager(int a2, int a3);
private:
    EXPORT void ctor_40E3E0(int a2, int a3);
    EXPORT void dtor_40E460(signed int flags);

private:
    int field_20_pMap;
    int field_24_screen_blocks;
    int field_28;
    PSX_RECT field_2C_rect;
    int field_34;
    __int16 field_38;
    unsigned __int16 field_3A;
    int field_3C;
    int field_40_flags;
    int field_44;
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
    int field_58;
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    int field_88;
    int field_8C;
    int field_90;
    int field_94;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
    int field_A8;
    int field_AC;
    int field_B0;
    int field_B4;
    int field_B8;
    int field_BC;
    int field_C0;
    int field_C4;
    int field_C8;
    int field_CC;
    int field_D0;
    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
    int field_100;
    int field_104;
    int field_108;
    int field_10C;
    int field_110;
    int field_114;
    int field_118;
    int field_11C;
    int field_120;
    int field_124;
    int field_128;
    int field_12C;
    int field_130;
    int field_134;
    int field_138;
    int field_13C;
    int field_140;
    int field_144;
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
    int field_158;
    int field_15C;
    int field_160;
    int field_164;
    int field_168;
    int field_16C;
    int field_170;
    int field_174;
    int field_178;
    int field_17C;
    int field_180;
    int field_184;
    int field_188;
    int field_18C;
    int field_190;
    int field_194;
    int field_198;
    int field_19C;
    int field_1A0;
};
ALIVE_ASSERT_SIZEOF(ScreenManager, 0x1A4u);

ALIVE_VAR_EXTERN(ScreenManager*, pScreenManager_5BB5F4);

