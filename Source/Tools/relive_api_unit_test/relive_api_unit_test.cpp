#include "relive_api.hpp"

#include "TlvsAO.hpp"
#include "TypesCollectionAO.hpp"
#include "TypesCollectionAE.hpp"

#include "../../AliveLibAE/DebugHelpers.hpp"

#include "../../AliveLibCommon/stdafx_common.h"
#include "../../AliveLibCommon/logger.hpp"

#include "LvlReaderWriter.hpp"
#include "CamConverter.hpp"
#include "JsonModelTypes.hpp"
#include "ApiFG1Reader.hpp"
#include "JsonReaderBase.hpp"

#include <gmock/gmock.h>

#include <array>
#include <memory>
#include <sstream>
#include <string_view>
#include <string>
#include <vector>

INITIALIZE_EASYLOGGINGPP

namespace {

[[nodiscard]] std::vector<u8>& getStaticFileBuffer()
{
    static std::vector<u8> result;
    return result;
}

[[nodiscard]] std::vector<u8>& readFileIntoStaticFileBuffer(const std::string& filePath)
{
    std::vector<u8>& buf = getStaticFileBuffer();

    if (!FS::ReadFileInto(buf, filePath))
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

    auto obj = pHoist->InstanceToJson(types);
    (void) pHoist->InstanceFromJson(types, obj); // TODO: check return value?
    (void) pHoist->StructureToJson();            // TODO: check return value?
    ASSERT_EQ(pHoist->InstanceNumber(), 99);

    ReliveAPI::AOLine tmpLine(types);
}


TEST(alive_api, tlv_reflection_ae_object)
{
    ReliveAPI::TypesCollectionAE types;

    Path_Hoist tlv = {};
    std::unique_ptr<ReliveAPI::TlvObjectBase> pHoist = types.MakeTlvAE(TlvTypes::Hoist_2, &tlv, 99);

    auto obj = pHoist->InstanceToJson(types);
    (void) pHoist->InstanceFromJson(types, obj); // TODO: check return value?
    (void) pHoist->StructureToJson();            // TODO: check return value?
    ASSERT_EQ(pHoist->InstanceNumber(), 99);

    ReliveAPI::AELine tmpLine(types);
}
/*
TEST(json_upgrade, upgrade_rename_structure)
{
    AliveAPI::JsonUpgradeResult r = AliveAPI::UpgradePathJson("rename_field.json");
    ASSERT_EQ(r.mResult, AliveAPI::UpgradeError::None);
}
*/

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
