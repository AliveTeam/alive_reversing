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

void Player::ValidateObjectStates()
{
    u32 objCount = 0;
    mFile.Read(objCount);

    if (static_cast<u32>(gBaseGameObject_list_9F2DF0->Size()) != objCount)
    {
        LOG_ERROR("Got " << gBaseGameObject_list_9F2DF0->Size() << " objects but expected " << objCount);
        ALIVE_FATAL("Object count de-sync");
    }

    for (u32 i = 0; i < objCount; i++)
    {
        s16 objType = 0;
        mFile.Read(objType);

        BaseGameObject* pObj = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (static_cast<s16>(pObj->field_4_typeId) != objType)
        {
            LOG_ERROR("Got " << static_cast<s16>(pObj->field_4_typeId) << " type but expected " << objType);
            ALIVE_FATAL("Object type de-sync");
        }

        if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            ValidField(mFile, pAliveObj->field_A8_xpos, "xpos");
            ValidField(mFile, pAliveObj->field_AC_ypos, "ypos");
            ValidField(mFile, pAliveObj->field_E8_LastLineYPos, "last line ypos");
            ValidField(mFile, pAliveObj->field_FC_current_motion, "current motion");
            ValidField(mFile, pAliveObj->field_FE_next_motion, "next motion");
            ValidField(mFile, pAliveObj->field_E4_previous_motion, "previous motion");
            ValidField(mFile, pAliveObj->field_100_health, "health motion");
        }
    }
}

u32 GameAutoPlayer::ReadInput(u32 padIdx)
{
    return Input().Input_Read_Pad(padIdx);
}

} // namespace AO
