#include "stdafx.h"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "ScreenManager.hpp"
#include "ShadowZone.hpp"
#include "BaseAliveGameObject.hpp"

BaseAnimatedWithPhysicsGameObject::BaseAnimatedWithPhysicsGameObject()
{
    static bool warned = false;
    if (!warned)
    {
        warned = true;
        LOG_WARNING("Exists only to make things build - func may break behaviour of objects");
    }
}

BaseAnimatedWithPhysicsGameObject * BaseAnimatedWithPhysicsGameObject::BaseAnimatedWithPhysicsGameObject_ctor_424930(signed __int16 resourceArraySize)
{
    BaseGameObject_ctor_4DBFA0(1, resourceArraySize);

    SetVTable(&field_20_animation, 0x544290); // gVtbl_animation_2a_544290
    SetVTable(this, 0x544C9C); // gVtbl_BaseAnimatedWithPhysicsGameObject_544C9C

    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);

    field_B8_xpos = FP_FromInteger(0);
    field_BC_ypos = FP_FromInteger(0);

    field_D4_b = 127;
    field_D2_g = 127;
    field_D0_r = 127;

    field_6_flags.Clear(BaseGameObject::eInteractive);
    field_6_flags.Clear(BaseGameObject::eCanExplode);

    field_6_flags.Set(BaseGameObject::eDrawable);
    field_6_flags.Set(BaseGameObject::eIsBaseAnimatedWithPhysicsObj);

    field_DC_bApplyShadows &= ~2;
    field_DC_bApplyShadows |= 1;

    field_C0_path_number = gMap_5C3030.sCurrentPathId_5C3032;
    field_C2_lvl_number = gMap_5C3030.sCurrentLevelId_5C3030;

    field_CC_sprite_scale = FP_FromInteger(1);

    field_D6_scale = 1;
    field_D8_yOffset = 0;
    field_DA_xOffset = 0;

    field_E0_pShadow = nullptr;

    return this;
}

EXPORT void BaseAnimatedWithPhysicsGameObject::BaseAnimatedWithPhysicsGameObject_dtor_424AD0()
{
    SetVTable(this, 0x544C9C); // gVtbl_BaseAnimatedWithPhysicsGameObject_544C9C

    if (!field_6_flags.Get(BaseGameObject::eListAddFailed))
    {
        if (field_6_flags.Get(BaseGameObject::eDrawable))
        {
            gObjList_drawables_5C1124->Remove_Item(this);
            field_20_animation.vCleanUp_40C630();
        }

        if (field_E0_pShadow)
        {
            field_E0_pShadow->dtor_4ACA30();
            Mem_Free_495540(field_E0_pShadow);
        }
    }
    BaseGameObject_dtor_4DBEC0();
}

BaseGameObject* BaseAnimatedWithPhysicsGameObject::vdtor_424A40(signed int flags)
{
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

BaseGameObject* BaseAnimatedWithPhysicsGameObject::VDestructor(signed int flags)
{
    return vdtor_424A40(flags);
}

void BaseAnimatedWithPhysicsGameObject::VUpdate()
{
    Update_424AB0();
}

void BaseAnimatedWithPhysicsGameObject::VRender(int** pOrderingTable)
{
    Render_424B90(pOrderingTable);
}

void BaseAnimatedWithPhysicsGameObject::Update_424AB0()
{
    // Empty
}

void BaseAnimatedWithPhysicsGameObject::Render_424B90(int** pOrderingTable)
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        // Only render if in the active level, path and camera
        if (gMap_5C3030.sCurrentPathId_5C3032 == field_C0_path_number
            && gMap_5C3030.sCurrentLevelId_5C3030 == field_C2_lvl_number
            && Is_In_Current_Camera_424A70() == CameraPos::eCamCurrent_0)
        {
            field_20_animation.field_14_scale = field_CC_sprite_scale;

            __int16 g = field_D2_g;
            __int16 r = field_D0_r;
            __int16 b = field_D4_b;

            PSX_RECT boundingRect = {};
            vGetBoundingRect_424FD0(&boundingRect, 1);
            PSX_Point xy = { boundingRect.x, boundingRect.y };
            PSX_Point wh = { boundingRect.w, boundingRect.h };

            if (field_DC_bApplyShadows & 1)
            {
                ShadowZone::ShadowZones_Calculate_Colour_463CE0(
                    FP_GetExponent(field_B8_xpos),      // Left side
                    (wh.field_2_y + xy.field_2_y) / 2,  // Middle of Height
                    field_D6_scale,
                    &r,
                    &g,
                    &b);
            }

            field_20_animation.field_8_r = static_cast<BYTE>(r);
            field_20_animation.field_9_g = static_cast<BYTE>(g);
            field_20_animation.field_A_b = static_cast<BYTE>(b);

            field_20_animation.vRender_40B820(
                FP_GetExponent((FP_FromInteger(field_DA_xOffset) + field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x)),
                FP_GetExponent((FP_FromInteger(field_D8_yOffset) + field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y)),
                pOrderingTable,
                0,
                0);

            PSX_RECT frameRect = {};
            field_20_animation.Get_Frame_Rect_409E10(&frameRect);
            pScreenManager_5BB5F4->InvalidateRect_40EC90(
                frameRect.x,
                frameRect.y,
                frameRect.w,
                frameRect.h,
                pScreenManager_5BB5F4->field_3A_idx);

            if (field_E0_pShadow)
            {
                field_E0_pShadow->Calculate_Position_4ACA50(
                    field_B8_xpos,
                    field_BC_ypos,
                    &frameRect,
                    field_CC_sprite_scale,
                    field_D6_scale);
                field_E0_pShadow->Render_4ACE60(pOrderingTable);
            }
        }
    }
}


