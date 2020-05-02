#include "stdafx.h"
#include "Game.hpp"
#include "logger.hpp"
#include "Function.hpp"
#include <set>
#include "..\AliveLib\FixedPoint.hpp"

void Game_ForceLink()
{

}


EXPORT void* alloc_450740(size_t)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

EXPORT void* alloc_447520(size_t)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

template<typename T, typename... Args>
inline T* alive_new(Args&&... args)
{
    void* buffer = alloc_447520(sizeof(T));
    if (buffer)
    {
        return new (buffer) T(std::forward<Args>(args)...);
    }
    return nullptr;
}

EXPORT void Mem_Free_450770(void*)
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0x9F2DF0, class DynamicArray*, gBaseGameObject_list_9F2DF0, nullptr);
ALIVE_VAR(1, 0x504618, class DynamicArray*, gObjList_drawables_504618, nullptr);


EXPORT void CC Remove_Item_417350(void*)
{
    NOT_IMPLEMENTED();
}


class BaseGameObject
{
public:
    EXPORT BaseGameObject* ctor_487E10(__int16 arraySize);


    void dtor_487DF0()
    {
        SetVTable(this, 0x4BD488); // vTable_BaseGameObject_4BD488
        Remove_Item_417350(this);
    }

    virtual BaseGameObject* VDestructor(signed int flags) = 0;

    virtual void VUpdate()
    {

    }

    virtual void VRender(int**)
    {

    }

    virtual void VScreenChanged()
    {

    }

    virtual void VUnknown()
    {

    }

public:
    //int field_0_VTable;
    __int16 field_4_typeId;
    unsigned __int16 field_6_flags;
    int field_8_update_delay;
    char field_C_bCanKill;
    char field_D;
    __int16 field_E;
};
ALIVE_ASSERT_SIZEOF(BaseGameObject, 0x10);

struct Animation
{

    virtual void VDummy() {}

    __int16 field_4_flags;
    __int16 field_6;
    int field_8;
    __int16 field_C_layer;
    __int16 field_E;
    int field_10;
    int field_14;
    int field_18;
    int field_1C_fn_ptrs;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
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
    __int16 field_84;
    __int16 field_86;
    __int16 field_88;
    __int16 field_8A;
    __int16 field_8C_vram_x_pal;
    __int16 field_8E_vram_y_pal;
    __int16 field_90_pal_depth;
    __int16 field_92;
    int field_94;
};
ALIVE_ASSERT_SIZEOF(Animation, 0x98);

class BaseAnimatedObj : public BaseGameObject
{
public:
    Animation field_10_anim;
    FP field_A8_xpos;
    FP field_AC_ypos;
    __int16 field_B0;
    __int16 field_B2;
    int field_B4;
    int field_B8;
    int field_BC_scale;
    __int16 field_C0_r;
    __int16 field_C2_g;
    __int16 field_C4_b;
    __int16 field_C6;
    __int16 field_C8;
    __int16 field_CA;
    char field_CC;
    char field_CD;
    char field_CE;
    char field_CF;
    int field_D0_res;
};
ALIVE_ASSERT_SIZEOF(BaseAnimatedObj, 0xD4);


class DynamicArray
{
public:
    EXPORT DynamicArray* ctor_4043E0(__int16 startingSize)
    {
        const auto sizeInBytes = startingSize * sizeof(void*);
        field_0_array = reinterpret_cast<void**>(alloc_450740(sizeInBytes));

        field_4_used_size = 0;
        field_6_max_size = 0;

        if (field_0_array)
        {
            memset(field_0_array, 0, sizeInBytes);
            field_4_used_size = 0;
            field_6_max_size = startingSize;
            field_8_expand_size = 10;
        }

        return this;
    }

