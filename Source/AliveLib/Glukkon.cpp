#include "stdafx.h"
#include "Glukkon.hpp"
#include "Function.hpp"

Glukkon* Glukkon::ctor_43F030(Path_Glukkon* pTlv, int tlvInfo)
{
    ctor_408240(0);
    field_1EC = -1;
    SetVTable(this, 0x5452E0);

    field_1A8_tlvData = *pTlv;

    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_214_tlv_info = tlvInfo;

    // TODO: Resource IDs
    switch (field_1A8_tlvData.field_22_glukkon_type)
    {
    case GlukkonTypes::Normal_0:
    case GlukkonTypes::Normal_4:
    case GlukkonTypes::Normal_5:
        Add_Resource_4DC130(ResourceManager::Resource_Animation, 801);
        Animation_Init_424E10(
            169608,
            163,
            79,
            Add_Resource_4DC130(ResourceManager::Resource_Animation, 800),
            1,
            1);
        break;

    case GlukkonTypes::Aslik_1:
        Add_Resource_4DC130(ResourceManager::Resource_Animation, 803);
        Animation_Init_424E10(
            171356,
            157,
            76,
            Add_Resource_4DC130(ResourceManager::Resource_Animation, 802),
            1,
            1);
        break;

    case GlukkonTypes::Drpik_2:
        Add_Resource_4DC130(ResourceManager::Resource_Animation, 805);
        Animation_Init_424E10(
            193064,
            162,
            78u,
            Add_Resource_4DC130(ResourceManager::Resource_Animation, 804),
            1,
            1);
        break;

    case GlukkonTypes::Phleg_3:
        Add_Resource_4DC130(ResourceManager::Resource_Animation, 807);
        Animation_Init_424E10(
            199500,
            145,
            72,
            Add_Resource_4DC130(ResourceManager::Resource_Animation, 806),
            1,
            1);
        break;
    default:
        break;
    }
    
    Init_43F260();

    return this;
}

void Glukkon::Init_43F260()
{
    NOT_IMPLEMENTED();
}
