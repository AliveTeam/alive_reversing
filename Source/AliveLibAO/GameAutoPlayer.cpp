#include "stdafx_ao.h"
#include "GameAutoPlayer.hpp"
#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"
#include "Input.hpp"
#include <algorithm>

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

        const s16 objType = static_cast<s16>(BaseGameObject::ToAO(pObj->Type()));
        ::fwrite(&objType, sizeof(s16), 1, mFile.GetFile());

        const u32 isBaseAliveGameObject = pObj->GetIsBaseAliveGameObject();
        mFile.Write(isBaseAliveGameObject);
        if (isBaseAliveGameObject)
        {
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);

            mFile.Write(pAliveObj->mXPos.fpValue);
            mFile.Write(pAliveObj->mYPos.fpValue);

            mFile.Write(pAliveObj->BaseAliveGameObjectLastLineYPos.fpValue);

            mFile.Write(pAliveObj->VGetMotion(eMotionType::eCurrentMotion));
            mFile.Write(pAliveObj->VGetMotion(eMotionType::eNextMotion));
            mFile.Write(pAliveObj->VGetMotion(eMotionType::ePreviousMotion));

            mFile.Write(pAliveObj->mHealth.fpValue);
        }
    }
}

bool Player::ValidateBaseAliveGameObject(BaseGameObject* pObj)
{
    const u32 isBaseAliveGameObject = mFile.ReadU32();
    if (isBaseAliveGameObject)
    {
        if (!pObj || !pObj->GetIsBaseAliveGameObject())
        {
            if (pObj)
            {
                LOG_WARNING("Object is not base alive game object, skip");
            }
            SkipValidField<decltype(BaseAliveGameObject::mXPos)>(mFile);
            SkipValidField<decltype(BaseAliveGameObject::mYPos)>(mFile);
            SkipValidField<decltype(BaseAliveGameObject::BaseAliveGameObjectLastLineYPos)>(mFile);
            SkipValidField<s16>(mFile); // current motion
            SkipValidField<s16>(mFile); // next motion
            SkipValidField<s16>(mFile); // previous motion
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
            validateFailed |= ValidField(mFile, pAliveObj->VGetMotion(eMotionType::eCurrentMotion), "current motion");
            validateFailed |= ValidField(mFile, pAliveObj->VGetMotion(eMotionType::eNextMotion), "next motion");
            validateFailed |= ValidField(mFile, pAliveObj->VGetMotion(eMotionType::ePreviousMotion), "previous motion");
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
        LOG_ERROR("Got %d objects but expected %d", CountWithOutLoadingFiles(), objCount);
        validateFailed = true;
        // TODO: This can be smarter and try to validate the list until the obj types no longer match
        for (s32 i = 0; i < std::min(static_cast<s32>(objCount), gBaseGameObjects->Size()); i++)
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

            if (pObj->Type() != reliveObjType)
            {
                LOG_ERROR("Got %d type but expected %d", static_cast<s16>(BaseGameObject::ToAO(pObj->Type())), objType);
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
                LOG_INFO("Extra obj type is : %d", aoType);
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

            if (pObj->Type() != reliveObjType)
            {
                LOG_ERROR("Got %d type but expected %d", static_cast<s16>(BaseGameObject::ToAO(pObj->Type())), objType);
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