void BaseAnimatedWithPhysicsGameObject::Animation_Init_424E10(int frameTableOffset, int maxW, unsigned __int16 maxH, BYTE **ppAnimData, __int16 a6, unsigned __int8 a7)
{
    if (field_20_animation.Init_40A030(
        frameTableOffset,
        gObjList_animations_5C1A24,
        this,
        static_cast<short>(maxW),
        maxH,
        ppAnimData,
        a7,
        0,
        0))
    {
        if (field_CC_sprite_scale.fpValue == 0x10000)
        {
            field_20_animation.field_C_render_layer = 27;
        }
        else
        {
            field_20_animation.field_C_render_layer = 8;
            field_D6_scale = 0;
        }

        if (!a6 || (gObjList_drawables_5C1124->Push_Back_40CAF0(this)))
        {
            field_20_animation.field_B_render_mode = 0;

            // TODO: Double check this logic
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
            field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
        }
        else
        {
            field_6_flags.Set(BaseGameObject::eDead);
            field_6_flags.Set(BaseGameObject::eListAddFailed);
        }
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead);
        field_6_flags.Set(BaseGameObject::eListAddFailed);
    }

}

void BaseAnimatedWithPhysicsGameObject::vOnCollisionWith_424EE0(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, int startingPointIdx, TCollisionCallBack pFn)
{
    OnCollisionWith_424EE0(xy, wh, pObjList, startingPointIdx, pFn);
}

PSX_RECT* BaseAnimatedWithPhysicsGameObject::vGetBoundingRect_424FD0(PSX_RECT* pRect, int pointIdx)
{
    return GetBoundingRect_424FD0(pRect, pointIdx);
}

__int16 BaseAnimatedWithPhysicsGameObject::vIsObjNearby_4253B0(FP radius, BaseAnimatedWithPhysicsGameObject * pObj)
{
    return IsObjNearby_4253B0(radius, pObj);
}

__int16 BaseAnimatedWithPhysicsGameObject::vIsObj_GettingNear_425420(BaseAnimatedWithPhysicsGameObject* pObj)
{
    return IsObj_GettingNear_On_X_425420(pObj);
}

__int16 BaseAnimatedWithPhysicsGameObject::vIsFacingMe_4254A0(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return IsFacingMe_4254A0(pOther);
}

__int16 BaseAnimatedWithPhysicsGameObject::vOnSameYLevel_425520(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return OnSameYLevel_425520(pOther);
}

void BaseAnimatedWithPhysicsGameObject::vStackOnObjectsOfType_425840(Types typeToFind)
{
    StackOnObjectsOfType_425840(typeToFind);
}

void BaseAnimatedWithPhysicsGameObject::VOnPickUpOrSlapped()
{
    vOnPickUpOrSlapped_408180();
}

void BaseAnimatedWithPhysicsGameObject::vnull_4081A0(BaseGameObject* pFrom)
{
    null_4081A0(pFrom);
}