    EXPORT void dtor_404440()
    {
        Mem_Free_450770(field_0_array);
    }

public:
    EXPORT __int16 Push_Back_404450(void *item)
    {
        if (!item || !field_0_array)
        {
            return 0;
        }

        if (field_4_used_size == field_6_max_size)
        {
            // Inlined expand call?
            const __int16 newSizeElements = field_6_max_size + field_8_expand_size;
            const auto newArraySizeBytes = newSizeElements * sizeof(void*);
            auto pNewArray = reinterpret_cast<void**>(alloc_450740(newArraySizeBytes));
            if (!pNewArray)
            {
                return 0;
            }

            // Clear new memory + copy in old array
            memset(pNewArray, 0, newArraySizeBytes);
            memcpy(pNewArray, field_0_array, field_4_used_size * sizeof(void*));

            // Free old array + assign new values
            Mem_Free_450770(field_0_array);
            field_0_array = pNewArray;
            field_6_max_size = newSizeElements;
        }
        
        field_0_array[field_4_used_size++] = item;

        return 1;
    }

//protected:

    EXPORT __int16 Remove_Item_404520(void *item)
    {
        // TODO: FIX ME
        NOT_IMPLEMENTED();

        int counter = 0;
        void* pCurItem = nullptr;
        for (;;)
        {
            if (counter < field_4_used_size)
            {
                counter++;
                pCurItem = field_0_array[counter];
            }
            else
            {
                pCurItem = nullptr;
            }

            if (!pCurItem)
            {
                break;
            }

            if (pCurItem == item)
            {
                field_4_used_size--;
                field_0_array[counter - 1] = field_0_array[field_4_used_size];
                field_0_array[field_4_used_size] = nullptr;
                return 1;
            }
        }
        return 0;
    }
public:
    void** field_0_array;
public:
    __int16 field_4_used_size;
private:
    __int16 field_6_max_size;
    __int16 field_8_expand_size;
    __int16 field_A_pad;
};

ALIVE_ASSERT_SIZEOF(DynamicArray, 0xC);




class ResourceManager
{
public:
    EXPORT void static CC Free_447540(void*)
    {
        NOT_IMPLEMENTED();
    }
};

class ObjectDumper;
ObjectDumper* gObjDumper = nullptr;


const static std::map<DWORD, std::string> sVTableToName = 
{
    { 0x4ba340, "DDCheat"},
    { 0x4bc028, "FG1"},
    { 0x4bb270, "Hoist"},
    { 0x4ba1a8, "DoorLight"},
    { 0x4bb288, "HoistParticle"},
    { 0x4ba8a8, "CheatController"},
    { 0x4bb158, "Abe"},
    { 0x4bbba8, "MusicController"},
    { 0x4ba890, "GameSpeak"},
    { 0x4ba230, "ScreenManager"},
    { 0x4ba230, "ScreenManager"},
    { 0x4bcd30, "BackgroundMusic"},
    { 0x4bcd30, "BackgroundMusic"},
    { 0x4bb088, "ResourceManager"},
    { 0x4bce78, "Menu"},

    // 102 VTable: 0x4bd7c8
    // 
};

ALIVE_VAR(1, 0xAC6420, int*, psxemu_videomem_AC6420, nullptr);

class Bmp
{
public:
    // 0x00ac6420
    // 8
    // 16
    // 0x00ac6420 fff7
    // 0x00ac6420 1
    // str
    EXPORT static int CC DrawString_48FB90(int* /*pBmp*/, int /*x*/, int /*y*/, int /*colour*/, int /*backColour*/, const char* /*lpString*/)
    {
        NOT_IMPLEMENTED();
        return 0;
    }
};

class FontContext
{
public:
    EXPORT __int16 LoadFontType_41C040(int)
    {
        NOT_IMPLEMENTED();
    }


    EXPORT void dtor_41C110()
    {
        NOT_IMPLEMENTED();
    }


    __int16 field_0;
    __int16 field_2;
    int field_4;
    int field_8;
    __int16 field_C;
    __int16 field_E;
};

ALIVE_VAR(1, 0x4FFD68, FontContext, sFontContext_4FFD68, {});

class AliveFont
{
public:
    EXPORT AliveFont* ctor_41C170(int /*a2*/, BYTE* /*pPal*/, FontContext* /*pContext*/)
    {
        NOT_IMPLEMENTED();
    }

    EXPORT unsigned int MeasureWidth_41C2B0(const char* /*pStr*/)
    {
        NOT_IMPLEMENTED();
        return 0;
    }

    EXPORT int DrawString_41C360(int** , const char *, __int16, __int16 , int , int , int , int , unsigned char , unsigned char , unsigned char , int , FP , int , int )
    {
        NOT_IMPLEMENTED();
        return 0;
    }

