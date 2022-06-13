#include "stdafx.h"
#include "Camera.hpp"
#include "ResourceManager.hpp"

Camera::Camera()
    : field_0(3)
{
    field_30_flags &= ~1u;
    field_C_pCamRes = nullptr;
}

void Camera::Free()
{
    if (field_C_pCamRes)
    {
        ResourceManager::FreeResource_49C330(field_C_pCamRes);
        field_C_pCamRes = nullptr;
    }

    DynamicArrayIter iter = {};
    iter.field_0_pDynamicArray = &field_0;
    iter.field_4_idx = 0;
    while (iter.field_4_idx < field_0.Size())
    {
        u8** pItem = field_0.ItemAt(iter.field_4_idx);
        iter.field_4_idx = iter.field_4_idx + 1;
        if (!pItem)
        {
            break;
        }
        ResourceManager::FreeResource_49C330(pItem);
        iter.Remove_At_Iter();
    }
}

Camera::~Camera()
{
    Free();
}

void Camera::On_Loaded(Camera* pCamera)
{
    pCamera->field_30_flags |= 1;
    pCamera->field_C_pCamRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Bits, pCamera->field_10_camera_resource_id, 1, 0);
}