PSX_RECT* BaseAnimatedWithPhysicsGameObject::GetBoundingRect_424FD0(PSX_RECT* pRect, int pointIdx)
{
    const FrameInfoHeader* pAnimFrameHeader = field_20_animation.Get_FrameHeader_40B730(-1);

    PSX_RECT rect = {};
    // Normally this data is 3 points, one that is the frame offset and then 2 that make up the bounding rect.
    // So usually pointIdx is 1. However the way the data is structured it could be anything to treat any index
    // into the array of points as a unique bounding rectangle. Also it appears there can be more than 3 points
    // supported in the data too.
    rect.x = pAnimFrameHeader->field_8_data.points[pointIdx].x;
    rect.y = pAnimFrameHeader->field_8_data.points[pointIdx].y;
    rect.w = pAnimFrameHeader->field_8_data.points[pointIdx +1].x;
    rect.h = pAnimFrameHeader->field_8_data.points[pointIdx +1].y;

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        std::swap(rect.x, rect.w);
        rect.x = -rect.x;
        rect.w = -rect.w;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit6_FlipY))
    {
        std::swap(rect.y, rect.h);
        rect.y = -rect.y;
        rect.h = -rect.h;
    }

    rect.x = FP_GetExponent((FP_FromInteger(rect.x) * field_CC_sprite_scale));
    rect.y = FP_GetExponent((FP_FromInteger(rect.y) * field_CC_sprite_scale));
    rect.w = FP_GetExponent((FP_FromInteger(rect.w) * field_CC_sprite_scale));
    rect.h = FP_GetExponent((FP_FromInteger(rect.h) * field_CC_sprite_scale));

    const short xpos = FP_GetExponent(field_B8_xpos);
    const short ypos = FP_GetExponent(field_BC_ypos);

    rect.x += xpos;
    rect.y += ypos;
    rect.w += xpos;
    rect.h += ypos;

    *pRect = rect;
    return pRect;
}

__int16 BaseAnimatedWithPhysicsGameObject::IsObjNearby_4253B0(FP radius, BaseAnimatedWithPhysicsGameObject * pObj)
{
    FP distance = pObj->field_B8_xpos - field_B8_xpos;

    if (distance < FP_FromInteger(0))
    {
        distance = field_B8_xpos - pObj->field_B8_xpos;
    }

    return distance <= radius;
}

__int16 BaseAnimatedWithPhysicsGameObject::IsObj_GettingNear_On_X_425420(BaseAnimatedWithPhysicsGameObject* pOther)
{
    if (pOther->field_B8_xpos < field_B8_xpos && pOther->field_C4_velx > field_C4_velx)
    {
        // Its before our xpos but its velocity is moving towards our xpos!
        return TRUE;
    }

    if (pOther->field_B8_xpos > field_B8_xpos && pOther->field_C4_velx < field_C4_velx)
    {
        // Its after our xpos but its velocity is moving towards our xpos!
        return TRUE;
    }

    // Not heading our way
    return FALSE;
}

// Muds use this to face "away" from Abe when stood on the same grid block. Also used to follow Abe in the correct direction etc.
__int16 BaseAnimatedWithPhysicsGameObject::IsFacingMe_4254A0(BaseAnimatedWithPhysicsGameObject* pOther)
{
    if (pOther->field_B8_xpos == field_B8_xpos
        && pOther->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) != field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        // They are in the same spot as us, so they can only be facing us if they are NOT facing the same way.
        // This seems strange but its what causes muds to keep changing direction if you turn while you are stood in the same grid as them.
        return TRUE;
    }
    else if (pOther->field_B8_xpos > field_B8_xpos && !field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        // They are to the right of us and facing left
        return TRUE;
    }
    else if (pOther->field_B8_xpos < field_B8_xpos && field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        // They are to the left of using and facing right
        return TRUE;
    }

    return FALSE;
}

// This is how Scrabs, Fleeches (and probably other stuff) know you are on the same "floor"
__int16 BaseAnimatedWithPhysicsGameObject::OnSameYLevel_425520(BaseAnimatedWithPhysicsGameObject* pOther)
{
    // Get bounding rects
    PSX_RECT ourRect = {};
    GetBoundingRect_424FD0(&ourRect, 1);

    PSX_RECT theirRect = {};
    pOther->GetBoundingRect_424FD0(&theirRect, 1);

    // Get mid Y of each
    const int theirMidY = (theirRect.h + theirRect.y) / 2;
    const int ourMidY = (ourRect.h + ourRect.y) / 2;

    if (theirMidY <= ourRect.h && theirMidY >= ourRect.y)
    {
        return TRUE;
    }

    if (ourMidY <= theirRect.h && ourMidY >= theirRect.y)
    {
        return TRUE;
    }

    return FALSE;
}

void BaseAnimatedWithPhysicsGameObject::StackOnObjectsOfType_425840(Types typeToFind)
{
    // For some reason this isn't const in the real game
    const __int16 kData[6] =
    {
        0, 3, -3, 6, -6, 2
    };

    int data_idx = 0;
    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == typeToFind && pObj != this)
        {
            data_idx++;
            if (data_idx >= ALIVE_COUNTOF(kData))
            {
                data_idx = 0;
            }
        }
    }

    field_DA_xOffset = FP_GetExponent(FP_FromInteger(kData[data_idx]) * field_CC_sprite_scale);
}

