#pragma once
#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Factory.hpp"

START_NS_AO;

class Elum : public BaseAliveGameObject
{
public:
    EXPORT BaseGameObject* Vdtor_411710(signed int flags);
    EXPORT void VUpdate_4102A0();
    EXPORT void VRender_410E40(int** pOrderingTable);
    EXPORT void vScreenChange_411340();

    EXPORT static void Elum::Spawn_410E90(TlvItemInfoUnion);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;
};
//ALIVE_ASSERT_SIZEOF(Elum, 0x30); todo

ALIVE_VAR_EXTERN(Elum*, gElum_507680);

END_NS_AO;
