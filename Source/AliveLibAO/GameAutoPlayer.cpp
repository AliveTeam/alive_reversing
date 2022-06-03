#include "GameAutoPlayer.hpp"
#include "Sys_common.hpp"
#include "BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"
#include "Input.hpp"

namespace AO {
void Recorder::SaveObjectStates()
{
    const u32 objCount = gBaseGameObjects->Size();
    mFile.Write(objCount);

    for (u32 i = 0; i < objCount; i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        const s16 objType = static_cast<s16>(pObj->mTypeId);
        ::fwrite(&objType, sizeof(s16), 1, mFile.GetFile());

        const u32 isBaseAliveGameObject = pObj->mGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6);
        mFile.Write(isBaseAliveGameObject);
        if (isBaseAliveGameObject)
        {
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);

            mFile.Write(pAliveObj->mXPos.fpValue);
            mFile.Write(pAliveObj->mYPos.fpValue);

            mFile.Write(pAliveObj->mLastLineYPos.fpValue);

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
        if (!pObj || !pObj->mGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            if (pObj)
            {
                LOG_WARNING("Object is not base alive game object, skip");
            }
            SkipValidField<decltype(BaseAliveGameObject::mXPos)>(mFile);
            SkipValidField<decltype(BaseAliveGameObject::mYPos)>(mFile);
            SkipValidField<decltype(BaseAliveGameObject::mLastLineYPos)>(mFile);
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
            validateFailed |= ValidField(mFile, pAliveObj->mLastLineYPos, "last line ypos");
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
    if (static_cast<u32>(gBaseGameObjects->Size()) != objCount)
    {
        LOG_ERROR("Got " << gBaseGameObjects->Size() << " objects but expected " << objCount);
        validateFailed = true;
        // TODO: This can be smarter and try to validate the list until the obj types no longer match
        for (u32 i = 0; i < objCount; i++)
        {
            s16 objType = 0;
            mFile.Read(objType);
            ValidateBaseAliveGameObject(nullptr);
        }
        return validateFailed;
    }

    for (u32 i = 0; i < objCount; i++)
    {
        s16 objType = 0;
        mFile.Read(objType);

        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (static_cast<s16>(pObj->mTypeId) != objType)
        {
            LOG_ERROR("Got " << static_cast<s16>(pObj->mTypeId) << " type but expected " << objType);
            validateFailed |= true;
        }
        if (!ValidateBaseAliveGameObject(pObj))
        {
            validateFailed |= true;
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
