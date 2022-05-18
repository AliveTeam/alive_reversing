#include "GameAutoPlayer.hpp"
#include "Input.hpp"
#include "Sys_common.hpp"
#include "BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"

void Recorder::SaveObjectStates()
{
    mFile.Write(RecordTypes::FrameCounter);
    mFile.Write(sGnFrame_5C1B84);

    const u32 objCount = gBaseGameObject_list_BB47C4->Size();
    mFile.Write(RecordTypes::ObjectCounter);
    mFile.Write(objCount);

    for (u32 i = 0; i < objCount; i++)
    {
        mFile.Write(RecordTypes::ObjectStates);
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        const s16 objType = static_cast<s16>(pObj->Type());
        ::fwrite(&objType, sizeof(s16), 1, mFile.GetFile());

        if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
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

void Player::ValidateObjectStates()
{
    ValidateNextTypeIs(RecordTypes::FrameCounter);

    const u32 gnFrame = mFile.ReadU32();
    if (gnFrame != sGnFrame_5C1B84)
    {
        ALIVE_FATAL("GnFrame de-sync");
    }

    ValidateNextTypeIs(RecordTypes::ObjectCounter);
    const u32 objCount = mFile.ReadU32();

    if (static_cast<u32>(gBaseGameObject_list_BB47C4->Size()) != objCount)
    {
        LOG_ERROR("Got " << gBaseGameObject_list_BB47C4->Size() << " objects but expected " << objCount);
        ALIVE_FATAL("Object count de-sync");
    }

    for (u32 i = 0; i < objCount; i++)
    {
        ValidateNextTypeIs(RecordTypes::ObjectStates);

        s16 objType = 0;
        mFile.Read(objType);

        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (static_cast<s16>(pObj->Type()) != objType)
        {
            LOG_ERROR("Got " << static_cast<s16>(pObj->Type()) << " type but expected " << objType);
            ALIVE_FATAL("Object type de-sync");
        }

        if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            ValidateNextTypeIs(RecordTypes::AliveObjectStates);

            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            ValidField(mFile, pAliveObj->field_B8_xpos, "xpos");
            ValidField(mFile, pAliveObj->field_BC_ypos, "ypos");
            ValidField(mFile, pAliveObj->field_F8_LastLineYPos, "last line ypos");
            ValidField(mFile, pAliveObj->field_106_current_motion, "current motion");
            ValidField(mFile, pAliveObj->field_108_next_motion, "next motion");
            ValidField(mFile, pAliveObj->field_F4_previous_motion, "previous motion");
            ValidField(mFile, pAliveObj->field_10C_health, "health motion");
        }
    }
}

u32 GameAutoPlayer::ReadInput(u32 padIdx)
{
    return Input_Read_Pad_4FA9C0(padIdx);
}
