#include "Camera.hpp"
#include "FG1.hpp"

namespace AO {

Camera::Camera()
    : field_0_array(10)
{
    field_30_flags &= ~1u;
    //field_C_ppBits = nullptr;
}

Camera::~Camera()
{
    /*
    ResourceManager::FreeResource_455550(field_C_ppBits);

    for (s32 i = 0; i < field_0_array.Size(); i++)
    {
        u8** ppRes = field_0_array.ItemAt(i);
        if (!ppRes)
        {
            break;
        }

        ResourceManager::FreeResource_455550(ppRes);
        i = field_0_array.RemoveAt(i);
    }*/
}


void Camera::On_Loaded_4447A0(Camera* /*pThis*/)
{
    //pThis->field_30_flags |= 1u;
    //pThis->field_C_ppBits = ResourceManager::GetLoadedResource(ResourceManager::Resource_Bits, pThis->field_10_resId, 1, 0);
}

void Camera::CreateFG1()
{
    Fg1Resource fg1Res = ResourceManagerWrapper::LoadFg1(field_1A_level, field_18_path, field_1C_camera_number);
    if (fg1Res.Any())
    {
        relive_new FG1(fg1Res, field_C_ppBits);
    }

    // HACK: Create dummy FG1 to match OG so playbacks don't de-sync for now
    for (u32 i = 1; i < fg1Res.mFg1ResBlockCount; i++)
    {
        Fg1Resource nullFG1Res;
        CamResource nullCamRes;
        relive_new FG1(nullFG1Res, nullCamRes);
    }

}

}
