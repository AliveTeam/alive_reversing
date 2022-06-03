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
        const s16 objType = static_cast<s16>(pObj->mBaseGameObjectTypeId);
        ::fwrite(&objType, sizeof(s16), 1, mFile.GetFile());

        const u32 isBaseAliveGameObject = pObj->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6);
        mFile.Write(isBaseAliveGameObject);
        if (isBaseAliveGameObject)
        {
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);

            mFile.Write(pAliveObj->field_A8_xpos.fpValue);
            mFile.Write(pAliveObj->field_AC_ypos.fpValue);

            mFile.Write(pAliveObj->field_E8_LastLineYPos.fpValue);

            mFile.Write(pAliveObj->field_FC_current_motion);
            mFile.Write(pAliveObj->field_FE_next_motion);
            mFile.Write(pAliveObj->field_E4_previous_motion);

            mFile.Write(pAliveObj->field_100_health.fpValue);
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
            SkipValidField<decltype(BaseAliveGameObject::field_A8_xpos)>(mFile);
            SkipValidField<decltype(BaseAliveGameObject::field_AC_ypos)>(mFile);
            SkipValidField<decltype(BaseAliveGameObject::field_E8_LastLineYPos)>(mFile);
            SkipValidField<decltype(BaseAliveGameObject::field_FC_current_motion)>(mFile);
            SkipValidField<decltype(BaseAliveGameObject::field_FE_next_motion)>(mFile);
            SkipValidField<decltype(BaseAliveGameObject::field_E4_previous_motion)>(mFile);
            SkipValidField<decltype(BaseAliveGameObject::field_100_health)>(mFile);
            return false;
        }
        else
        {
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            bool validateFailed = false;
            validateFailed |= ValidField(mFile, pAliveObj->field_A8_xpos, "xpos");
            validateFailed |= ValidField(mFile, pAliveObj->field_AC_ypos, "ypos");
            validateFailed |= ValidField(mFile, pAliveObj->field_E8_LastLineYPos, "last line ypos");
            validateFailed |= ValidField(mFile, pAliveObj->field_FC_current_motion, "current motion");
            validateFailed |= ValidField(mFile, pAliveObj->field_FE_next_motion, "next motion");
            validateFailed |= ValidField(mFile, pAliveObj->field_E4_previous_motion, "previous motion");
            validateFailed |= ValidField(mFile, pAliveObj->field_100_health, "health");
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
        if (static_cast<s16>(pObj->mBaseGameObjectTypeId) != objType)
        {
            LOG_ERROR("Got " << static_cast<s16>(pObj->mBaseGameObjectTypeId) << " type but expected " << objType);
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
