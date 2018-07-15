#include "stdafx.h"
#include "DebugHelpers.hpp"
#include "stdlib.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "PathData.hpp"
#include "PsxDisplay.hpp"
#include "ScreenManager.hpp"
#include "ResourceManager.hpp"
#include "Font.hpp"

//#ifdef DEVELOPER_MODE
//static Font_Context debugFont;
//static ;
//debugFont.LoadFontTypeFromFile("Debug.Font", "Debug.Font_Atlas", debugFontPalette);
//field_F4_font.ctor_433590(256, reinterpret_cast<BYTE*>(debugFontPalette), &debugFont);
//#else
//
//#endif

class ObjectDebugger : public BaseGameObject
{
public:
    ObjectDebugger()
    {
        DisableVTableHack disableHack;

        BaseGameObject_ctor_4DBFA0(1, 1);
        field_6_flags |= BaseGameObject::eDrawable | 0x100;
        field_4_typeId = (BaseGameObject::Types)1001;
        mFontContext.LoadFontTypeFromFile("Debug.Font", "Debug.Font_Atlas", mFontPalette);
        mFont.ctor_433590(512, reinterpret_cast<BYTE*>(mFontPalette), &mFontContext);

        gObjList_drawables_5C1124->Push_Back(this);
    }

    virtual void VDestructor(signed int flags) override
    {
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
    }

    virtual void VUpdate() override
    {

    }

    virtual void vsub_4DC0A0() override
    {
        // Dont kill!
    }

    virtual void VRender(int** pOrderingTable) override
    {
        int pIndex = 0;
        for (int baseObjIdx = 0; baseObjIdx < gBaseGameObject_list_BB47C4->Size(); baseObjIdx++)
        {
            BaseGameObject* pBaseGameObject = gBaseGameObject_list_BB47C4->ItemAt(baseObjIdx);

            if (!pBaseGameObject)
            {
                break;
            }

            if (pBaseGameObject->field_6_flags & BaseGameObject::eBit04)
            {
                auto aliveObj = ((BaseAnimatedWithPhysicsGameObject*)pBaseGameObject);
                int x = aliveObj->field_B8_xpos.GetExponent() - gMap_5C3030.field_24_camera_offset_x.GetExponent();
                int y = aliveObj->field_BC_ypos.GetExponent() - gMap_5C3030.field_28_camera_offset_y.GetExponent();

                if (aliveObj->field_20_animation.field_92_current_frame != -1)
                {
                    auto framePtr = aliveObj->field_20_animation.Get_FrameHeader_40B730(aliveObj->field_20_animation.field_92_current_frame);
                    if (framePtr != nullptr)
                    {
                        y += (framePtr->mBottomRight.y * aliveObj->field_CC_sprite_scale.GetDouble());
                    }
                }
                
                std::string text = std::to_string(pBaseGameObject->field_4_typeId);
                
                pIndex = mFont.DrawString_4337D0(pOrderingTable, (char*)text.c_str(), x - (mFont.MeasureWidth_433700((char*)text.c_str()) / 2) + 1, y + 1, 0, 0, 0, 39, 0, 0, 0, pIndex, FP_FromDouble(1.0), 640, 0);
                pIndex = mFont.DrawString_4337D0(pOrderingTable, (char*)text.c_str(), x - (mFont.MeasureWidth_433700((char*)text.c_str()) / 2), y, 0, 1, 0, 40, 255, 255, 255, pIndex, FP_FromDouble(1.0), 640, 0);
            }
            
        }
    }

    Font mFont;
    char mFontPalette[32];
    Font_Context mFontContext;
};

void DebugHelpers_Init() {
    auto objDebugger = alive_new<ObjectDebugger>();
}

std::vector<BYTE> FS::ReadFile(std::string filePath)
{
    std::ifstream f(filePath, std::ios::binary);
    std::vector<BYTE> v(std::istreambuf_iterator<char>{f}, {});
    return v;
}
