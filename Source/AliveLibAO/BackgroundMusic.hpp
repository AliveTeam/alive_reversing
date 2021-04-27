#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"

namespace AO {

class BackgroundMusic : public BaseGameObject
{
public:
    EXPORT BackgroundMusic* ctor_476370(s16 musicId);

    EXPORT BaseGameObject* dtor_4763A0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT BackgroundMusic* Vdtor_476A80(s32 flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_4763B0();

    EXPORT static void CC Stop_476290();

    EXPORT static void CC Play_4762B0();

    s16 field_10_music_id;
    //s16 field_12_padding;
};
ALIVE_ASSERT_SIZEOF(BackgroundMusic, 0x14);

ALIVE_VAR_EXTERN(s16, sBackgroundMusic_seq_id_4CFFF8);

} // namespace AO
