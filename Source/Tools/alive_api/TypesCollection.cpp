#include "../AliveLibCommon/stdafx_common.h"
#include "TypesCollection.hpp"
#include "AOTlvs.hpp"
#include "AETlvs.hpp"
#include "../AliveLibAO/SwitchStates.hpp"


TypesCollection::TypesCollection()
{
    AddBasicType<BYTE>("Byte", 0, 255);
    AddBasicType<short>("UInt16", 0, 65535);
    
    AddAOTypes();


    // TODO AE types
    //AETlvs::Path_Hoist::AddTypes(*this);

}

template<class T>
static void DoRegisterType(std::map<AO::TlvTypes, std::function<std::unique_ptr<TlvObjectBase>(TypesCollection&, AO::Path_TLV*)>>& factory, std::map<std::string, std::function<std::unique_ptr<TlvObjectBase>(TypesCollection&, AO::Path_TLV*)>>& reverseFactory, AO::TlvTypes tlvType, TypesCollection& constructingTypes)
{ 
    T tmp(constructingTypes, nullptr);
    auto fnCreate = [tlvType](TypesCollection& types, AO::Path_TLV* pTlv)
    {
        auto ret = std::make_unique<T>(types, pTlv);
        ret->SetType(tlvType);
        return ret;
    };
    reverseFactory[tmp.Name()] = fnCreate;
    factory[tlvType] = fnCreate;
}

#define REGISTER_TYPE(TlvWrapperType, TlvType) DoRegisterType<TlvWrapperType>(mTlvFactory, mReverseTlvFactory, TlvType, *this)

void TypesCollection::AddAOTypes()
{

    REGISTER_TYPE(AOTlvs::Path_Hoist, AO::TlvTypes::Hoist_3);
    REGISTER_TYPE(AOTlvs::Path_ContinuePoint, AO::TlvTypes::ContinuePoint_0);
    REGISTER_TYPE(AOTlvs::Path_Door, AO::TlvTypes::Door_6);
    REGISTER_TYPE(AOTlvs::Path_Change, AO::TlvTypes::PathTransition_1);
    REGISTER_TYPE(AOTlvs::Path_Switch, AO::TlvTypes::Switch_26);
    REGISTER_TYPE(AOTlvs::Path_LightEffect, AO::TlvTypes::LightEffect_106);
    REGISTER_TYPE(AOTlvs::Path_ElectricWall, AO::TlvTypes::ElectricWall_67);
    REGISTER_TYPE(AOTlvs::Path_ContinueZone, AO::TlvTypes::ContinueZone_2);
    REGISTER_TYPE(AOTlvs::Path_StartController, AO::TlvTypes::StartController_28);

    for (auto& [key, value] : mTlvFactory)
    {
        value(*this, nullptr)->AddTypes(*this);
    }

    AddEnum<AO::SwitchOp>("Enum_SwitchOp",
        {
             { AO::SwitchOp::eSetTrue_0, "SetTrue" },
             { AO::SwitchOp::eSetFalse_1, "SetFalse" },
             { AO::SwitchOp::eToggle_2, "Toggle" },
             { AO::SwitchOp::eIncrement_3, "Increment" },
             { AO::SwitchOp::eDecrement_4, "Decrement" },
        });

    AddEnum<AO::LevelIds>("Enum_LevelIds",
        {
            {AO::LevelIds::eNone, "None"},
            {AO::LevelIds::eMenu_0, "Menu"},
            {AO::LevelIds::eRuptureFarms_1, "RuptureFarms"},
            {AO::LevelIds::eLines_2, "Lines"},
            {AO::LevelIds::eForest_3, "Forest"},
            {AO::LevelIds::eForestTemple_4, "ForestTemple"},
            {AO::LevelIds::eStockYards_5, "StockYards"},
            {AO::LevelIds::eStockYardsReturn_6, "StockYardsReturn"},
            {AO::LevelIds::eRemoved_7, "Removed7"},
            {AO::LevelIds::eDesert_8, "Desert"},
            {AO::LevelIds::eDesertTemple_9, "DesertTemple"},
            {AO::LevelIds::eCredits_10, "Credits"},
            {AO::LevelIds::eRemoved_11, "Removed11"},
            {AO::LevelIds::eBoardRoom_12, "BoardRoom"},
            {AO::LevelIds::eRuptureFarmsReturn_13, "RuptureFarmsReturn"},
            {AO::LevelIds::eForestChase, "ForestChase"},
            {AO::LevelIds::eDesertEscape, "DesertEscape"},
        });

}

void TypesCollection::AddTlvsToJsonArray(jsonxx::Array& array)
{
    for (auto& [key, value] : mTlvFactory)
    {
        array << value(*this, nullptr)->StructureToJson();
    }
}

std::unique_ptr<TlvObjectBase> TypesCollection::MakeTlvAO(AO::TlvTypes tlvType, AO::Path_TLV* pTlv)
{
    auto it = mTlvFactory.find(tlvType);
    if (it == std::end(mTlvFactory))
    {
        LOG_WARNING("Type " << static_cast<int>(tlvType) << " unknown");
        return nullptr;
    }
    return it->second(*this, pTlv);
}

std::unique_ptr<TlvObjectBase> TypesCollection::MakeTlvAO(const std::string& tlvTypeName, AO::Path_TLV* pTlv)
{
    auto it = mReverseTlvFactory.find(tlvTypeName);
    if (it == std::end(mReverseTlvFactory))
    {
        LOG_WARNING("Type " << tlvTypeName << " unknown");
        return nullptr;
    }
    return it->second(*this, pTlv);
}
