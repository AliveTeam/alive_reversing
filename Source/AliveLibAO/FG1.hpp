#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/MapWrapper.hpp"
#include "../AliveLibCommon/Primitives_common.hpp"

struct Fg1Chunk;
enum class EReliveLevelIds : s16;

namespace AO {

struct Fg1Block;

class FG1 final : public ::BaseGameObject
{
public:
    FG1(Fg1Resource& pFg1Res, CamResource& camRes);
    ~FG1();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;

    //s16 field_10_cam_pos_x = 0;
    //s16 field_12_cam_pos_y = 0;

    EReliveLevelIds field_14_current_level = EReliveLevelIds::eNone;
    s16 field_16_current_path = 0;

    Poly_FT4 mPolys[4] = {};
    Fg1Resource mFG1Res;
    CamResource mCamRes;
};

} // namespace AO