void BaseAnimatedWithPhysicsGameObject::vOnPickUpOrSlapped_408180()
{
    NOT_IMPLEMENTED();
}

void BaseAnimatedWithPhysicsGameObject::null_4081A0(BaseGameObject* /*pFrom*/)
{
    NOT_IMPLEMENTED();
}

void BaseAnimatedWithPhysicsGameObject::DealDamageRect_4247A0(PSX_RECT * pRect)
{
    NOT_IMPLEMENTED();

    if (gBaseAliveGameObjects_5C1B7C)
    {
        int w1 = pRect->w;
        int w2 = pRect->w;
        if (pRect->x <= w1)
        {
            w2 = pRect->x;
        }
        int w3 = pRect->w;
        if (w1 <= pRect->x)
        {
            w3 = pRect->x;
        }
        int y1 = pRect->y;
        int h1 = pRect->h;
        int h2 = pRect->h;
        if (y1 <= h1)
        {
            h2 = pRect->y;
        }
        int h3 = pRect->h;
        if (h1 <= y1)
        {
            h3 = pRect->y;
        }
        int xposInt = FP_GetExponent(field_B8_xpos);
        int xposOffW2 = xposInt + w2;
        int xposOffW3 = xposInt + w3;
        int yposInt = FP_GetExponent(field_BC_ypos);
        int yposOffH2 = yposInt + h2;
        int yposOffH3 = yposInt + h3;

        for (int baseObjIdx = 0; baseObjIdx < gBaseAliveGameObjects_5C1B7C->Size(); baseObjIdx++)
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(baseObjIdx);

            if (!pObj)
            {
                break;
            }

            int xposInt2 = FP_GetExponent(field_B8_xpos);

            if (xposInt2 >= xposOffW2 && xposInt2 <= xposOffW3)
            {
                int yposInt2 = FP_GetExponent(field_BC_ypos);

                if (yposInt2 >= yposOffH2
                    && yposInt2 <= yposOffH3
                    && this->field_CC_sprite_scale == (pObj->field_CC_sprite_scale * FP_FromDouble(2.75)))
                {
                    pObj->VTakeDamage_408730(this);
                }
            }
        }
    }
}

CameraPos BaseAnimatedWithPhysicsGameObject::Is_In_Current_Camera_424A70()
{
    PSX_RECT rect = {};
    vGetBoundingRect_424FD0(&rect, 1);
    return gMap_5C3030.Is_Rect_In_Current_Camera_480FE0(&rect);
}

void BaseAnimatedWithPhysicsGameObject::OnCollisionWith_424EE0(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList, int startingPointIdx, TCollisionCallBack pFn)
{
    if (!pObjList)
    {
        return;
    }

    //LOG_INFO("X " << xy.field_0_x << " Y " << xy.field_2_y << " W " << wh.field_0_x << " H " << wh.field_2_y);

    for (int i = 0; i < pObjList->Size(); i++)
    {
        BaseGameObject* pElement = pObjList->ItemAt(i);
        if (!pElement)
        {
            break;
        }

        if (pElement->field_6_flags.Get(BaseGameObject::eIsBaseAnimatedWithPhysicsObj))
        {
            BaseAnimatedWithPhysicsGameObject* pObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(pElement);
            if (pObj->field_6_flags.Get(BaseGameObject::eDrawable))
            {
                PSX_RECT bRect = {};
                pObj->GetBoundingRect_424FD0(&bRect, startingPointIdx);
                if (xy.field_0_x <= bRect.w && xy.field_2_y <= bRect.h && wh.field_0_x >= bRect.x  && wh.field_2_y >= bRect.y && field_D6_scale == pObj->field_D6_scale)
                {
                    if (!(this->*(pFn))(pObj))
                    {
                        break;
                    }
                }
            }
        }
    }
}

void BaseAnimatedWithPhysicsGameObject::SetTint_425600(const TintEntry* pTintArray, LevelIds level_id)
{
    while (pTintArray->field_0_level != static_cast<int>(level_id)) // TODO: HACK in static data its a byte which doesn't match the enum size
    {
        if (pTintArray->field_0_level == static_cast<int>(level_id) || pTintArray->field_0_level == static_cast<int>(LevelIds::eNone))
        {
            break;
        }
        pTintArray++;
    }

    field_D0_r = pTintArray->field_1_r;
    field_D2_g = pTintArray->field_2_g;
    field_D4_b = pTintArray->field_3_b;
}


