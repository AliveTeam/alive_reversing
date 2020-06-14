#include "stdafx_ao.h"
#include "GameSpeak.hpp"
#include "Function.hpp"

START_NS_AO

GameSpeak* GameSpeak::ctor_40F990()
{
    NOT_IMPLEMENTED();
    return this;
}

GameSpeak* GameSpeak::Vdtor_40FB50(signed int /*flags*/)
{
    NOT_IMPLEMENTED();
    return this;
}

BaseGameObject* GameSpeak::VDestructor(signed int flags)
{
    return Vdtor_40FB50(flags);
}

END_NS_AO
