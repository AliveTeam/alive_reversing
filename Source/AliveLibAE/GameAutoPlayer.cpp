#include "GameAutoPlayer.hpp"
#include "Input.hpp"
#include "Sys_common.hpp"
#include "BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"

void Recorder::SaveObjectStates()
{
    mFile.Write(RecordTypes::FrameCounter);
    mFile.Write(sGnFrame_5C1B84);

    const u32 objCount = gBaseGameObjects->Size();
    mFile.Write(RecordTypes::ObjectCounter);
    mFile.Write(objCount);

    for (u32 i = 0; i < objCount; i++)
    {
        mFile.Write(RecordTypes::ObjectStates);
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        const s16 objType = static_cast<s16>(pObj->Type());
        ::fwrite(&objType, sizeof(s16), 1, mFile.GetFile());

        if (pObj->mFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            mFile.Write(RecordTypes::AliveObjectStates);

            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);

            mFile.Write(pAliveObj->field_B8_xpos.fpValue);
            mFile.Write(pAliveObj->field_BC_ypos.fpValue);

            mFile.Write(pAliveObj->field_F8_LastLineYPos.fpValue);

            mFile.Write(pAliveObj->field_106_current_motion);
            mFile.Write(pAliveObj->field_108_next_motion);
            mFile.Write(pAliveObj->field_F4_previous_motion);

            mFile.Write(pAliveObj->field_10C_health.fpValue);
        }
    }
}

bool Player::ValidateObjectStates()
{
    ValidateNextTypeIs(RecordTypes::FrameCounter);

    const u32 gnFrame = mFile.ReadU32();
    if (gnFrame != sGnFrame_5C1B84)
    {
        return false;
    }

    ValidateNextTypeIs(RecordTypes::ObjectCounter);
    const u32 objCount = mFile.ReadU32();

    if (static_cast<u32>(gBaseGameObjects->Size()) != objCount)
    {
        LOG_ERROR("Got " << gBaseGameObjects->Size() << " objects but expected " << objCount);
        return false;
    }

    for (u32 i = 0; i < objCount; i++)
    {
        ValidateNextTypeIs(RecordTypes::ObjectStates);

        s16 objType = 0;
        mFile.Read(objType);

        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (static_cast<s16>(pObj->Type()) != objType)
        {
            LOG_ERROR("Got " << static_cast<s16>(pObj->Type()) << " type but expected " << objType);
            return false;
        }

        if (pObj->mFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            ValidateNextTypeIs(RecordTypes::AliveObjectStates);

            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            bool validateFailed = false;
            validateFailed |= ValidField(mFile, pAliveObj->field_B8_xpos, "xpos");
            validateFailed |= ValidField(mFile, pAliveObj->field_BC_ypos, "ypos");
            validateFailed |= ValidField(mFile, pAliveObj->field_F8_LastLineYPos, "last line ypos");
            validateFailed |= ValidField(mFile, pAliveObj->field_106_current_motion, "current motion");
            validateFailed |= ValidField(mFile, pAliveObj->field_108_next_motion, "next motion");
            validateFailed |= ValidField(mFile, pAliveObj->field_F4_previous_motion, "previous motion");
            validateFailed |= ValidField(mFile, pAliveObj->field_10C_health, "health motion");
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
    return Input_Read_Pad_4FA9C0(padIdx);
}
