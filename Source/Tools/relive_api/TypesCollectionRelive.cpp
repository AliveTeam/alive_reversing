#include "TypesCollectionRelive.hpp"

#include "TlvsRelive.hpp"
#include "TlvObjectBase.hpp"

#include "../../AliveLibAE/PathData.hpp"
#include "../../relive_lib/SwitchStates.hpp"
#include "../../AliveLibAE/Path.hpp"
#include "../../AliveLibAE/Map.hpp"

#include "../../relive_lib/stdafx.h"

namespace ReliveAPI {
TypesCollectionRelive::TypesCollectionRelive()
{
    AddReliveTypes();
}

void TypesCollectionRelive::AddTlvsToJsonArray(jsonxx::Array& array)
{
    mTlvFactoryRelive.AddTlvsToJsonArray(*this, array);
}

[[nodiscard]] std::unique_ptr<TlvObjectBase> TypesCollectionRelive::MakeTlvRelive(ReliveTypes tlvType, relive::Path_TLV* pTlv, s32 instanceCount)
{
    return mTlvFactoryRelive.MakeTlvByEnum(*this, tlvType, pTlv, instanceCount);
}

[[nodiscard]] std::unique_ptr<TlvObjectBase> TypesCollectionRelive::MakeTlvRelive(const std::string& tlvTypeName, relive::Path_TLV* pTlv)
{
    return mTlvFactoryRelive.MakeTlvByName(*this, tlvTypeName, pTlv);
}

[[nodiscard]] std::unique_ptr<TlvObjectBase> TypesCollectionRelive::MakeTlvFromString(const std::string& tlvTypeName)
{
    return mTlvFactoryRelive.MakeTlvByName(*this, tlvTypeName, nullptr);
}

void TypesCollectionRelive::AddReliveTypes()
{
#define REGISTER_TYPE_RELIVE(TlvWrapperType) mTlvFactoryRelive.DoRegisterType<TlvWrapperType>(*this)

    REGISTER_TYPE_RELIVE(ReliveTlvs::Path_TimedMine);
    // TODO: Others

#undef REGISTER_TYPE_RELIVE

    // TODO: relive type global enums
    /*
    AddEnum<::Path_FlyingSlig_Data::SpawnDelayStates>("Enum_FlyingSligSpawnDelayStates",
    {
        {::Path_FlyingSlig_Data::SpawnDelayStates::eMoveImmediately_0, "Move Immediately"},
        {::Path_FlyingSlig_Data::SpawnDelayStates::eUseCustomSpawnMoveDelay_1, "Use Custom Spawn Move Delay"},
    });*/
}
} // namespace ReliveAPI