void BaseAnimatedWithPhysicsGameObject::SetRGB(__int16 r, __int16 g, __int16 b)
{
    field_D0_r = r;
    field_D2_g = g;
    field_D4_b = b;
}

namespace Test
{
    class TestObj : public BaseAnimatedWithPhysicsGameObject
    {
    public:
        virtual BaseGameObject* VDestructor(signed int) override
        {
            // Stub
            return this;
        }
    };

    struct TestAnimData
    {
        AnimationHeader mHeader;
        FrameInfoHeader mFrameInfoHeader[10];
        FrameHeader mFrameHeader;
    };

    static void GetBoundingRect_424FD0_Test()
    {
        TestObj t;

        t.field_20_animation.field_4_flags.Raw().all = 0;
        t.field_20_animation.field_92_current_frame = 0;
        t.field_20_animation.field_14_scale = FP_FromDouble(1.0);
        t.field_B8_xpos = FP_FromDouble(3.0);
        t.field_BC_ypos = FP_FromDouble(5.0);

        TestAnimData testData = {};
        testData.mHeader.mFrameOffsets[0] = sizeof(AnimationHeader);
        testData.mHeader.field_0_fps = 2;

        testData.mFrameInfoHeader[0].field_0_frame_header_offset = sizeof(AnimationHeader) + sizeof(FrameInfoHeader);
        testData.mFrameInfoHeader[0].field_6_count = 1;
        testData.mFrameInfoHeader[0].field_8_data.points[0].x = 77;
        testData.mFrameInfoHeader[0].field_8_data.points[0].y = 66;
        testData.mFrameInfoHeader[0].field_8_data.points[1].x = 10;
        testData.mFrameInfoHeader[0].field_8_data.points[1].y = 20;
        testData.mFrameInfoHeader[0].field_8_data.points[2].x = 30;
        testData.mFrameInfoHeader[0].field_8_data.points[2].y = 40;

        testData.mFrameInfoHeader[1].field_8_data.points[0].x = 77;
        testData.mFrameInfoHeader[1].field_8_data.points[0].y = 66;
        testData.mFrameInfoHeader[1].field_8_data.points[1].x = 10;
        testData.mFrameInfoHeader[1].field_8_data.points[1].y = 20;
        testData.mFrameInfoHeader[1].field_8_data.points[2].x = 30;
        testData.mFrameInfoHeader[1].field_8_data.points[2].y = 40;

        //testData.mFrameHeader.field_4_width = 20;
        //testData.mFrameHeader.field_5_height = 30;
        testData.mFrameHeader.field_6_colour_depth = 8;

        TestAnimData* pTestData = &testData;

        t.field_20_animation.field_20_ppBlock = (BYTE **)&pTestData;
        t.field_20_animation.field_18_frame_table_offset = 0;

        t.field_CC_sprite_scale = FP_FromDouble(1.0);

        {
            PSX_RECT bRect = {};
            t.vGetBoundingRect_424FD0(&bRect, 2); // kinda sorta changes the frame to use ??
            PSX_RECT expected = { 33, 45, 3, 5 };
            ASSERT_EQ(bRect, expected);
        }

        t.field_CC_sprite_scale = FP_FromDouble(2.0);

        {
            PSX_RECT bRect = {};
            t.vGetBoundingRect_424FD0(&bRect, 1);
            PSX_RECT expected = { (10 * 2) + 3, (20 * 2) + 5, (30 * 2) + 3, (40 * 2) + 5 };
            ASSERT_EQ(bRect, expected);
        }

        {
            t.field_20_animation.field_4_flags.Set(AnimFlags::eBit6_FlipY);
            PSX_RECT bRect = {};
            t.vGetBoundingRect_424FD0(&bRect, 1);
            PSX_RECT expected = { (10 * 2) + 3, -(40 * 2) + 5, (30 * 2) + 3, -(20 * 2) + 5  };
            ASSERT_EQ(bRect, expected);
        }

        {
            t.field_20_animation.field_4_flags.Clear(AnimFlags::eBit6_FlipY);
            t.field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
            PSX_RECT bRect = {};
            t.vGetBoundingRect_424FD0(&bRect, 1);
            PSX_RECT expected = { -(30 * 2) + 3, (20 * 2) + 5, -(10 * 2) + 3, (40 * 2) + 5 };
            ASSERT_EQ(bRect, expected);
        }
    }

    void BaseAnimatedWithPhysicsGameObjectTests()
    {
        GetBoundingRect_424FD0_Test();
    }
}
