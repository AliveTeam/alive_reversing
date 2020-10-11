#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"

START_NS_AO

class BackgroundMusic : public BaseGameObject
{
public:
    EXPORT BackgroundMusic* ctor_476370(__int16 musicId);

    EXPORT BaseGameObject* dtor_4763A0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BackgroundMusic* Vdtor_476A80(signed int flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_4763B0();

    EXPORT static void CC Stop_476290();

    EXPORT static void CC Play_4762B0();

    __int16 field_10_music_id;
    //__int16 field_12_padding;
};
ALIVE_ASSERT_SIZEOF(BackgroundMusic, 0x14);


END_NS_AO
