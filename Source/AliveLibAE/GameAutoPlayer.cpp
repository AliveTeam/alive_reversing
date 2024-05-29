#include "stdafx.h"
#include "GameAutoPlayer.hpp"
#include "Input.hpp"
#include "../relive_lib/GameObjects/BaseAliveGameObject.hpp"
#include <algorithm>
#include "Mudokon.hpp"
#include "Slig.hpp"


void Recorder::SaveObjectStates()
{
    mFile.Write(RecordTypes::FrameCounter);
    mFile.Write(sGnFrame);

    const u32 objCount = gBaseGameObjects->Size();
    mFile.Write(RecordTypes::ObjectCounter);
    mFile.Write(objCount);

    for (u32 i = 0; i < objCount; i++)
    {
        mFile.Write(RecordTypes::ObjectStates);
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        const s16 objType = static_cast<s16>(BaseGameObject::ToAE(pObj->Type()));
        ::fwrite(&objType, sizeof(s16), 1, mFile.GetFile());

        if (pObj->GetIsBaseAliveGameObject())
        {
            mFile.Write(RecordTypes::AliveObjectStates);

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

bool Player::ValidateObjectStates()
{
    ValidateNextTypeIs(RecordTypes::FrameCounter);

    const u32 gnFrame = mFile.ReadU32();
    if (gnFrame != sGnFrame)
    {
        return false;
    }

    ValidateNextTypeIs(RecordTypes::ObjectCounter);
    const u32 objCount = mFile.ReadU32();

    if (static_cast<u32>(gBaseGameObjects->Size()) != objCount)
    {
        for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
        {
            auto pObj = gBaseGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }
            LOG_INFO("object at %d with type %d", i, (s32)pObj->Type());
        }
        LOG_ERROR("Got %d objects but expected %d on gnframe %d", gBaseGameObjects->Size(), objCount, sGnFrame);
    }

    for (u32 i = 0; i < std::min(objCount, static_cast<u32>(gBaseGameObjects->Size())); i++)
    {
        ValidateNextTypeIs(RecordTypes::ObjectStates);

        s16 objType = 0;
        mFile.Read(objType);

        ReliveTypes reliveObjType = BaseGameObject::FromAE(static_cast<AETypes>(objType));

        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);

        // hack fix - OG forgot to set the type of the throwable array in the ctor but it gets
        // set in the VGetSaveState function so we assume that this is the case here to avoid desyncing.
        if (pObj->Type() == ReliveTypes::eThrowableArray && reliveObjType == ReliveTypes::eNone)
        {
            continue;
        }

        if (pObj->Type() != reliveObjType)
        {
            LOG_ERROR("Got %d type but expected %d", static_cast<s16>(BaseGameObject::ToAE(pObj->Type())), objType);
            return false;
        }

        if (pObj->GetIsBaseAliveGameObject())
        {
            ValidateNextTypeIs(RecordTypes::AliveObjectStates);

            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            bool validateFailed = false;
            validateFailed |= ValidField(mFile, pAliveObj->mXPos, "xpos");
            validateFailed |= ValidField(mFile, pAliveObj->mYPos, "ypos");
            validateFailed |= ValidField(mFile, pAliveObj->BaseAliveGameObjectLastLineYPos, "last line ypos");
            validateFailed |= ValidField(mFile, pAliveObj->VGetMotion(eMotionType::eCurrentMotion), "current motion");
            validateFailed |= ValidField(mFile, pAliveObj->VGetMotion(eMotionType::eNextMotion), "next motion");
            validateFailed |= ValidField(mFile, pAliveObj->VGetMotion(eMotionType::ePreviousMotion), "previous motion");
            validateFailed |= ValidField(mFile, pAliveObj->mHealth, "health motion");
            if (validateFailed)
            {
                return false;
            }
        }
    }

    if (gBaseGameObjects->Size() != static_cast<s32>(objCount))
    {
        if (gBaseGameObjects->Size() < static_cast<s32>(objCount))
        {
            ValidateNextTypeIs(RecordTypes::ObjectStates);

            s16 objType = 0;
            mFile.Read(objType);

            ReliveTypes reliveObjType = BaseGameObject::FromAE(static_cast<AETypes>(objType));
            LOG_INFO("First extra object type is %d", static_cast<u32>(reliveObjType));
        }
        return false;
    }

    return true;
}

u32 GameAutoPlayer::ReadInput(u32 padIdx)
{
    return Input_Read_Pad(padIdx);
}
