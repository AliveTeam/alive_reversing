#include "stdafx_ao.h"
#include "Sfx.hpp"
#include "Function.hpp"

START_NS_AO
 
int CC SFX_Play_real_43AE60(unsigned __int8 /*sfxId*/, int /*volume*/, int /*pitch*/, BaseAnimatedWithPhysicsGameObject* /*pObj*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

int CC SFX_Play_real_43AD70(unsigned __int8 /*sfxId*/, int /*a2*/, BaseGameObject* /*a3*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

int CC SFX_Play_real_43ADE0(unsigned __int8 /*sfxId*/, int /*leftVol*/, int /*rightVol*/, BaseAliveGameObject* /*pObj*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

int CC SFX_Play_real_43AED0(unsigned __int8 /*sfxId*/, signed int /*volume*/, CameraPos /*direction*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

int unknownSfxLogger(unsigned __int8 sfxId)
{
    static bool knownSounds[112] = {};
    static bool setThem = false;

    if (!setThem)
    {
        knownSounds[3] = true;
        knownSounds[4] = true;
        knownSounds[5] = true;
        knownSounds[8] = true;
        knownSounds[16] = true;
        knownSounds[21] = true;
        knownSounds[24] = true;
        knownSounds[25] = true;
        knownSounds[27] = true;
        knownSounds[28] = true;
        knownSounds[33] = true;
        knownSounds[35] = true;
        knownSounds[37] = true;
        knownSounds[43] = true;
        knownSounds[47] = true;
        knownSounds[48] = true;
        knownSounds[56] = true;
        knownSounds[58] = true;
        knownSounds[61] = true;
        knownSounds[78] = true;
        knownSounds[82] = true;
        knownSounds[85] = true;
        knownSounds[86] = true;
        knownSounds[87] = true;
        knownSounds[89] = true;
        knownSounds[90] = true;
        knownSounds[91] = true;
        knownSounds[94] = true;
        knownSounds[96] = true;
        knownSounds[98] = true;
        knownSounds[99] = true;
        knownSounds[100] = true;
        knownSounds[101] = true;
        knownSounds[102] = true;
        knownSounds[103] = true;
        knownSounds[104] = true;
        knownSounds[107] = true;
        knownSounds[110] = true;

        setThem = true;
    }

    if (!knownSounds[sfxId])
    {
        LOG_INFO("sfx id: " << static_cast<int>(sfxId));
        return true;
    }
    return false;
}


int CC SFX_Play_43AE60(unsigned __int8 sfxId, int volume, int pitch, BaseAnimatedWithPhysicsGameObject* pObj)
{
    unknownSfxLogger(sfxId);
    return SFX_Play_real_43AE60(sfxId, volume, pitch, pObj);
}

int CC SFX_Play_43AD70(unsigned __int8 sfxId, int a2, BaseGameObject* a3)
{
    unknownSfxLogger(sfxId);
    return SFX_Play_real_43AD70(sfxId, a2, a3);
}

int CC SFX_Play_43ADE0(unsigned __int8 sfxId, int leftVol, int rightVol, BaseAliveGameObject* pObj)
{
    unknownSfxLogger(sfxId);
    return SFX_Play_real_43ADE0(sfxId, leftVol, rightVol, pObj);
}

int CC SFX_Play_43AED0(unsigned __int8 sfxId, signed int volume, CameraPos direction)
{
    unknownSfxLogger(sfxId);
    return SFX_Play_real_43AED0(sfxId, volume, direction);
}

END_NS_AO
