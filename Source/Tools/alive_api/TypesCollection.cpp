#include "../AliveLibCommon/stdafx_common.h"
#include "TypesCollection.hpp"
#include "AOTlvs.hpp"
#include "AETlvs.hpp"
#include "../AliveLibAO/SwitchStates.hpp"
#include "../AliveLibAE/PathData.hpp"
#include "../AliveLibAE/SwitchStates.hpp"
#include "magic_enum/include/magic_enum.hpp"

TypesCollection::TypesCollection(Game gameType) 
    : mGameType(gameType)
{
    AddBasicType<BYTE>("Byte", 0, 255);
    AddBasicType<short>("UInt16", 0, 65535);
    
    if (mGameType == Game::AO)
    {
        AddAOTypes();
    }
    else
    {
        AddAETypes();
    }
}

template<class TlvEnumType, class TlvType, class T>
static void DoRegisterType(std::map<TlvEnumType, std::function<std::unique_ptr<TlvObjectBase>(TypesCollection&, TlvType*)>>& factory, std::map<std::string, std::function<std::unique_ptr<TlvObjectBase>(TypesCollection&, TlvType*)>>& reverseFactory, TypesCollection& constructingTypes)
{ 
    T tmp(constructingTypes, nullptr);
    TlvEnumType tlvType = tmp.TlvType();
    auto fnCreate = [](TypesCollection& types, TlvType* pTlv)
    {
        return std::make_unique<T>(types, pTlv);
    };
    reverseFactory[tmp.Name()] = fnCreate;
    factory[tlvType] = fnCreate;
}

#define REGISTER_TYPE_AO(TlvWrapperType) DoRegisterType<AO::TlvTypes, AO::Path_TLV, TlvWrapperType>(mTlvFactoryAO, mReverseTlvFactoryAO, *this)

#define REGISTER_TYPE_AE(TlvWrapperType) DoRegisterType<TlvTypes, Path_TLV, TlvWrapperType>(mTlvFactoryAE, mReverseTlvFactoryAE, *this)

