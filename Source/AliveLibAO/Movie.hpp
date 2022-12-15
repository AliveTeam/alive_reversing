#pragma once

#include "../relive_lib/BaseGameObject.hpp"

namespace AO {

class Movie final : public ::BaseGameObject
{
public:
    explicit Movie(const char_type* pFmvName);
    ~Movie();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

private:
    const char_type* mFmvName = nullptr;
};

extern s32 gMovieRefCount;

} // namespace AO
