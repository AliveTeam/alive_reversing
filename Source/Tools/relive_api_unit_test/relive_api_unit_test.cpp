#include "TlvsAO.hpp"
#include "TypesCollectionAO.hpp"
#include "TypesCollectionAE.hpp"

#include "../../AliveLibCommon/stdafx_common.h"

#include "../../relive_lib/data_conversion/guid.hpp"
#include "../../relive_lib/data_conversion/PathTlvsAE.hpp"
#include "../../relive_lib/data_conversion/file_system.hpp"

#include "JsonModelTypes.hpp"
#include "JsonReaderBase.hpp"

#include <gmock/gmock.h>

#include <array>
#include <memory>
#include <sstream>
#include <string_view>
#include <string>
#include <vector>

namespace {

[[nodiscard]] std::vector<u8>& getStaticFileBuffer()
{
    static std::vector<u8> result;
    return result;
}

[[maybe_unused]] [[nodiscard]] std::vector<u8>& readFileIntoStaticFileBuffer(const std::string& filePath)
{
    std::vector<u8>& buf = getStaticFileBuffer();

    FileSystem fs;
    if (!fs.LoadToVec(filePath.c_str(), buf))
    {
        std::cerr << "Failure reading file '" << filePath << "'\n";
        std::abort();
    }

    return buf;
}

// Get version

// Upgrade

TEST(alive_api, tlv_reflection_ao_object)
{
    ReliveAPI::TypesCollectionAO types;

    AO::Path_Hoist tlv = {};
    std::unique_ptr<ReliveAPI::TlvObjectBase> pHoist = types.MakeTlvAO(AO::TlvTypes::Hoist_3, &tlv, 99);

    ReliveAPI::Context context;

    auto obj = pHoist->InstanceToJson(types, context);
    pHoist->InstanceFromJson(types, obj, context);
    (void) pHoist->StructureToJson();            // TODO: check return value?
    ASSERT_EQ(pHoist->InstanceNumber(), 99);

    ReliveAPI::AOLine tmpLine(types);
}


TEST(alive_api, tlv_reflection_ae_object)
{
    ReliveAPI::TypesCollectionAE types;

    Path_Hoist tlv = {};
    std::unique_ptr<ReliveAPI::TlvObjectBase> pHoist = types.MakeTlvAE(TlvTypes::Hoist_2, &tlv, 99);

    ReliveAPI::Context context;

    auto obj = pHoist->InstanceToJson(types, context);
    pHoist->InstanceFromJson(types, obj, context);
    (void) pHoist->StructureToJson();            // TODO: check return value?
    ASSERT_EQ(pHoist->InstanceNumber(), 99);

    ReliveAPI::AELine tmpLine(types);
}

/*
TEST(json_upgrade, upgrade_rename_structure)
{
    // TODO: Put the json into an in memory string instead of loading from disk
    std::string upgradedJson = ReliveAPI::UpgradePathJson("C:\\GOG Games\\Abes Exoddus\\mi_1.json");
    std::ofstream newJson;
    newJson.open("C:\\GOG Games\\Abes Exoddus\\testing.txt");
    newJson << upgradedJson;
}
*/

TEST(guid, guid)
{
    {
        const Guid guid;
        ASSERT_FALSE(guid.IsValid());
    }

    {
        const Guid guid = Guid::NewGuid();
        ASSERT_TRUE(guid.IsValid());
    }

    {
        const Guid default1;
        const Guid default2;

        ASSERT_TRUE(default1 == default2);
        ASSERT_FALSE(default1 != default2);

        const Guid guid1 = Guid::NewGuid();
        const Guid guid2 = Guid::NewGuid();
        ASSERT_FALSE(guid1 == guid2);
        ASSERT_TRUE(guid1 != guid2);

    }

    {
        union TlvInfo
        {
            // When converted from an OG lvl we keep the original TLV info in the first 32 bits
            // as this allows us to convert saves as they containing TLV info records
            Guid::TlvOffsetLevelIdPathId mTlvInfo;
            u32 mData;
        };

        TlvInfo info;
        info.mData = 0;
        info.mTlvInfo.tlvOffset = 0xAABB;
        info.mTlvInfo.pathId = 0xCC;
        info.mTlvInfo.levelId = 0xDD;
        const Guid guid = Guid::NewGuidFromTlvInfo(info.mData);
        ASSERT_TRUE(guid.IsValid());

        ASSERT_STREQ("{CCDDAABB-00000000-00000000-00000000}", guid.ToString().c_str());

        Guid fromStringGuid = Guid::FromString(guid.ToString());
        ASSERT_STREQ("{CCDDAABB-00000000-00000000-00000000}", fromStringGuid.ToString().c_str());

     
    }

    {
        Guid guid = Guid::FromString("{DEADBEEF-CAFEBABE-12345678-ABCDEF99}");
        ASSERT_STREQ("{DEADBEEF-CAFEBABE-12345678-ABCDEF99}", guid.ToString().c_str());

    }
}

class ArgsAdapter
{
public:
    ArgsAdapter(s32 argc, char_type* argv[])
    {
        for (s32 i = 0; i < argc; i++)
        {
            mArgs.push_back(argv[i]);
        }
    }

    void Add(const std::string& arg)
    {
        mArgs.push_back(arg);
    }

    [[nodiscard]] s32 ArgC() const
    {
        return static_cast<s32>(mArgs.size());
    }

    [[nodiscard]] std::unique_ptr<char_type* []> ArgV() const
    {
        auto ptr = std::make_unique<char_type*[]>(mArgs.size());

        s32 i = 0;
        for (const auto& arg : mArgs)
        {
            ptr[i++] = const_cast<char_type*>(arg.c_str());
        }
        return ptr;
    }

    private : std::vector<std::string> mArgs;
};

} // namespace

// stop sdl_main messing with stuff
#undef main

s32 main(s32 argc, char_type* argv[])
{
    ArgsAdapter args(argc, argv);
    args.Add("--gtest_catch_exceptions=0");

    s32 newArgc = args.ArgC();
    auto newArgv = args.ArgV();

    ::testing::InitGoogleTest(&newArgc, newArgv.get());
    const auto ret = RUN_ALL_TESTS();
    return ret;
}