    EXPORT void dtor_41C130()
    {
        NOT_IMPLEMENTED();
    }


    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    __int16 field_28;
    __int16 field_2A;
    int field_2C;
    int field_30;
    FontContext *field_34_pFontContext;
};

char sStatsSignFontPalette_4CD570[32] =
{
  '\0',
  '\0',
  '\x01',
  '€',
  '\x01',
  '„',
  ' ',
  '„',
  '!',
  '€',
  ' ',
  '„',
  '!',
  '„',
  'e',
  'Î',
  'e',
  'Œ',
  'Œ',
  '±',
  '\x13',
  '”',
  'd',
  'Î',
  'e',
  'Î',
  '×',
  '˜',
  '\x14',
  '¡',
  '\x18',
  'Ø'
};


// TODO: Wrong values
 BYTE pal_4D0090[32] = {
    0x00, 0x00, 0x21, 0x84, 0x42, 0x88, 0x63, 0x8C, 0x84, 0x90,
    0xA5, 0x14, 0xE7, 0x1C, 0x08, 0x21, 0x29, 0x25, 0x4A, 0x29,
    0x6B, 0x2D, 0x8C, 0x31, 0xAD, 0x35, 0xEF, 0x3D, 0x10, 0x42,
    0x73, 0x4E };

EXPORT void ResourceManager_Request_446C90(const char *, int , int , __int16 , int )
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0x508BF4, BYTE, byte_508BF4, 0);

struct Map
{
    __int16 field_0_current_level;
    __int16 field_2_current_path;
    __int16 field_4_current_camera;
    __int16 field_6_state;
    __int16 field_8_force_load;
    __int16 field_A_level;
    __int16 field_C_path;
    __int16 field_E_camera;
    __int16 field_10_screenChangeEffect;
    __int16 field_12_fmv_base_id;
    __int16 field_14_direction;
    __int16 field_16;
    int field_18_pAliveObj;
    __int16 field_1C_cameraSwapEffect;
    __int16 field_1E_door;
    __int16 field_20;
    __int16 field_22;
    __int16 field_24;
    __int16 field_26_door;
    __int16 field_28_cd_or_overlay_num;
    __int16 field_2A;
    FP field_2C_camera_offset_x;
    FP field_30_camera_offset_y;
    int field_34_camera_array[5];
    int field_48_stru_5[5];
    int field_5C_path_res_array[30];
    int field_D4;
    __int16 field_D8;
    __int16 field_DA;
    __int16 field_DC;
    __int16 field_DE;
    int field_E0;
};
ALIVE_ASSERT_SIZEOF(Map, 0xE4);

ALIVE_VAR(1, 0x507BA8, Map, gMap_507BA8, {});

template<class T>
static std::string ToHex(T value)
{
    std::stringstream sstream;
    sstream << std::hex << value;
    return sstream.str();
}

class ObjectDumper : public BaseGameObject
{
public:

    AliveFont mFont[50];
    //FontContext mFontContext;
    int mCounter = 0;
    bool mLoaded = false;

    ObjectDumper()
    {
        DisableVTableHack disableHack;

        ctor_487E10(1);
        LOG_INFO(field_6_flags);

        field_6_flags |= 0x300;
        field_6_flags |= 0x8;
        field_8_update_delay = 0;

        gObjList_drawables_504618->Push_Back_404450(this);

        gObjDumper = this;


    }


    ~ObjectDumper()
    {
        LOG_INFO("dtor");
    }

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        gObjDumper = nullptr;
        gObjList_drawables_504618->Remove_Item_404520(this);

        for (auto& font : mFont)
        {
            font.dtor_41C130();
        }

        //mFontContext.dtor_41C110();

