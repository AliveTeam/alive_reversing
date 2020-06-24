#include "stdafx_ao.h"
#include "Function.hpp"
#include "PlatformBase.hpp"

START_NS_AO

void PlatformBase::VAdd(BaseAliveGameObject* pObj)
{
    VAdd_4515D0(pObj);
}

void PlatformBase::VRemove(BaseAliveGameObject* pObj)
{
    VRemove_451680(pObj);
}

void PlatformBase::VRemove_451680(BaseAliveGameObject* /*pObj*/)
{
    field_114_count--;
}

void PlatformBase::VAdd_4515D0(BaseAliveGameObject* /*pObj*/)
{
    NOT_IMPLEMENTED();
}

END_NS_AO

