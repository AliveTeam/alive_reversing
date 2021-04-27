#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

class BackgroundMusic : public BaseGameObject
{
public:
    EXPORT static void CC Stop_4CB000();
    EXPORT static void CC Play_4CB030();

    EXPORT BackgroundMusic* ctor_4CB110(s16 musicId);

    virtual BaseGameObject* VDestructor(s32 flags) override;
    EXPORT BaseGameObject* vdtor_4CB150(s32 flags);
    EXPORT void dtor_4CB180();

    virtual void VUpdate() override;
    EXPORT void Update_4CB1A0();

private:
    s16 field_20_music_id;
    // s16 field_22_padding;
};
ALIVE_ASSERT_SIZEOF(BackgroundMusic, 0x24);
