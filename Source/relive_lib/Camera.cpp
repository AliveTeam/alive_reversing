#include "stdafx.h"
#include "Camera.hpp"
#include "FG1.hpp"

Camera::Camera()
{
    field_1A_level = EReliveLevelIds::eNone;
}

Camera::~Camera()
{

}

void Camera::On_Loaded(Camera* /*pCamera*/)
{

}

void Camera::CreateFG1()
{
    Fg1Resource fg1Res = ResourceManagerWrapper::LoadFg1(field_1A_level, field_18_path, field_1C_camera_number);
    if (fg1Res.Any())
    {
        relive_new FG1(fg1Res, field_C_pCamRes);
    }
    // HACK: Create dummy FG1 to match OG so playbacks don't de-sync for now
    for (u32 i = 1; i < fg1Res.mFg1ResBlockCount; i++)
    {
        Fg1Resource nullFG1Res;
        CamResource nullCamRes;
        relive_new FG1(nullFG1Res, nullCamRes);
    }

}
