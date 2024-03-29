#pragma once

enum class Layer : s16
{
    // Misc
    eLayer_0 = 0,
    eLayer_1 = 1,

    // Half layers
    eLayer_NotUsed_2 = 2,
    eLayer_BeforeWell_Half_3 = 3,
    eLayer_Well_Half_4 = 4,
    eLayer_RopeWebDrill_Half_5 = 5,
    eLayer_BeforeShadow_Half_6 = 6,
    eLayer_Shadow_Half_7 = 7,  // Shadow in AO - more stuff is on it in AE
    eLayer_8 = 8,
    eLayer_ZapLinesMuds_Half_9 = 9,
    eLayer_BirdPortal_Half_10 = 10,
    eLayer_InBirdPortal_Half_11 = 11,
    eLayer_FallingItemPortalClip_Half_12 = 12,
    eLayer_AbeMenu_Half_13 = 13,
    eLayer_SligGreeterFarts_Half_14 = 14,
    eLayer_SlogFleech_Half_15 = 15,
    eLayer_BombMineCar_Half_16 = 16,
    eLayer_Foreground_Half_17 = 17,
    eLayer_FG1_Half_18 = 18,
    eLayer_19 = 19, // Half menu button, not used
    eLayer_Above_FG1_Half_20 = 20,

    // Full layers
    eLayer_NotUsed_21 = 21,
    eLayer_BeforeWell_22 = 22,
    eLayer_Well_23 = 23,
    eLayer_RopeWebDrill_24 = 24,
    eLayer_BeforeShadow_25 = 25,
    eLayer_Shadow_26 = 26, // Shadow in AO - more stuff is on it in AE
    eLayer_27 = 27,
    eLayer_ZapLinesMuds_28 = 28,
    eLayer_BirdPortal_29 = 29,
    eLayer_InBirdPortal_30 = 30,
    eLayer_FallingItemPortalClip_31 = 31,
    eLayer_AbeMenu_32 = 32,
    eLayer_SligGreeterFarts_33 = 33,
    eLayer_SlogFleech_34 = 34,
    eLayer_BombMineCar_35 = 35,
    eLayer_Foreground_36 = 36,
    eLayer_FG1_37 = 37,
    eLayer_MainMenuButton_38 = 38,
    eLayer_Above_FG1_39 = 39,

    // Misc
    eLayer_FadeFlash_40 = 40,
    eLayer_Menu_41 = 41,
    eLayer_Text_42 = 42,
};
