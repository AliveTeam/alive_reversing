#include "stdafx.h"
#include "Camera.hpp"
#include "ResourceManager.hpp"

Camera::Camera()
{

}

void Camera::ctor_480DD0()
{
    field_0.ctor_40CA60(3);
    field_30_flags &= ~1u;
    field_C_pCamRes = nullptr;
}

void Camera::dtor_480E00()
{
    ResourceManager::FreeResource_49C330(field_C_pCamRes);

    DynamicArrayIter iter = {};
    iter.field_0_pDynamicArray = &field_0;
    iter.field_4_idx = 0;
    while (iter.field_4_idx < field_0.Size())
    {
        BYTE** pItem = field_0.ItemAt(iter.field_4_idx);
        iter.field_4_idx = iter.field_4_idx + 1;
        if (!pItem)
        {
            break;
        }
        ResourceManager::FreeResource_49C330(pItem);
        iter.Remove_At_Iter_40CCA0();
    }

    field_0.dtor_40CAD0();
}

void CC Camera::On_Loaded_480ED0(Camera* pCamera)
{
    pCamera->field_30_flags |= 1;
    pCamera->field_C_pCamRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Bits, pCamera->field_10_camera_resource_id, 1, 0);
}
