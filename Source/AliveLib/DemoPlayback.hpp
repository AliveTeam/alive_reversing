#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"

class DemoPlayback : public BaseGameObject
{
public:
    // NOTE: Inlined in real game at 0x4D6990
    void ctor();

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    EXPORT void dtor_4978B0();
    EXPORT DemoPlayback* vdtor_497880(signed int flags);
    EXPORT void vUpdate_4978E0();
};
ALIVE_ASSERT_SIZEOF(DemoPlayback, 0x20);

ALIVE_VAR_EXTERN(DemoPlayback*, sDemoObj_dword_5D1E20);
ALIVE_VAR_EXTERN(short, word_5C1B9C);