void TypesCollection::AddAOTypes()
{
    REGISTER_TYPE_AO(AOTlvs::Path_Hoist);
    REGISTER_TYPE_AO(AOTlvs::Path_ContinuePoint);
    REGISTER_TYPE_AO(AOTlvs::Path_Door);
    REGISTER_TYPE_AO(AOTlvs::Path_Change);
    REGISTER_TYPE_AO(AOTlvs::Path_Switch);
    REGISTER_TYPE_AO(AOTlvs::Path_LightEffect);
    REGISTER_TYPE_AO(AOTlvs::Path_ElectricWall);
    REGISTER_TYPE_AO(AOTlvs::Path_ContinueZone);
    REGISTER_TYPE_AO(AOTlvs::Path_StartController);

    for (auto& [key, value] : mTlvFactoryAO)
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

void TypesCollection::AddAETypes()
{
    REGISTER_TYPE_AE(AETlvs::Path_Hoist);
    REGISTER_TYPE_AE(AETlvs::Path_Switch);
    REGISTER_TYPE_AE(AETlvs::Path_Door);
    REGISTER_TYPE_AE(AETlvs::Path_StatsSign);
    REGISTER_TYPE_AE(AETlvs::Path_BirdPortal);
    REGISTER_TYPE_AE(AETlvs::Path_Mudokon);
    REGISTER_TYPE_AE(AETlvs::Path_ElectricWall);
    REGISTER_TYPE_AE(AETlvs::Path_BoomMachine);
    REGISTER_TYPE_AE(AETlvs::Path_Slig);
    REGISTER_TYPE_AE(AETlvs::Path_Fleech);
    REGISTER_TYPE_AE(AETlvs::Path_EnemyStopper);
    REGISTER_TYPE_AE(AETlvs::Path_Teleporter);
    REGISTER_TYPE_AE(AETlvs::Path_UXB);
    REGISTER_TYPE_AE(AETlvs::Path_LCDScreen);
    REGISTER_TYPE_AE(AETlvs::Path_Edge);
    REGISTER_TYPE_AE(AETlvs::Path_StatusLight);
    REGISTER_TYPE_AE(AETlvs::Path_ShadowZone);

    for (auto& [key, value] : mTlvFactoryAE)
    {
        value(*this, nullptr)->AddTypes(*this);
    }

    AddEnum<SwitchOp>("Enum_SwitchOp",
        {
             { SwitchOp::eSetTrue_0, "SetTrue" },
             { SwitchOp::eSetFalse_1, "SetFalse" },
             { SwitchOp::eToggle_2, "Toggle" },
             { SwitchOp::eIncrement_3, "Increment" },
             { SwitchOp::eDecrement_4, "Decrement" },
        });

    AddEnum<LevelIds>("Enum_LevelIds",
        {
            {LevelIds::eNone, "None"},
            {LevelIds::eMenu_0, "Menu"},
            {LevelIds::eMines_1, "Mines"},
            {LevelIds::eNecrum_2, "Necrum"},
            {LevelIds::eMudomoVault_3, "MudomoVault"},
            {LevelIds::eMudancheeVault_4, "MudancheeVault"},
            {LevelIds::eFeeCoDepot_5, "FeeCoDepot"},
            {LevelIds::eBarracks_6, "Barracks"},
            {LevelIds::eMudancheeVault_Ender_7, "MudancheeVault_Ender"},
            {LevelIds::eBonewerkz_8, "Bonewerkz"},
            {LevelIds::eBrewery_9, "Brewery"},
            {LevelIds::eBrewery_Ender_10, "Brewery_Ender"},
            {LevelIds::eMudomoVault_Ender_11, "MudomoVault_Ender"},
            {LevelIds::eFeeCoDepot_Ender_12, "FeeCoDepot_Ender"},
            {LevelIds::eBarracks_Ender_13, "Barracks_Ender"},
            {LevelIds::eBonewerkz_Ender_14, "Bonewerkz_Ender"},
            {LevelIds::eNotUsed_15, "NotUsed"},
            {LevelIds::eCredits_16, "Credits"},
        });
}

void TypesCollection::AddTlvsToJsonArray(jsonxx::Array& array)
{
    if (mGameType == Game::AO)
    {
        for (auto& [key, value] : mTlvFactoryAO)
        {
            array << value(*this, nullptr)->StructureToJson();
        }
    }
    else
    {
        for (auto& [key, value] : mTlvFactoryAE)
        {
            array << value(*this, nullptr)->StructureToJson();
        }
    }
}

std::unique_ptr<TlvObjectBase> TypesCollection::MakeTlvAE(TlvTypes tlvType, Path_TLV* pTlv)
{
    auto it = mTlvFactoryAE.find(tlvType);
    if (it == std::end(mTlvFactoryAE))
    {
        LOG_WARNING("Type " << magic_enum::enum_name(tlvType) << " unknown");
        return nullptr;
    }
    return it->second(*this, pTlv);
}

std::unique_ptr<TlvObjectBase> TypesCollection::MakeTlvAO(AO::TlvTypes tlvType, AO::Path_TLV* pTlv)
{
    auto it = mTlvFactoryAO.find(tlvType);
    if (it == std::end(mTlvFactoryAO))
    {
        LOG_WARNING("Type " << magic_enum::enum_name(tlvType) << " unknown");
        return nullptr;
    }
    return it->second(*this, pTlv);
}

std::unique_ptr<TlvObjectBase> TypesCollection::MakeTlvAO(const std::string& tlvTypeName, AO::Path_TLV* pTlv)
{
    auto it = mReverseTlvFactoryAO.find(tlvTypeName);
    if (it == std::end(mReverseTlvFactoryAO))
    {
        LOG_WARNING("Type " << tlvTypeName << " unknown");
        return nullptr;
    }
    return it->second(*this, pTlv);
}
