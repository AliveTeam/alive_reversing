#include "stdafx.h"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include <gmock/gmock.h>

BaseAnimatedWithPhysicsGameObject::BaseAnimatedWithPhysicsGameObject()
{
    LOG_WARNING("Exists only to make things build - func may break behaviour of objects");
}

BaseAnimatedWithPhysicsGameObject * BaseAnimatedWithPhysicsGameObject::BaseAnimatedWithPhysicsGameObject_ctor_424930(signed __int16 resourceArraySize)
{
    BaseGameObject_ctor_4DBFA0(1, resourceArraySize);

    SetVTable(&field_20_animation, 0x544290); // gVtbl_animation_2a_544290
    SetVTable(this, 0x544C9C); // gVtbl_BaseAnimatedWithPhysicsGameObject_544C9C

    field_C4_velx = FP(0);
    field_C8_vely = FP(0);

    field_B8_xpos = FP(0);
    field_BC_ypos = FP(0);

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

    field_CC_sprite_scale = FP(1);

    field_D6_scale = 1;
    field_D8_yOffset = 0;
    field_DA_xOffset = 0;

    field_E0_176_ptr = nullptr;

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

        if (field_E0_176_ptr)
        {
            field_E0_176_ptr->dtor_4ACA30();
            Mem_Free_495540(field_E0_176_ptr);
        }
    }
    BaseGameObject_dtor_4DBEC0();
}

void BaseAnimatedWithPhysicsGameObject::dtor_424A40(signed int flags)
{
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
}

void BaseAnimatedWithPhysicsGameObject::VDestructor(signed int flags)
{
    dtor_424A40(flags);
}

void BaseAnimatedWithPhysicsGameObject::VUpdate()
{
    Update_424AB0();
}

void BaseAnimatedWithPhysicsGameObject::Update_424AB0()
{
    // Empty
}

void BaseAnimatedWithPhysicsGameObject::Animation_Init_424E10(int frameTableOffset, int maxW, unsigned __int16 maxH, BYTE **ppAnimData, __int16 a6, unsigned __int8 a7)
{
    if (field_20_animation.Init_40A030(
        frameTableOffset,
        gObjList_animations_5C1A24,
        this,
        maxW,
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

__int16 BaseAnimatedWithPhysicsGameObject::vsub_424EE0(int a2, int a3, int a4, int a5, void* a6)
{
    return sub_424EE0(a2, a3, a4, a5, a6);
}

PSX_RECT* BaseAnimatedWithPhysicsGameObject::vGetBoundingRect_424FD0(PSX_RECT* pRect, int pointIdx)
{
    return GetBoundingRect_424FD0(pRect, pointIdx);
}

__int16 BaseAnimatedWithPhysicsGameObject::vsub_4253B0(int a2, int a3)
{
    return sub_4253B0(a2, a3);
}

__int16 BaseAnimatedWithPhysicsGameObject::vsub_425420(int a2)
{
    return sub_425420(a2);
}

__int16 BaseAnimatedWithPhysicsGameObject::vsub_4254A0(int a2)
{
    return sub_4254A0(a2);
}

__int16 BaseAnimatedWithPhysicsGameObject::vsub_425520(int a2)
{
    return sub_425520(a2);
}

int BaseAnimatedWithPhysicsGameObject::vsub_425840(unsigned __int16 a2)
{
    return sub_425840(a2);
}

void BaseAnimatedWithPhysicsGameObject::vnull_408180()
{
    null_408180();
}

void BaseAnimatedWithPhysicsGameObject::vnull_4081A0()
{
    null_4081A0();
}

__int16 BaseAnimatedWithPhysicsGameObject::sub_424EE0(int /*a2*/, int /*a3*/, int /*a4*/, int /*a5*/, void* /*a6*/)
{
    NOT_IMPLEMENTED();
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

    rect.x = (FP_FromInteger(rect.x) * field_CC_sprite_scale).GetExponent();
    rect.y = (FP_FromInteger(rect.y) * field_CC_sprite_scale).GetExponent();
    rect.w = (FP_FromInteger(rect.w) * field_CC_sprite_scale).GetExponent();
    rect.h = (FP_FromInteger(rect.h) * field_CC_sprite_scale).GetExponent();

    const short xpos = field_B8_xpos.GetExponent();
    const short ypos = field_BC_ypos.GetExponent();

    rect.x += xpos;
    rect.y += ypos;
    rect.w += xpos;
    rect.h += ypos;

    *pRect = rect;
    return pRect;
}

__int16 BaseAnimatedWithPhysicsGameObject::sub_4253B0(int /*a2*/, int /*a3*/)
{
    NOT_IMPLEMENTED();
}

__int16 BaseAnimatedWithPhysicsGameObject::sub_425420(int /*a2*/)
{
    NOT_IMPLEMENTED();
}

__int16 BaseAnimatedWithPhysicsGameObject::sub_4254A0(int /*a2*/)
{
    NOT_IMPLEMENTED();
}

__int16 BaseAnimatedWithPhysicsGameObject::sub_425520(int /*a2*/)
{
    NOT_IMPLEMENTED();
}

int BaseAnimatedWithPhysicsGameObject::sub_425840(unsigned __int16 /*a2*/)
{
    NOT_IMPLEMENTED();
}

void BaseAnimatedWithPhysicsGameObject::null_408180()
{
    NOT_IMPLEMENTED();
}

void BaseAnimatedWithPhysicsGameObject::null_4081A0()
{
    NOT_IMPLEMENTED();
}

__int16 BaseAnimatedWithPhysicsGameObject::Is_In_Current_Camera_424A70()
{
    PSX_RECT rect = {};
    vGetBoundingRect_424FD0(&rect, 1);
    return gMap_5C3030.Is_Rect_In_Current_Camera_480FE0(&rect);
}

namespace Test
{
    class TestObj : public BaseAnimatedWithPhysicsGameObject
    {
    public:
        virtual void VDestructor(signed int) override
        {
            // Stub
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
            t.GetBoundingRect_424FD0(&bRect, 2); // kinda sorta changes the frame to use ??
            PSX_RECT expected = { 33, 45, 3, 5 };
            ASSERT_EQ(bRect, expected);
        }

        t.field_CC_sprite_scale = FP_FromDouble(2.0);

        {
            PSX_RECT bRect = {};
            t.GetBoundingRect_424FD0(&bRect, 1);
            PSX_RECT expected = { (10 * 2) + 3, (20 * 2) + 5, (30 * 2) + 3, (40 * 2) + 5 };
            ASSERT_EQ(bRect, expected);
        }

        {
            t.field_20_animation.field_4_flags.Set(AnimFlags::eBit6_FlipY);
            PSX_RECT bRect = {};
            t.GetBoundingRect_424FD0(&bRect, 1);
            PSX_RECT expected = { (10 * 2) + 3, -(40 * 2) + 5, (30 * 2) + 3, -(20 * 2) + 5  };
            ASSERT_EQ(bRect, expected);
        }

        {
            t.field_20_animation.field_4_flags.Clear(AnimFlags::eBit6_FlipY);
            t.field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
            PSX_RECT bRect = {};
            t.GetBoundingRect_424FD0(&bRect, 1);
            PSX_RECT expected = { -(30 * 2) + 3, (20 * 2) + 5, -(10 * 2) + 3, (40 * 2) + 5 };
            ASSERT_EQ(bRect, expected);
        }
    }

    void BaseAnimatedWithPhysicsGameObjectTests()
    {
        GetBoundingRect_424FD0_Test();
    }
}
