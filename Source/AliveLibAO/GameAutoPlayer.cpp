#include "GameAutoPlayer.hpp"
#include "Sys_common.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"
#include "Input.hpp"

namespace AO {
static s32 CountWithOutLoadingFiles()
{
    s32 ret = 0;
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        if (gBaseGameObjects->ItemAt(i)->Type() != ReliveTypes::eLoadingFile)
        {
            ret++;
        }
    }
    return ret;
}

void Recorder::SaveObjectStates()
{
    const u32 objCount = CountWithOutLoadingFiles();
    mFile.Write(objCount);

    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (pObj->Type() == ReliveTypes::eLoadingFile)
        {
            continue;
        }

        const s16 objType = static_cast<s16>(BaseGameObject::ToAO(pObj->mBaseGameObjectTypeId));
        ::fwrite(&objType, sizeof(s16), 1, mFile.GetFile());

        const u32 isBaseAliveGameObject = pObj->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6);
        mFile.Write(isBaseAliveGameObject);
        if (isBaseAliveGameObject)
        {
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);

            mFile.Write(pAliveObj->mXPos.fpValue);
            mFile.Write(pAliveObj->mYPos.fpValue);

            mFile.Write(pAliveObj->BaseAliveGameObjectLastLineYPos.fpValue);

            mFile.Write(pAliveObj->mCurrentMotion);
            mFile.Write(pAliveObj->mNextMotion);
            mFile.Write(pAliveObj->mPreviousMotion);

            mFile.Write(pAliveObj->mHealth.fpValue);
        }
    }
}

bool Player::ValidateBaseAliveGameObject(BaseGameObject* pObj)
{
    const u32 isBaseAliveGameObject = mFile.ReadU32();
    if (isBaseAliveGameObject)
    {
        if (!pObj || !pObj->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            if (pObj)
            {
                LOG_WARNING("Object is not base alive game object, skip");
            }
            SkipValidField<decltype(BaseAliveGameObject::mXPos)>(mFile);
            SkipValidField<decltype(BaseAliveGameObject::mYPos)>(mFile);
            SkipValidField<decltype(BaseAliveGameObject::BaseAliveGameObjectLastLineYPos)>(mFile);
            SkipValidField<decltype(BaseAliveGameObject::mCurrentMotion)>(mFile);
            SkipValidField<decltype(BaseAliveGameObject::mNextMotion)>(mFile);
            SkipValidField<decltype(BaseAliveGameObject::mPreviousMotion)>(mFile);
            SkipValidField<decltype(BaseAliveGameObject::mHealth)>(mFile);
            return false;
        }
        else
        {
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            bool validateFailed = false;
            validateFailed |= ValidField(mFile, pAliveObj->mXPos, "xpos");
            validateFailed |= ValidField(mFile, pAliveObj->mYPos, "ypos");
            validateFailed |= ValidField(mFile, pAliveObj->BaseAliveGameObjectLastLineYPos, "last line ypos");
            validateFailed |= ValidField(mFile, pAliveObj->mCurrentMotion, "current motion");
            validateFailed |= ValidField(mFile, pAliveObj->mNextMotion, "next motion");
            validateFailed |= ValidField(mFile, pAliveObj->mPreviousMotion, "previous motion");
            validateFailed |= ValidField(mFile, pAliveObj->mHealth, "health");
            if (validateFailed)
            {
                return false;
            }
        }
    }
    return true;
}

bool Player::ValidateObjectStates()
{
    u32 objCount = 0;
    mFile.Read(objCount);

    bool validateFailed = false;
    if (CountWithOutLoadingFiles() != static_cast<s32>(objCount))
    {
        LOG_ERROR("Got " << CountWithOutLoadingFiles() << " objects but expected " << objCount);
        validateFailed = true;
        // TODO: This can be smarter and try to validate the list until the obj types no longer match
        for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
        {
            BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
            // Skip loading files
            if (pObj->Type() == ReliveTypes::eLoadingFile)
            {
                continue;
            }

            s16 objType = 0;
            mFile.Read(objType);
            // Convert to relive type
            ReliveTypes reliveObjType = BaseGameObject::FromAO(static_cast<AOTypes>(objType));

            if (pObj->mBaseGameObjectTypeId != reliveObjType)
            {
                LOG_ERROR("Got " << static_cast<s16>(BaseGameObject::ToAO(pObj->mBaseGameObjectTypeId)) << " type but expected " << objType);
            }
            ValidateBaseAliveGameObject(nullptr);
        }

        if (CountWithOutLoadingFiles() > static_cast<s32>(objCount))
        {
            // What extra stuff is knocking about?
            for (s32 i = objCount; i < gBaseGameObjects->Size(); i++)
            {
                const BaseGameObject* pExtraObj = gBaseGameObjects->ItemAt(i);
                // Skip loading files
                if (pExtraObj->Type() == ReliveTypes::eLoadingFile)
                {
                    continue;
                }
                const s32 aoType = static_cast<s32>(BaseGameObject::ToAO(pExtraObj->Type()));
                LOG_INFO("Extra obj type is : " << aoType);
            }
        }
    }
    else
    {
        for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
        {
            BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);

            // Skip loading files
            if (pObj->Type() == ReliveTypes::eLoadingFile)
            {
                continue;
            }

            s16 objType = 0;
            mFile.Read(objType);

            // Convert to relive type
            ReliveTypes reliveObjType = BaseGameObject::FromAO(static_cast<AOTypes>(objType));

            if (pObj->mBaseGameObjectTypeId != reliveObjType)
            {
                LOG_ERROR("Got " << static_cast<s16>(BaseGameObject::ToAO(pObj->mBaseGameObjectTypeId)) << " type but expected " << objType);
                validateFailed |= true;
            }
            if (!ValidateBaseAliveGameObject(pObj))
            {
                validateFailed |= true;
            }
        }
    }

    if (validateFailed)
    {
        return false;
    }

    return true;
}

u32 GameAutoPlayer::ReadInput(u32 padIdx)
{
    return Input().Input_Read_Pad(padIdx);
}

} // namespace AO
