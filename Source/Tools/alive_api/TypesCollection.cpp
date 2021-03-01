#include "../AliveLibCommon/stdafx_common.h"
#include "TypesCollection.hpp"
#include "AOTlvs.hpp"
#include "AETlvs.hpp"



TypesCollection::TypesCollection()
{
    AddBasicType<BYTE>("Byte", 0, 255);
    AddBasicType<short>("UInt16", 0, 65535);
    
    AddAOTypes();


    // TODO AE types
    //AETlvs::Path_Hoist::AddTypes(*this);

}


#define REGISTER_TYPE(tlvTypeName, classType) mTlvFactory[tlvTypeName] = [](TypesCollection& types, AO::Path_TLV* pTlv) { return std::make_unique<classType>(types, pTlv); }


void TypesCollection::AddAOTypes()
{
    REGISTER_TYPE(AO::TlvTypes::Hoist_3, AOTlvs::Path_Hoist);
    REGISTER_TYPE(AO::TlvTypes::ContinuePoint_0, AOTlvs::Path_ContinuePoint);
    REGISTER_TYPE(AO::TlvTypes::Door_6, AOTlvs::Path_Door);

    for (auto& [key, value] : mTlvFactory)
    {
        value(*this, nullptr)->AddTypes(*this);
    }

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

std::unique_ptr<TlvObjectBase> TypesCollection::MakeTlv(AO::TlvTypes tlvType, AO::Path_TLV* pTlv)
{
    auto it = mTlvFactory.find(tlvType);
    if (it == std::end(mTlvFactory))
    {
        LOG_WARNING("Type " << static_cast<int>(tlvType) << " unknown");
        return nullptr;
    }
    return it->second(*this, pTlv);
}
