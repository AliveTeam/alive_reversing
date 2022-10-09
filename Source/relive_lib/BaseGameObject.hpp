#pragma once

#include "../AliveLibCommon/Types.hpp"
#include "../AliveLibCommon/BitField.hpp"
#include "DynamicArray.hpp"
#include "../relive_lib/data_conversion/guid.hpp"
#include "../relive_lib/ResourceManagerWrapper.hpp"
#include "../relive_lib/ObjectTypes.hpp"

extern u32 sGnFrame;

struct PrimHeader;

class BaseGameObject
{
public:
    enum Options
    {
        eListAddFailed_Bit1 = 0x001,
        eUpdatable_Bit2 = 0x002,
        eDead = 0x004,
        eDrawable_Bit4 = 0x008,
        eIsBaseAnimatedWithPhysicsObj_Bit5 = 0x010,
        eIsBaseAliveGameObject_Bit6 = 0x020,
        eCanExplode_Bit7 = 0x040,
        eInteractive_Bit8 = 0x080,
        eSurviveDeathReset_Bit9 = 0x100,
        eUpdateDuringCamSwap_Bit10 = 0x200,
        eCantKill_Bit11 = 0x400
    };

    BaseGameObject(s16 bAddToObjectList, s16 resourceArraySize);


    virtual ~BaseGameObject();

    virtual void VUpdate();

    virtual void VRender(PrimHeader** pOrderingTable);

    virtual void VScreenChanged();

    virtual void VStopAudio();

    virtual s32 VGetSaveState(u8* /*pSaveBuffer*/);

    static ReliveTypes FromAO(AO::AOTypes aoType);
    static AO::AOTypes ToAO(ReliveTypes reliveType);

    static ReliveTypes FromAE(AETypes aeType);

    static AETypes ToAE(ReliveTypes reliveType);

    void SetType(ReliveTypes type)
    {
        mBaseGameObjectTypeId = type;
    }

    ReliveTypes Type() const
    {
        return mBaseGameObjectTypeId;
    }

protected:
    u8** Add_Resource(u32 type, s32 resourceID);

    static Guid RefreshId(const Guid& objectId);

    // Helper to check if a timer has expired
    template <class T>
    static inline bool Expired(const T& value)
    {
        return static_cast<s32>(sGnFrame) > value;
    }

    template <class T>
    static s32 MakeTimer(const T value)
    {
        return static_cast<s32>(sGnFrame) + value;
    }

public:
    void SetUpdateDelay(s32 updateDelay)
    {
        mBaseGameObjectUpdateDelay = updateDelay;
    }

    s32 UpdateDelay() const
    {
        return mBaseGameObjectUpdateDelay;
    }

    AnimResource& GetAnimRes(AnimId animId)
    {
        for (u32 i = 0; i < mLoadedAnims.size(); i++)
        {
            if (mLoadedAnims[i].mId == animId)
            {
                return mLoadedAnims[i];
            }
        }
        LOG_ERROR("Anim res " << static_cast<s32>(animId) << " not found");
        ALIVE_FATAL("Anim res not found");
    }

    PalResource& GetPalRes(PalId palId)
    {
        for (u32 i = 0; i < mLoadedPals.size(); i++)
        {
            if (mLoadedPals[i].mId == palId)
            {
                return mLoadedPals[i];
            }
        }
        LOG_ERROR("Pal res " << static_cast<s32>(palId) << " not found");
        ALIVE_FATAL("Pal res not found");
    }

public:
    BitField16<Options> mBaseGameObjectFlags = {};

    Guid mBaseGameObjectId;                  // AE
    Guid mBaseGameObjectTlvInfo;                 // AE
    DynamicArrayT<u8*> field_10_resources_array; // AE
    std::vector<AnimResource> mLoadedAnims;
    std::vector<PalResource> mLoadedPals;

    s8 mBaseGameObjectRefCount = 0; // AO

private:
    ReliveTypes mBaseGameObjectTypeId = ReliveTypes::eNone;
    s32 mBaseGameObjectUpdateDelay = 0;

};

class IMap& GetMap();

extern DynamicArrayT<BaseGameObject>* gBaseGameObjects;