        dtor_487DF0();
        if (flags & 1)
        {
            ResourceManager::Free_447540(this);
        }
        return this;
    }

    virtual void VUpdate() override
    {
        if (GetKeyState(VK_F12))
        {
            DumpObjects();
        }

        mCounter++;
        if (mCounter > 250 && !mLoaded)
        {

            //ResourceManager_Request_446C90("LCDFONT.FNT", 'tnoF', 2, 1, 0);

           // mFontContext.LoadFontType_41C040(2);
            for (auto& font : mFont)
            {
                font.ctor_41C170(20, pal_4D0090, &sFontContext_4FFD68);
            }

            mLoaded = true;
        }
    }

    virtual void VScreenChanged() override
    {
        //LOG_INFO("Don't kill me!");
    }

    virtual void VRender(int** ppOt)
    {
        if (mLoaded)
        {
            int fontCount = 0;
            for (int i = 0; i < gBaseGameObject_list_9F2DF0->field_4_used_size; i++)
            {
                BaseGameObject* pObj = static_cast<BaseGameObject*>(gBaseGameObject_list_9F2DF0->field_0_array[i]);

                if (pObj->field_6_flags & 0x10 && pObj->field_4_typeId != 0)
                {
                  
                    BaseAnimatedObj* pAnimated = static_cast<BaseAnimatedObj*>(pObj);

                    byte_508BF4 = 0;

                    auto objX = FP_GetExponent(pAnimated->field_A8_xpos);
                    auto mapX = FP_GetExponent(gMap_507BA8.field_2C_camera_offset_x);

                    auto objY = FP_GetExponent(pAnimated->field_AC_ypos);
                    auto mapY = FP_GetExponent(gMap_507BA8.field_30_camera_offset_y);

                    if (objX == 0 || objY == 0)
                    {
                        continue;
                    }

                    auto xpos = static_cast<short>((objX - mapX) + 160);
                    auto ypos = static_cast<short>((objY - mapY) + 50);

                    struct VTableHack
                    {
                        DWORD vtable;
                    };

                    //VTableHack* pHack = reinterpret_cast<VTableHack*>(pAnimated);

                    auto tmp = std::to_string(pAnimated->field_4_typeId);// +" " + ToHex(pHack->vtable);

                    mFont[fontCount++].DrawString_41C360(
                        ppOt,
                        tmp.c_str(),
                        xpos,
                        ypos,
                        1,
                        3,
                        0,
                        39,
                        255,
                        0,
                        255,
                        0,
                        FP_FromDouble(1.0),
                        640, // max render x
                        0); // random colour range

                    if (fontCount >= ALIVE_COUNTOF(mFont)) return;

                }
            }
        }
    }

    virtual void VUnknown() override
    {
        LOG_INFO("??");
    }

    bool bLog = false;

private:


 
    void DumpObjects()
    {
        if ((GetKeyState(VK_F12) & 0x8000) || bLog)
        {
            LOG_INFO("==Dump start===");
            for (int i = 0; i < gBaseGameObject_list_9F2DF0->field_4_used_size; i++)
            {
                auto pObj = reinterpret_cast<BaseGameObject*>(gBaseGameObject_list_9F2DF0->field_0_array[i]);
                struct VTable
                {
                    DWORD addr;
                };

                VTable* pTable = reinterpret_cast<VTable*>(pObj);

                auto it = sVTableToName.find(pTable->addr);
                
                if (it != std::end(sVTableToName))
                {
                    //LOG_INFO("Add object with id: " << std::dec << pObj->field_4_typeId << " VTable: " << it->second);
                }
                else
                {
                    LOG_INFO("Add object with id: " << std::dec << pObj->field_4_typeId << " VTable: 0x" << std::hex << pTable->addr);
                }
            }
            LOG_INFO("==Dump end===");
            bLog = false;
        }
    }

};

static void TriggerObjectDump()
{
    if (gObjDumper)
    {
        gObjDumper->bLog = true;
    }
}

EXPORT BaseGameObject* BaseGameObject::ctor_487E10(__int16 arraySize)
{
    SetVTable(this, 0x4BD488); // vTable_BaseGameObject_4BD488

    if (!this) // Compiler code or hack
    {
        return this;
    }

    field_8_update_delay = 0;
    field_4_typeId = 0;
    field_C_bCanKill = 0;

    field_6_flags &= ~0x3F5u;
    field_6_flags |= 2;

    if (!arraySize)
    {
        TriggerObjectDump();
        return this;
    }

    if (!gBaseGameObject_list_9F2DF0->Push_Back_404450(this))
    {
        field_6_flags |= 1u;
    }

    TriggerObjectDump();
    return this;
}

EXPORT int CC Nop_49A750()
{
    LOG_INFO("Test");
    alive_new<ObjectDumper>();
    return 0;
}

