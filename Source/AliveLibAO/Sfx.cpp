#include "stdafx_ao.h"
#include "Sfx.hpp"
#include "Function.hpp"

namespace AO {
 
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
    if (sfxId == 0 || sfxId == 10 || sfxId == 11)
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

}
