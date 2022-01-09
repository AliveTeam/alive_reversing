#include "GameAutoPlayer.hpp"
#include "Input.hpp"
#include "Sys_common.hpp"
#include "BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"

constexpr u32 kVersion = 0x1997;

static bool ExtractNamePairArgument(char* pOutArgument, const char* pCmdLine, const char* argumentPrefix)
{
    const char* pArg = strstr(pCmdLine, argumentPrefix);
    if (!pArg)
    {
        return false;
    }

    pArg += strlen(argumentPrefix);

    u32 len = 0;
    const char* pArgIter = pArg;
    while (*pArgIter && *pArgIter != ' ')
    {
        len++;
        pArgIter++;
    }

    if (len == 0 || len >= 254)
    {
        return false;
    }

    memcpy(pOutArgument, pArg, len);
    return true;
}

void Recorder::Init(const char* pFileName)
{
    LOG_INFO("Recording to " << pFileName);
    if (!mFile.Open(pFileName, "wb"))
    {
        ALIVE_FATAL("Can't open recording file for writing");
    }

    // Write header
    mFile.Write(kVersion);
}

void Recorder::SaveInput(const Pads& data)
{
    mFile.Write(data);
}

void Recorder::SaveObjectStates()
{
    const u32 objCount = gBaseGameObject_list_BB47C4->Size();
    mFile.Write(objCount);

    for (u32 i = 0; i < objCount; i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        const s16 objType = static_cast<s16>(pObj->Type());
        ::fwrite(&objType, sizeof(s16), 1, mFile.GetFile());

        if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
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


void Player::Init(const char* pFileName)
{
    LOG_INFO("Playing from " << pFileName);
    if (!mFile.Open(pFileName, "rb"))
    {
        ALIVE_FATAL("Can't open play back file for reading");
    }

    // Read + validate header
    u32 readVersion = 0;
    mFile.Read(readVersion);
    if (readVersion != kVersion)
    {
        ALIVE_FATAL("File version too old, new or corrupted data");
    }
}

Pads Player::ReadInput()
{
    Pads data = {};
    mFile.Read(data);
    return data;
}

template<typename TypeToValidate>
static void ValidField(AutoFILE& file, const TypeToValidate& expectedValue, const char* name)
{
    TypeToValidate tmpValue = {};
    file.Read(tmpValue);
    if (tmpValue != expectedValue)
    {
        LOG_ERROR("Field " << name << " de-synced");
        ALIVE_FATAL("Field value de-sync");
    }
}

void Player::ValidateObjectStates()
{
    u32 objCount = 0;
    mFile.Read(objCount);

    if (static_cast<u32>(gBaseGameObject_list_BB47C4->Size()) != objCount)
    {
        LOG_ERROR("Got " << gBaseGameObject_list_BB47C4->Size() << " objects but expected " << objCount);
        ALIVE_FATAL("Object count de-sync");
    }

    for (u32 i = 0; i < objCount; i++)
    {
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

void GameAutoPlayer::ParseCommandLine(const char* pCmdLine)
{
    char buffer[256] = {};
    if (ExtractNamePairArgument(buffer, pCmdLine, "-record="))
    {
        mRecorder.Init(buffer);
        mMode = Mode::Record;
    }
    else if (ExtractNamePairArgument(buffer, pCmdLine, "-play="))
    {
        mPlayer.Init(buffer);
        mMode = Mode::Play;
    }
}

u32 GameAutoPlayer::GetInput(u32 padIdx)
{
    if (mMode == Mode::Play)
    {
        Pads data = mPlayer.ReadInput();
        return data.mPads[padIdx];
    }
    else
    {
        Pads data = {};
        data.mPads[padIdx] = Input_Read_Pad_4FA9C0(padIdx);
        if (mMode == Mode::Record)
        {
            mRecorder.SaveInput(data);
        }
        return data.mPads[padIdx];
    }
}

void GameAutoPlayer::ValidateObjectStates()
{
    if (mMode == Mode::Play)
    {
        mPlayer.ValidateObjectStates();
    }
    else if (mMode == Mode::Record)
    {
        mRecorder.SaveObjectStates();
    }
}

GameAutoPlayer gGameAutoPlayer;
