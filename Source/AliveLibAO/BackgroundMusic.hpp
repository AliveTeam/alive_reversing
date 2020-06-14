#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"

START_NS_AO

class BackgroundMusic : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BackgroundMusic* Vdtor_476A80(signed int flags);

    EXPORT BackgroundMusic* ctor_476370(__int16 musicId);

    EXPORT static void CC Stop_476290();

    __int16 field_10_music_id;
    //__int16 field_12_padding;
};
ALIVE_ASSERT_SIZEOF(BackgroundMusic, 0x14);


END_NS_AO
