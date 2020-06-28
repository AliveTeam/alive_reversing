#include "stdafx_ao.h"
#include "Function.hpp"
#include "MotionDetector.hpp"
#include "stdlib.hpp"

START_NS_AO

void MotionDetector::SetDontComeBack_437E00(__int16 bDontComeBack)
{
    field_F6_bDontComeBack = bDontComeBack;
}

BaseGameObject* MotionDetector::dtor_437D70()
{
    SetVTable(this, 0x4BB878);

    if (field_F6_bDontComeBack)
    {
        gMap_507BA8.TLV_Reset_446870(field_E4_tlvInfo, -1, 0, 0);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_E4_tlvInfo, -1, 0, 1);
    }

    if (field_108_pLaser)
    {
        field_108_pLaser->field_C_refCount--;
        field_108_pLaser->field_6_flags.Set(Options::eDead_Bit3);
    }

    return dtor_417D10();
}

BaseGameObject* MotionDetector::VDestructor(signed int flags)
{
    return Vdtor_438530(flags);
}

MotionDetector* MotionDetector::Vdtor_438530(signed int flags)
{
    dtor_437D70();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }

    return this;
}

END_NS_AO
