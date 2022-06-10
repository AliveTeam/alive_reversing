#include "Camera.hpp"
#include "ResourceManager.hpp"

namespace AO {

Camera::Camera()
    : field_0_array(10)
{
    field_30_flags &= ~1u;
    field_C_ppBits = nullptr;
}

Camera::~Camera()
{
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
    }
}


void Camera::On_Loaded_4447A0(Camera* pThis)
{
    pThis->field_30_flags |= 1u;
    pThis->field_C_ppBits = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Bits, pThis->field_10_resId, 1, 0);
}

}
