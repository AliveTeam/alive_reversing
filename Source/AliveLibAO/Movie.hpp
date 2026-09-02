#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"

namespace AO {

class Movie final : public ::BaseGameObject
{
public:
    explicit Movie(const char_type* pFmvName);
    ~Movie();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    static s32 gMovieRefCount;
private:
    const char_type* mFmvName = nullptr;
};


} // namespace AO
