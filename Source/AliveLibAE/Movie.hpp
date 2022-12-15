#pragma once

#include "../relive_lib/BaseGameObject.hpp"

struct CdlLOC;

s8 DDV_Play(const char_type* pDDVName);
bool AreMovieSkippingInputsHeld();

class Movie final : public BaseGameObject
{
public:
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    explicit Movie(const char_type* pName);

private:
    void Init();
    void DeInit();

    const char_type* mName = nullptr;
};

extern s32 gMovieRefCount;
