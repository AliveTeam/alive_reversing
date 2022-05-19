#include "GameAutoPlayer.hpp"
#include "Sys_common.hpp"
#include "BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"
#include "Input.hpp"

namespace AO {
void Recorder::SaveObjectStates()
{
    const u32 objCount = gBaseGameObject_list_9F2DF0->Size();
    mFile.Write(objCount);

    for (u32 i = 0; i < objCount; i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_9F2DF0->ItemAt(i);
        const s16 objType = static_cast<s16>(pObj->field_4_typeId);
        ::fwrite(&objType, sizeof(s16), 1, mFile.GetFile());

        if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
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

bool Player::ValidateObjectStates()
{
    u32 objCount = 0;
    mFile.Read(objCount);

    if (static_cast<u32>(gBaseGameObject_list_9F2DF0->Size()) != objCount)
    {
        LOG_ERROR("Got " << gBaseGameObject_list_9F2DF0->Size() << " objects but expected " << objCount);
        return false;
    }

    for (u32 i = 0; i < objCount; i++)
    {
        s16 objType = 0;
        mFile.Read(objType);

        BaseGameObject* pObj = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (static_cast<s16>(pObj->field_4_typeId) != objType)
        {
            LOG_ERROR("Got " << static_cast<s16>(pObj->field_4_typeId) << " type but expected " << objType);
            return false;
        }

        if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            bool validateFailed = false;
            validateFailed |= ValidField(mFile, pAliveObj->field_A8_xpos, "xpos");
            validateFailed |= ValidField(mFile, pAliveObj->field_AC_ypos, "ypos");
            validateFailed |= ValidField(mFile, pAliveObj->field_E8_LastLineYPos, "last line ypos");
            validateFailed |= ValidField(mFile, pAliveObj->field_FC_current_motion, "current motion");
            validateFailed |= ValidField(mFile, pAliveObj->field_FE_next_motion, "next motion");
            validateFailed |= ValidField(mFile, pAliveObj->field_E4_previous_motion, "previous motion");
            validateFailed |= ValidField(mFile, pAliveObj->field_100_health, "health motion");
            if (validateFailed)
            {
                return false;
            }
        }
    }

    return true;
}

u32 GameAutoPlayer::ReadInput(u32 padIdx)
{
    return Input().Input_Read_Pad(padIdx);
}

} // namespace AO
