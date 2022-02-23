#include "relive_config.h"
#include "logger.hpp"
#include "FunctionFwd.hpp"
#include "SDL_main.h"
#include "Io.hpp"
#include "Sys.hpp"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "misc/cpp/imgui_stdlib.h"
#include "imgui_impl_opengl3.h"
#include "GL/glew.h"
#include "../AliveLibAE/stdlib.hpp"

#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 1024
#include "magic_enum/include/magic_enum.hpp"

#include "../AliveLibAE/LvlArchive.hpp"
#include "../AliveLibAE/Game.hpp"
#include "../AliveLibAE/PathData.hpp"
#include "../AliveLibAE/ResourceManager.hpp"
#include "../AliveLibAE/Animation.hpp"
#include "../AliveLibAE/Compression.hpp"
#include "../AliveLibAE/PsxDisplay.hpp"

#include "../AliveLibAO/Compression.hpp"

#include "AnimResources.hpp"
#include "../AliveLibAE/ObjectIds.hpp"

#include <string>
#include <functional>
#include <filesystem>
#include <thread>

#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "asset_common.hpp"
#include "asset_video.hpp"

namespace fs = std::filesystem;

#pragma warning(disable:4312)

bool CC RunningAsInjectedDll()
{
    return false;
}

struct RGBAPixel final {
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
};

struct PreviewTexture final
{
    GLuint id;
    int width;
    int height;
    int offsetX;
    int offsetY;
};

struct PreviewExternalTexture final
{
    GLuint id;
};

struct LoadedAnimRecord final
{
    std::string name;
    AnimRecord record;
};

const ImGuiWindowFlags kImGuiFullscreenFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

struct Bounds final
{
    int x;
    int y;
    int w;
    int h;
};

struct LevelInfo
{
    const char* lvl_file;
    const char* lvl_name;
};

const LevelInfo AeLvls[] = {
    {"st.lvl", "Start"},
    {"mi.lvl", "Mines"},
    {"ne.lvl", "Necrum"},
    {"ba.lvl", ""},
    {"bm.lvl", ""},
    {"br.lvl", ""},
    {"bw.lvl", "Bonewerkz"},
    {"fd.lvl", "Feeco Depot"},
    {"pv.lvl", "Paramonia Vault"},
    {"sv.lvl", "Scrabania Vault"},
    {"cr.lvl", "Credits"},
};

const LevelInfo AoLvls[]{
    {"c1.lvl", ""},
    {"d1.lvl", ""},
    {"d2.lvl", ""},
    {"d7.lvl", ""},
    {"e1.lvl", ""},
    {"e2.lvl", ""},
    {"f1.lvl", ""},
    {"f2.lvl", ""},
    {"f4.lvl", ""},
    {"l1.lvl", ""},
    {"r1.lvl", ""},
    {"r2.lvl", ""},
    {"r6.lvl", ""},
    {"s1.lvl", ""},
};

struct AppTool
{
    const char* name;
    std::function<void(ImGuiWindowFlags, float)> update_func;
};

struct FrameTableEntry
{
    std::string file_prefix;
    int x_offset;
    int y_offset;
    bool file_exists;
};

// Big old table to convert ae file names to new names
static std::map<int, FrameTableEntry> frametableMatches = {
    {7812, {"ABEBLOW.BAN_0", 0, 0, false } },
    {7732, {"ABEBLOW.BAN_1", 0, 0, false } },
    {7772, {"ABEBLOW.BAN_2", 0, 0, false } },
    {6520, {"ABEBOMB.BAN_0", 0, 0, false } },
    {271248, {"ABEBSIC.BAN_0", 0, 0, false } },
    {269688, {"ABEBSIC.BAN_1", 0, 0, false } },
    {269708, {"ABEBSIC.BAN_2", 0, 0, false } },
    {269764, {"ABEBSIC.BAN_3", 0, 0, false } },
    {269784, {"ABEBSIC.BAN_4", 0, 0, false } },
    {269804, {"ABEBSIC.BAN_5", 0, 0, false } },
    {269856, {"ABEBSIC.BAN_6", 0, 0, false } },
    {269876, {"ABEBSIC.BAN_7", 0, 0, false } },
    {269928, {"ABEBSIC.BAN_8", 0, 0, false } },
    {269976, {"ABEBSIC.BAN_9", 0, 0, false } },
    {270024, {"ABEBSIC.BAN_10", 0, 0, false } },
    {270060, {"ABEBSIC.BAN_11", 0, 0, false } },
    {270092, {"ABEBSIC.BAN_12", 0, 0, false } },
    {270120, {"ABEBSIC.BAN_13", 0, 0, false } },
    {270140, {"ABEBSIC.BAN_14", 0, 0, false } },
    {270160, {"ABEBSIC.BAN_15", 0, 0, false } },
    {270180, {"ABEBSIC.BAN_16", 0, 0, false } },
    {270240, {"ABEBSIC.BAN_17", 0, 0, false } },
    {270252, {"ABEBSIC.BAN_18", 0, 0, false } },
    {270268, {"ABEBSIC.BAN_19", 0, 0, false } },
    {270288, {"ABEBSIC.BAN_20", 0, 0, false } },
    {270308, {"ABEBSIC.BAN_21", 0, 0, false } },
    {270328, {"ABEBSIC.BAN_22", 0, 0, false } },
    {270408, {"ABEBSIC.BAN_23", 0, 0, false } },
    {270428, {"ABEBSIC.BAN_24", 0, 0, false } },
    {270468, {"ABEBSIC.BAN_25", 0, 0, false } },
    {270488, {"ABEBSIC.BAN_26", 0, 0, false } },
    {270508, {"ABEBSIC.BAN_27", 0, 0, false } },
    {270596, {"ABEBSIC.BAN_28", 0, 0, false } },
    {270616, {"ABEBSIC.BAN_29", 0, 0, false } },
    {270668, {"ABEBSIC.BAN_30", 0, 0, false } },
    {270688, {"ABEBSIC.BAN_31", 0, 0, false } },
    {270708, {"ABEBSIC.BAN_32", 0, 0, false } },
    {270728, {"ABEBSIC.BAN_33", 0, 0, false } },
    {270748, {"ABEBSIC.BAN_34", 0, 0, false } },
    {270820, {"ABEBSIC.BAN_35", 0, 0, false } },
    {270840, {"ABEBSIC.BAN_36", 0, 0, false } },
    {270860, {"ABEBSIC.BAN_37", 0, 0, false } },
    {270932, {"ABEBSIC.BAN_38", 0, 0, false } },
    {270944, {"ABEBSIC.BAN_39", 0, 0, false } },
    {270964, {"ABEBSIC.BAN_40", 0, 0, false } },
    {271004, {"ABEBSIC.BAN_41", 0, 0, false } },
    {271056, {"ABEBSIC.BAN_42", 0, 0, false } },
    {271080, {"ABEBSIC.BAN_43", 0, 0, false } },
    {271120, {"ABEBSIC.BAN_44", 0, 0, false } },
    {271152, {"ABEBSIC.BAN_45", 0, 0, false } },
    {271192, {"ABEBSIC.BAN_46", 0, 0, false } },
    {59112, {"ABEBSIC1.BAN_0", 0, 0, false } },
    {58748, {"ABEBSIC1.BAN_1", 0, 0, false } },
    {58768, {"ABEBSIC1.BAN_2", 0, 0, false } },
    {58788, {"ABEBSIC1.BAN_3", 0, 0, false } },
    {58808, {"ABEBSIC1.BAN_4", 0, 0, false } },
    {58888, {"ABEBSIC1.BAN_5", 0, 0, false } },
    {58920, {"ABEBSIC1.BAN_6", 0, 0, false } },
    {58956, {"ABEBSIC1.BAN_7", 0, 0, false } },
    {58992, {"ABEBSIC1.BAN_8", 0, 0, false } },
    {59028, {"ABEBSIC1.BAN_9", 0, 0, false } },
    {59064, {"ABEBSIC1.BAN_10", 0, 0, false } },
    {19184, {"ABEDOOR.BAN_0", 0, 0, false } },
    {19040, {"ABEDOOR.BAN_1", 0, 0, false } },
    {19088, {"ABEDOOR.BAN_2", 0, 0, false } },
    {19136, {"ABEDOOR.BAN_3", 0, 0, false } },
    {32168, {"ABEEDGE.BAN_0", 0, 0, false } },
    {32012, {"ABEEDGE.BAN_1", 0, 0, false } },
    {32040, {"ABEEDGE.BAN_2", 0, 0, false } },
    {32068, {"ABEEDGE.BAN_3", 0, 0, false } },
    {32112, {"ABEEDGE.BAN_4", 0, 0, false } },
    {32132, {"ABEEDGE.BAN_5", 0, 0, false } },
    {32152, {"ABEEDGE.BAN_6", 0, 0, false } },
    {28824, {"ABEGAS.BAN_0", 0, 0, false } },
    {57712, {"ABEHOIST.BAN_0", 0, 0, false } },
    {57440, {"ABEHOIST.BAN_1", 0, 0, false } },
    {57540, {"ABEHOIST.BAN_2", 0, 0, false } },
    {57580, {"ABEHOIST.BAN_3", 0, 0, false } },
    {57680, {"ABEHOIST.BAN_4", 0, 0, false } },
    {49912, {"ABEKNBK.BAN_0", 0, 0, false } },
    {49740, {"ABEKNBK.BAN_1", 0, 0, false } },
    {49800, {"ABEKNBK.BAN_2", 0, 0, false } },
    {49856, {"ABEKNBK.BAN_3", 0, 0, false } },
    {17240, {"ABEKNFD.BAN_0", 0, 0, false } },
    {17180, {"ABEKNFD.BAN_1", 0, 0, false } },
    {24628, {"ABEKNOKZ.BAN_0", 0, 0, false } },
    {24580, {"ABEKNOKZ.BAN_1", 0, 0, false } },
    {22596, {"ABELIFT.BAN_0", 0, 0, false } },
    {22464, {"ABELIFT.BAN_1", 0, 0, false } },
    {22496, {"ABELIFT.BAN_2", 0, 0, false } },
    {22548, {"ABELIFT.BAN_3", 0, 0, false } },
    {22572, {"ABELIFT.BAN_4", 0, 0, false } },
    {10040, {"ABEOMM.BAN_0", 0, 0, false } },
    {9992, {"ABEOMM.BAN_1", 0, 0, false } },
    {6004, {"ABEPICK.BAN_0", 0, 0, false } },
    {11396, {"ABEPULL.BAN_0", 0, 0, false } },
    {16220, {"ABESTONE.BAN_0", 0, 0, false } },
    {16040, {"ABESTONE.BAN_1", 0, 0, false } },
    {16096, {"ABESTONE.BAN_2", 0, 0, false } },
    {31656, {"ABETHROW.BAN_0", 0, 0, false } },
    {31392, {"ABETHROW.BAN_1", 0, 0, false } },
    {31432, {"ABETHROW.BAN_2", 0, 0, false } },
    {31504, {"ABETHROW.BAN_3", 0, 0, false } },
    {31576, {"ABETHROW.BAN_4", 0, 0, false } },
    {31632, {"ABETHROW.BAN_5", 0, 0, false } },
    {20572, {"ABEWELL.BAN_0", 0, 0, false } },
    {20404, {"ABEWELL.BAN_1", 0, 0, false } },
    {20484, {"ABEWELL.BAN_2", 0, 0, false } },
    {20552, {"ABEWELL.BAN_3", 0, 0, false } },
    {8588, {"ABECAR.BAN_0", 0, 0, false } },
    {8540, {"ABECAR.BAN_1", 0, 0, false } },
    {8772, {"ABEMORPH.BAN_0", 0, 0, false } },
    {8732, {"ABEMORPH.BAN_1", 0, 0, false } },
    {5748, {"ABENOELM.BND_0", 0, 0, false } },
    {5724, {"ABENOELM.BND_1", 0, 0, false } },
    {8104, {"ABENOELM.BND_0", 0, 0, false } },
    {11888, {"ABEWORK.BAN_0", 0, 0, false } },
    {11816, {"ABEWORK.BAN_1", 0, 0, false } },
    {11856, {"ABEWORK.BAN_2", 0, 0, false } },
    {4488, {"DUST.BAN_0", 0, 0, false } },
    {216, {"BLOOD.BAN_0", 0, 0, false } },
    {192, {"BLOOD.BAN_1", 0, 0, false } },
    {204, {"BLOOD.BAN_2", 0, 0, false } },
    {33552, {"SLGSLEEP.BAN_0", 0, 0, false } },
    {33448, {"SLGSLEEP.BAN_1", 0, 0, false } },
    {6560, {"SLGBLOW.BAN_0", 0, 0, false } },
    {6480, {"SLGBLOW.BAN_1", 0, 0, false } },
    {6520, {"SLGBLOW.BAN_2", 0, 0, false } },
    {12660, {"SLGBEAT.BAN_0", 0, 0, false } },
    {30628, {"SLGEDGE.BAN_0", 0, 0, false } },
    {30560, {"SLGEDGE.BAN_1", 0, 0, false } },
    {30592, {"SLGEDGE.BAN_2", 0, 0, false } },
    {13016, {"SLGKNFD.BAN_0", 0, 0, false } },
    {30628, {"SLGEDGE.BAN_0", 0, 0, false } },
    {30560, {"SLGEDGE.BAN_1", 0, 0, false } },
    {30592, {"SLGEDGE.BAN_2", 0, 0, false } },
    {9204, {"SLGSMASH.BAN_0", 0, 0, false } },
    {12612, {"SLGLEVER.BAN_0", 0, 0, false } },
    {23200, {"SLGLIFT.BAN_0", 0, 0, false } },
    {23048, {"SLGLIFT.BAN_1", 0, 0, false } },
    {23072, {"SLGLIFT.BAN_2", 0, 0, false } },
    {23096, {"SLGLIFT.BAN_3", 0, 0, false } },
    {23148, {"SLGLIFT.BAN_4", 0, 0, false } },
    {136132, {"SLIG.BND_0", 0, 0, false } },
    {135360, {"SLIG.BND_1", 0, 0, false } },
    {135440, {"SLIG.BND_2", 0, 0, false } },
    {135512, {"SLIG.BND_3", 0, 0, false } },
    {135544, {"SLIG.BND_4", 0, 0, false } },
    {135576, {"SLIG.BND_5", 0, 0, false } },
    {135632, {"SLIG.BND_6", 0, 0, false } },
    {135652, {"SLIG.BND_7", 0, 0, false } },
    {135696, {"SLIG.BND_8", 0, 0, false } },
    {135768, {"SLIG.BND_9", 0, 0, false } },
    {135788, {"SLIG.BND_10", 0, 0, false } },
    {135804, {"SLIG.BND_11", 0, 0, false } },
    {135824, {"SLIG.BND_12", 0, 0, false } },
    {135844, {"SLIG.BND_13", 0, 0, false } },
    {135876, {"SLIG.BND_14", 0, 0, false } },
    {135896, {"SLIG.BND_15", 0, 0, false } },
    {135916, {"SLIG.BND_16", 0, 0, false } },
    {135936, {"SLIG.BND_17", 0, 0, false } },
    {135976, {"SLIG.BND_18", 0, 0, false } },
    {136012, {"SLIG.BND_19", 0, 0, false } },
    {136048, {"SLIG.BND_20", 0, 0, false } },
    {136088, {"SLIG.BND_21", 0, 0, false } },
    {33348, {"SLIG.BND_0", 0, 0, false } },
    {33184, {"SLIG.BND_1", 0, 0, false } },
    {33228, {"SLIG.BND_2", 0, 0, false } },
    {960, {"SLIG.BND_0", 0, 0, false } },
    {320, {"SLIG.BND_0", 0, 0, false } },
    {24628, {"SLIGZ.BND_0", 0, 0, false } },
    {24580, {"SLIGZ.BND_1", 0, 0, false } },
    {2832, {"SLIGZ.BND_0", 0, 0, false } },
    {9260, {"SLIGZ.BND_0", 0, 0, false } },
    {9208, {"SLIGZ.BND_1", 0, 0, false } },
    {216, {"BLOOD.BAN_0", 0, 0, false } },
    {192, {"BLOOD.BAN_1", 0, 0, false } },
    {204, {"BLOOD.BAN_2", 0, 0, false } },
    {6560, {"SLGBLOW.BAN_0", 0, 0, false } },
    {6480, {"SLGBLOW.BAN_1", 0, 0, false } },
    {6520, {"SLGBLOW.BAN_2", 0, 0, false } },
    {1252, {"GRENADE.BAN_0", 0, 0, false } },
    {14108, {"SMEXP.BAN_0", 0, 0, false } },
    {2244, {"METAL.BAN_0", 0, 0, false } },
    {7812, {"ABEBLOW.BAN_0", 0, 0, false } },
    {7732, {"ABEBLOW.BAN_1", 0, 0, false } },
    {7772, {"ABEBLOW.BAN_2", 0, 0, false } },
    {960, {"BIGFLASH.BAN_0", 0, 0, false } },
    {5264, {"VAPOR.BAN_0", 0, 0, false } },
    {7544, {"DOGBLOW.BAN_0", 0, 0, false } },
    {7504, {"DOGBLOW.BAN_1", 0, 0, false } },
    {117752, {"FLYSLIG.BND_0", 0, 0, false } },
    {116888, {"FLYSLIG.BND_1", 0, 0, false } },
    {116912, {"FLYSLIG.BND_2", 0, 0, false } },
    {116936, {"FLYSLIG.BND_3", 0, 0, false } },
    {116988, {"FLYSLIG.BND_4", 0, 0, false } },
    {117012, {"FLYSLIG.BND_5", 0, 0, false } },
    {117036, {"FLYSLIG.BND_6", 0, 0, false } },
    {117060, {"FLYSLIG.BND_7", 0, 0, false } },
    {117084, {"FLYSLIG.BND_8", 0, 0, false } },
    {117132, {"FLYSLIG.BND_9", 0, 0, false } },
    {117188, {"FLYSLIG.BND_10", 0, 0, false } },
    {117276, {"FLYSLIG.BND_11", 0, 0, false } },
    {117296, {"FLYSLIG.BND_12", 0, 0, false } },
    {117316, {"FLYSLIG.BND_13", 0, 0, false } },
    {117336, {"FLYSLIG.BND_14", 0, 0, false } },
    {117356, {"FLYSLIG.BND_15", 0, 0, false } },
    {117376, {"FLYSLIG.BND_16", 0, 0, false } },
    {117396, {"FLYSLIG.BND_17", 0, 0, false } },
    {117424, {"FLYSLIG.BND_18", 0, 0, false } },
    {117444, {"FLYSLIG.BND_19", 0, 0, false } },
    {117464, {"FLYSLIG.BND_20", 0, 0, false } },
    {117492, {"FLYSLIG.BND_21", 0, 0, false } },
    {117524, {"FLYSLIG.BND_22", 0, 0, false } },
    {117552, {"FLYSLIG.BND_23", 0, 0, false } },
    {117584, {"FLYSLIG.BND_24", 0, 0, false } },
    {117616, {"FLYSLIG.BND_25", 0, 0, false } },
    {117648, {"FLYSLIG.BND_26", 0, 0, false } },
    {117668, {"FLYSLIG.BND_27", 0, 0, false } },
    {117700, {"FLYSLIG.BND_28", 0, 0, false } },
    {117736, {"FLYSLIG.BND_29", 0, 0, false } },
    {216, {"BLOOD.BAN_0", 0, 0, false } },
    {192, {"BLOOD.BAN_1", 0, 0, false } },
    {204, {"BLOOD.BAN_2", 0, 0, false } },
    {6560, {"SLGBLOW.BAN_0", 0, 0, false } },
    {6480, {"SLGBLOW.BAN_1", 0, 0, false } },
    {6520, {"SLGBLOW.BAN_2", 0, 0, false } },
    {1252, {"GRENADE.BAN_0", 0, 0, false } },
    {14108, {"SMEXP.BAN_0", 0, 0, false } },
    {2244, {"METAL.BAN_0", 0, 0, false } },
    {7812, {"ABEBLOW.BAN_0", 0, 0, false } },
    {7732, {"ABEBLOW.BAN_1", 0, 0, false } },
    {7772, {"ABEBLOW.BAN_2", 0, 0, false } },
    {960, {"BIGFLASH.BAN_0", 0, 0, false } },
    {5264, {"VAPOR.BAN_0", 0, 0, false } },
    {7544, {"DOGBLOW.BAN_0", 0, 0, false } },
    {7504, {"DOGBLOW.BAN_1", 0, 0, false } },
    {4364, {"LOCKER.BAN_0", 0, 0, false } },
    {4352, {"LOCKER.BAN_1", 0, 0, false } },
    {420, {"CSLGBUTN.BAN_0", 0, 0, false } },
    {408, {"CSLGBUTN.BAN_1", 0, 0, false } },
    {54188, {"CRAWLSLG.BND_0", 0, 0, false } },
    {53560, {"CRAWLSLG.BND_1", 0, 0, false } },
    {53616, {"CRAWLSLG.BND_2", 0, 0, false } },
    {53684, {"CRAWLSLG.BND_3", 0, 0, false } },
    {53720, {"CRAWLSLG.BND_4", 0, 0, false } },
    {53736, {"CRAWLSLG.BND_5", 0, 0, false } },
    {53752, {"CRAWLSLG.BND_6", 0, 0, false } },
    {53780, {"CRAWLSLG.BND_7", 0, 0, false } },
    {53816, {"CRAWLSLG.BND_8", 0, 0, false } },
    {53852, {"CRAWLSLG.BND_9", 0, 0, false } },
    {53928, {"CRAWLSLG.BND_10", 0, 0, false } },
    {53964, {"CRAWLSLG.BND_11", 0, 0, false } },
    {54000, {"CRAWLSLG.BND_12", 0, 0, false } },
    {54032, {"CRAWLSLG.BND_13", 0, 0, false } },
    {54076, {"CRAWLSLG.BND_14", 0, 0, false } },
    {54096, {"CRAWLSLG.BND_15", 0, 0, false } },
    {54120, {"CRAWLSLG.BND_16", 0, 0, false } },
    {54140, {"CRAWLSLG.BND_17", 0, 0, false } },
    {54160, {"CRAWLSLG.BND_18", 0, 0, false } },
    {51588, {"BGEXPLD.BAN_0", 0, 0, false } },
    {960, {"BIGFLASH.BAN_0", 0, 0, false } },
    {216, {"BLOOD.BAN_0", 0, 0, false } },
    {192, {"BLOOD.BAN_1", 0, 0, false } },
    {204, {"BLOOD.BAN_2", 0, 0, false } },
    {308, {"BLOODROP.BAN_0", 0, 0, false } },
    {556, {"BOMBFLSH.BAN_0", 0, 0, false } },
    {544, {"BOMBFLSH.BAN_1", 0, 0, false } },
    {8788, {"BONEBAG.BAN_0", 0, 0, false } },
    {8708, {"BONEBAG.BAN_1", 0, 0, false } },
    {8748, {"BONEBAG.BAN_2", 0, 0, false } },
    {17548, {"MOVEBOMB.BAN_0", 0, 0, false } },
    {15888, {"MEATSACK.BAN_0", 0, 0, false } },
    {15848, {"MEATSACK.BAN_1", 0, 0, false } },
    {10008, {"DEADFLR.BAN_0", 0, 0, false } },
    {9912, {"DEADFLR.BAN_1", 0, 0, false } },
    {9940, {"DEADFLR.BAN_2", 0, 0, false } },
    {6484, {"DEBRIS00.BAN_0", 0, 0, false } },
    {8704, {"DOOR.BAN_0", 0, 0, false } },
    {8692, {"DOOR.BAN_1", 0, 0, false } },
    {5836, {"DOVBASIC.BAN_0", 0, 0, false } },
    {5516, {"DOVBASIC.BAN_1", 0, 0, false } },
    {5580, {"DOVBASIC.BAN_2", 0, 0, false } },
    {5760, {"DOVBASIC.BAN_3", 0, 0, false } },
    {204, {"DRPROCK.BAN_0", 0, 0, false } },
    {180, {"DRPROCK.BAN_1", 0, 0, false } },
    {192, {"DRPROCK.BAN_2", 0, 0, false } },
    {288, {"DRPSPRK.BAN_0", 0, 0, false } },
    {252, {"DRPSPRK.BAN_1", 0, 0, false } },
    {272, {"DRPSPRK.BAN_2", 0, 0, false } },
    {15384, {"ELECWALL.BAN_0", 0, 0, false } },
    {1252, {"EMOHAP.BAN_0", 0, 0, false } },
    {1076, {"EMOANGRY.BAN_0", 0, 0, false } },
    {1248, {"EMONORM.BAN_0", 0, 0, false } },
    {51156, {"EXPLO2.BAN_0", 0, 0, false } },
    {14108, {"SMEXP.BAN_0", 0, 0, false } },
    {2228, {"MAIMORB.BAN_0", 0, 0, false } },
    {316, {"BREWBTN.BAN_0", 0, 0, false } },
    {3532, {"EVILFART.BAN_0", 0, 0, false } },
    {4208, {"FARTFAN.BAN_0", 0, 0, false } },
    {4256, {"FALLROCK.BAN_0", 0, 0, false } },
    {4232, {"FALLROCK.BAN_1", 0, 0, false } },
    {5156, {"FIRE.BAN_0", 0, 0, false } },
    {7068, {"GHOSTTRP.BAN_0", 0, 0, false } },
    {6976, {"GHOSTTRP.BAN_1", 0, 0, false } },
    {7056, {"GHOSTTRP.BAN_2", 0, 0, false } },
    {1400, {"GLOW1.BAN_0", 0, 0, false } },
    {50236, {"GREETER.BAN_0", 0, 0, false } },
    {50028, {"GREETER.BAN_1", 0, 0, false } },
    {50072, {"GREETER.BAN_2", 0, 0, false } },
    {50104, {"GREETER.BAN_3", 0, 0, false } },
    {50144, {"GREETER.BAN_4", 0, 0, false } },
    {50172, {"GREETER.BAN_5", 0, 0, false } },
    {50196, {"GREETER.BAN_6", 0, 0, false } },
    {50212, {"GREETER.BAN_7", 0, 0, false } },
    {356, {"GOLDGLOW.BAN_0", 0, 0, false } },
    {112, {"GRENGLOW.BAN_0", 0, 0, false } },
    {13936, {"HIGHLITE.BAN_0", 0, 0, false } },
    {13900, {"HIGHLITE.BAN_1", 0, 0, false } },
    {13912, {"HIGHLITE.BAN_2", 0, 0, false } },
    {13924, {"HIGHLITE.BAN_3", 0, 0, false } },
    {256, {"HUBLIGHT.BAN_0", 0, 0, false } },
    {244, {"HUBLIGHT.BAN_1", 0, 0, false } },
    {256, {"HUBRED.BAN_0", 0, 0, false } },
    {244, {"HUBRED.BAN_1", 0, 0, false } },
    {784, {"LANDMINE.BAN_0", 0, 0, false } },
    {900, {"LOADING.BAN_0", 0, 0, false } },
    {2244, {"METAL.BAN_0", 0, 0, false } },
    {1736, {"MFLARE.BAN_0", 0, 0, false } },
    {400, {"MINEFLSH.BAN_0", 0, 0, false } },
    {23280, {"MOTION.BAN_0", 0, 0, false } },
    {5328, {"MUDBTLNK.BAN_0", 0, 0, false } },
    {5320, {"MUDCHSL.BAN_0", 0, 0, false } },
    {5276, {"MUDCHSL.BAN_1", 0, 0, false } },
    {5308, {"MUDCHSL.BAN_2", 0, 0, false } },
    {9640, {"MUDIDLE.BAN_0", 0, 0, false } },
    {11392, {"MUDLTUS.BAN_0", 0, 0, false } },
    {11300, {"MUDLTUS.BAN_1", 0, 0, false } },
    {11336, {"MUDLTUS.BAN_2", 0, 0, false } },
    {5280, {"MUDODUCK.BAN_0", 0, 0, false } },
    {5236, {"MUDODUCK.BAN_1", 0, 0, false } },
    {5256, {"MUDODUCK.BAN_2", 0, 0, false } },
    {9388, {"MUDSCRUB.BAN_0", 0, 0, false } },
    {21000, {"MUDTORT.BAN_0", 0, 0, false } },
    {20864, {"MUDTORT.BAN_1", 0, 0, false } },
    {20892, {"MUDTORT.BAN_2", 0, 0, false } },
    {1672, {"OMMFLARE.BAN_0", 0, 0, false } },
    {1632, {"OMMFLARE.BAN_1", 0, 0, false } },
    {808, {"SPARKS.BAN_0", 0, 0, false } },
    {3092, {"PULLRING.BAN_0", 0, 0, false } },
    {3020, {"PULLRING.BAN_1", 0, 0, false } },
    {3060, {"PULLRING.BAN_2", 0, 0, false } },
    {240, {"SHADOW.BAN_0", 0, 0, false } },
    {320, {"SHELL.BAN_0", 0, 0, false } },
    {2808, {"SLURG.BAN_0", 0, 0, false } },
    {2708, {"SLURG.BAN_1", 0, 0, false } },
    {2740, {"SLURG.BAN_2", 0, 0, false } },
    {2772, {"SLURG.BAN_3", 0, 0, false } },
    {288, {"SPLINE.BAN_0", 0, 0, false } },
    {276, {"SPLINE.BAN_1", 0, 0, false } },
    {1536, {"SPOTLITE.BAN_0", 0, 0, false } },
    {5084, {"SQBSMK.BAN_0", 0, 0, false } },
    {3228, {"STATICON.BAN_0", 0, 0, false } },
    {3204, {"STATICON.BAN_1", 0, 0, false } },
    {3216, {"STATICON.BAN_2", 0, 0, false } },
    {252, {"STATUSLT.BAN_0", 0, 0, false } },
    {240, {"STATUSLT.BAN_1", 0, 0, false } },
    {1704, {"STICK.BAN_0", 0, 0, false } },
    {5848, {"SWITCH1.BAN_0", 0, 0, false } },
    {5696, {"SWITCH1.BAN_1", 0, 0, false } },
    {5708, {"SWITCH1.BAN_2", 0, 0, false } },
    {5760, {"SWITCH1.BAN_3", 0, 0, false } },
    {5796, {"SWITCH1.BAN_4", 0, 0, false } },
    {8192, {"TBOMB.BAN_0", 0, 0, false } },
    {8048, {"TBOMB.BAN_1", 0, 0, false } },
    {8120, {"TBOMB.BAN_2", 0, 0, false } },
    {500, {"TEARS.BAN_0", 0, 0, false } },
    {588, {"TRIGGER.BAN_0", 0, 0, false } },
    {568, {"TRIGGER.BAN_1", 0, 0, false } },
    {104, {"WDROP.BAN_0", 0, 0, false } },
    {332, {"SPLASH.BAN_0", 0, 0, false } },
    {476, {"WELLLEAF.BAN_0", 0, 0, false } },
    {2728, {"WORKWHEL.BAN_0", 0, 0, false } },
    {2716, {"WORKWHEL.BAN_1", 0, 0, false } },
    {6004, {"BTHROW.BND_0", 0, 0, false } },
    {31656, {"BTHROW.BND_0", 0, 0, false } },
    {31392, {"BTHROW.BND_1", 0, 0, false } },
    {31432, {"BTHROW.BND_2", 0, 0, false } },
    {31504, {"BTHROW.BND_3", 0, 0, false } },
    {31576, {"BTHROW.BND_4", 0, 0, false } },
    {31632, {"BTHROW.BND_5", 0, 0, false } },
    {456, {"BTHROW.BND_0", 0, 0, false } },
    {848, {"BOMB.BND_0", 0, 0, false } },
    {836, {"BOMB.BND_1", 0, 0, false } },
    {556, {"BOMB.BND_0", 0, 0, false } },
    {544, {"BOMB.BND_1", 0, 0, false } },
    {784, {"MINE.BND_0", 0, 0, false } },
    {400, {"MINE.BND_0", 0, 0, false } },
    {8192, {"UXB.BND_0", 0, 0, false } },
    {8048, {"UXB.BND_1", 0, 0, false } },
    {8120, {"UXB.BND_2", 0, 0, false } },
    {556, {"UXB.BND_0", 0, 0, false } },
    {544, {"UXB.BND_1", 0, 0, false } },
    {6520, {"EXPLODE.BND_0", 0, 0, false } },
    {6484, {"EXPLODE.BND_0", 0, 0, false } },
    {51588, {"EXPLODE.BND_0", 0, 0, false } },
    {5280, {"MUDWORK.BND_0", 0, 0, false } },
    {5236, {"MUDWORK.BND_1", 0, 0, false } },
    {5256, {"MUDWORK.BND_2", 0, 0, false } },
    {5328, {"MUDWORK.BND_0", 0, 0, false } },
    {4168, {"PORTAL.BND_0", 0, 0, false } },
    {4068, {"PORTAL.BND_1", 0, 0, false } },
    {4144, {"PORTAL.BND_2", 0, 0, false } },
    {4256, {"PORTAL.BND_0", 0, 0, false } },
    {13576, {"PORTAL.BND_0", 0, 0, false } },
    {8772, {"SHRYPORT.BND_0", 0, 0, false } },
    {8732, {"SHRYPORT.BND_1", 0, 0, false } },
    {82824, {"SHRYPORT.BND_0", 0, 0, false } },
    {82676, {"SHRYPORT.BND_1", 0, 0, false } },
    {82712, {"SHRYPORT.BND_2", 0, 0, false } },
    {15632, {"MIP01C20.CAM_0", 0, 0, false } },
    {15632, {"MIP01C24.CAM_0", 0, 0, false } },
    {15632, {"MIP01C25.CAM_0", 0, 0, false } },
    {15632, {"MIP02C04.CAM_0", 0, 0, false } },
    {15632, {"MIP03C01.CAM_0", 0, 0, false } },
    {15632, {"MIP03C03.CAM_0", 0, 0, false } },
    {15632, {"MIP03C05.CAM_0", 0, 0, false } },
    {29772, {"MIP04C01.CAM_0", 0, 0, false } },
    {29700, {"MIP04C01.CAM_1", 0, 0, false } },
    {29748, {"MIP04C01.CAM_2", 0, 0, false } },
    {7104, {"MIP04C18.CAM_0", 0, 0, false } },
    {29772, {"MIP04C29.CAM_0", 0, 0, false } },
    {29700, {"MIP04C29.CAM_1", 0, 0, false } },
    {29748, {"MIP04C29.CAM_2", 0, 0, false } },
    {15632, {"MIP04C32.CAM_0", 0, 0, false } },
    {29772, {"MIP04C33.CAM_0", 0, 0, false } },
    {29700, {"MIP04C33.CAM_1", 0, 0, false } },
    {29748, {"MIP04C33.CAM_2", 0, 0, false } },
    {29772, {"MIP04C36.CAM_0", 0, 0, false } },
    {29700, {"MIP04C36.CAM_1", 0, 0, false } },
    {29748, {"MIP04C36.CAM_2", 0, 0, false } },
    {7104, {"MIP04C36.CAM_0", 0, 0, false } },
    {1736, {"MIP05C01.CAM_0", 0, 0, false } },
    {23280, {"MIP05C01.CAM_0", 0, 0, false } },
    {1736, {"MIP05C02.CAM_0", 0, 0, false } },
    {23280, {"MIP05C02.CAM_0", 0, 0, false } },
    {1736, {"MIP05C03.CAM_0", 0, 0, false } },
    {23280, {"MIP05C03.CAM_0", 0, 0, false } },
    {1736, {"MIP05C04.CAM_0", 0, 0, false } },
    {23280, {"MIP05C04.CAM_0", 0, 0, false } },
    {1736, {"MIP05C09.CAM_0", 0, 0, false } },
    {23280, {"MIP05C09.CAM_0", 0, 0, false } },
    {1736, {"MIP05C10.CAM_0", 0, 0, false } },
    {23280, {"MIP05C10.CAM_0", 0, 0, false } },
    {964, {"MIP08C06.CAM_0", 0, 0, false } },
    {15632, {"MIP08C20.CAM_0", 0, 0, false } },
    {964, {"MIP08C21.CAM_0", 0, 0, false } },
    {964, {"MIP08C32.CAM_0", 0, 0, false } },
    {964, {"MIP09C18.CAM_0", 0, 0, false } },
    {964, {"MIP11C22.CAM_0", 0, 0, false } },
    {964, {"MIP11C27.CAM_0", 0, 0, false } },
    {29772, {"MIP11C29.CAM_0", 0, 0, false } },
    {29700, {"MIP11C29.CAM_1", 0, 0, false } },
    {29748, {"MIP11C29.CAM_2", 0, 0, false } },
    {1736, {"MIP12C02.CAM_0", 0, 0, false } },
    {23280, {"MIP12C02.CAM_0", 0, 0, false } },
    {964, {"MIP12C02.CAM_0", 0, 0, false } },
    {1736, {"MIP12C03.CAM_0", 0, 0, false } },
    {23280, {"MIP12C03.CAM_0", 0, 0, false } },
    {1736, {"MIP12C04.CAM_0", 0, 0, false } },
    {23280, {"MIP12C04.CAM_0", 0, 0, false } },
    {1736, {"MIP12C09.CAM_0", 0, 0, false } },
    {23280, {"MIP12C09.CAM_0", 0, 0, false } },
    {1736, {"MIP12C10.CAM_0", 0, 0, false } },
    {23280, {"MIP12C10.CAM_0", 0, 0, false } },
    {31656, {"RTHROW.BND_0", 0, 0, false } },
    {31392, {"RTHROW.BND_1", 0, 0, false } },
    {31432, {"RTHROW.BND_2", 0, 0, false } },
    {31504, {"RTHROW.BND_3", 0, 0, false } },
    {31576, {"RTHROW.BND_4", 0, 0, false } },
    {31632, {"RTHROW.BND_5", 0, 0, false } },
    {6004, {"RTHROW.BND_0", 0, 0, false } },
    {488, {"RTHROW.BND_0", 0, 0, false } },
    {20900, {"BAYROLL.BAN_0", 0, 0, false } },
    {20788, {"BAYROLL.BAN_1", 0, 0, false } },
    {20812, {"BAYROLL.BAN_2", 0, 0, false } },
    {20824, {"BAYROLL.BAN_3", 0, 0, false } },
    {20836, {"BAYROLL.BAN_4", 0, 0, false } },
    {20848, {"BAYROLL.BAN_5", 0, 0, false } },
    {20872, {"BAYROLL.BAN_6", 0, 0, false } },
    {29772, {"ROCKBAG.BAN_0", 0, 0, false } },
    {29700, {"ROCKBAG.BAN_1", 0, 0, false } },
    {29748, {"ROCKBAG.BAN_2", 0, 0, false } },
    {488, {"PUIROCK.BAN_0", 0, 0, false } },
    {4840, {"TRAPDOOR.BAN_0", 0, 0, false } },
    {4788, {"TRAPDOOR.BAN_1", 0, 0, false } },
    {4800, {"TRAPDOOR.BAN_2", 0, 0, false } },
    {4812, {"TRAPDOOR.BAN_3", 0, 0, false } },
    {748, {"ROPES.BAN_0", 0, 0, false } },
    {148, {"WEB.BAN_0", 0, 0, false } },
    {4808, {"MILIFT.BND_0", 0, 0, false } },
    {4748, {"MILIFT.BND_1", 0, 0, false } },
    {4760, {"MILIFT.BND_2", 0, 0, false } },
    {4784, {"MILIFT.BND_3", 0, 0, false } },
    {1936, {"MILIFT.BND_0", 0, 0, false } },
    {6736, {"DRILL.BAN_0", 0, 0, false } },
    {6676, {"DRILL.BAN_1", 0, 0, false } },
    {6688, {"DRILL.BAN_2", 0, 0, false } },
    {6712, {"DRILL.BAN_3", 0, 0, false } },
    {2688, {"SLAM.BAN_0", 0, 0, false } },
    {2640, {"SLAM.BAN_1", 0, 0, false } },
    {2656, {"SLAM.BAN_2", 0, 0, false } },
    {2672, {"SLAM.BAN_3", 0, 0, false } },
    {15632, {"MINEFAN.BAN_0", 0, 0, false } },
    {96876, {"SLOG.BND_0", 0, 0, false } },
    {96344, {"SLOG.BND_1", 0, 0, false } },
    {96424, {"SLOG.BND_2", 0, 0, false } },
    {96464, {"SLOG.BND_3", 0, 0, false } },
    {96496, {"SLOG.BND_4", 0, 0, false } },
    {96532, {"SLOG.BND_5", 0, 0, false } },
    {96580, {"SLOG.BND_6", 0, 0, false } },
    {96640, {"SLOG.BND_7", 0, 0, false } },
    {96660, {"SLOG.BND_8", 0, 0, false } },
    {96680, {"SLOG.BND_9", 0, 0, false } },
    {96692, {"SLOG.BND_10", 0, 0, false } },
    {96716, {"SLOG.BND_11", 0, 0, false } },
    {96728, {"SLOG.BND_12", 0, 0, false } },
    {96752, {"SLOG.BND_13", 0, 0, false } },
    {96764, {"SLOG.BND_14", 0, 0, false } },
    {96804, {"SLOG.BND_15", 0, 0, false } },
    {15156, {"SLOG.BND_0", 0, 0, false } },
    {15068, {"SLOG.BND_1", 0, 0, false } },
    {15108, {"SLOG.BND_2", 0, 0, false } },
    {15132, {"SLOG.BND_3", 0, 0, false } },
    {39064, {"SLOG.BND_0", 0, 0, false } },
    {38904, {"SLOG.BND_1", 0, 0, false } },
    {38960, {"SLOG.BND_2", 0, 0, false } },
    {12812, {"SLOG.BND_0", 0, 0, false } },
    {12724, {"SLOG.BND_1", 0, 0, false } },
    {12412, {"DOGKNFD.BAN_0", 0, 0, false } },
    {7544, {"DOGBLOW.BAN_0", 0, 0, false } },
    {7504, {"DOGBLOW.BAN_1", 0, 0, false } },
};

static std::vector<PreviewTexture> gPreviewTextures;
static std::vector<PreviewExternalTexture> gExternalPreviewTextures;
static std::vector<Bounds> gPreviewTexturesBounds;
static AssetMeta gExternalPreviewMeta;
static std::vector<LoadedAnimRecord> gLoadedAnimRecords;
static int gActiveToolIndex = 0;
static int gPreviewFPS = 0;
static std::vector<RGBAPixel> gLastPalette;
static int gTableIndex = -1;
static float gElapsedSeconds = 0.0f;
static bool gPingPong = false;
static bool gPonging = false;
static int gResetFrame = 0;
static int gCurrentFrame = 0;
static bool gIsAo = false;
static bool gExternalPreviewEnabled = true;
static AnimRecord gSelectedRecord;
static std::vector<AppTool> gTools;
static std::string gLoadedLevelName = "";
static bool gHasAeFiles = false;
static bool gHasAoFiles = false;

// Exporting
static std::thread gExportThread;
static bool gExporting = false;
static float gExportProgress = 0;
static std::string gExportMessage = "";
static bool gShowExportPreDialog = false;

// Message Box
static bool gShowMessageBox = false;
static std::string gMessageBoxTitle = "";
static std::string gMessageBoxText = "";

///// Helper Functions /////

static void CheckDirectory(std::string path)
{
    std::filesystem::path dir(path);
    if (!std::filesystem::exists(dir))
    {
        std::filesystem::create_directories(dir);
    }
}

static bool FileExists(std::string filePath)
{
    std::ifstream file(filePath);
    bool exists = file.good();
    file.close();
    return exists;
}

static bool FolderExists(std::string path)
{
    fs::path p(path);
    return fs::exists(p) && fs::is_directory(p);
}

/////

static void ConvertPalette(const u8* srcPalData, RGBAPixel* dst, s32 palDepth)
{
    const u16* palShortPtr = reinterpret_cast<const u16*>(srcPalData);
    for (s32 i = 0; i < palDepth; i++)
    {
        const u16 oldPixel = palShortPtr[i];

        dst[i].R = static_cast<u8>((((oldPixel >> 0) & 0x1F)) << 3);
        dst[i].G = static_cast<u8>((((oldPixel >> 5) & 0x1F)) << 3);
        dst[i].B = static_cast<u8>((((oldPixel >> 10) & 0x1F)) << 3);
        dst[i].A = static_cast<u8>((((((oldPixel) >> 15) & 0xffff)) ? 127 : 255));
    }

    dst[0] = { 0,0,0,0 };
}

static GLuint GenTexture()
{
    GLuint texId = 0;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return texId;
}

static void LoadLevel(const char* path)
{
    sLvlArchive_5BC520.Free_433130();
    if (!sLvlArchive_5BC520.Open_Archive_432E80(path))
    {
        ALIVE_FATAL("Failed to open LVL!");
    }

    // get filename from path
    std::string lvlName = path;
    auto pos = lvlName.find_last_of('/');
    if (pos != std::string::npos)
    {
        lvlName = lvlName.substr(pos + 1);
    }
    gLoadedLevelName = lvlName;

    gLoadedAnimRecords.clear();
    gTableIndex = -1;

    for (int i = 1; i < static_cast<int>(AnimId::Anim_Tester); i++)
    {
        AnimRecord record;

        if (gIsAo)
        {
            if (!AnimRecExists(false, static_cast<AnimId>(i)))
            {
                continue;
            }
            record = AO::AnimRec(static_cast<AnimId>(i));
        }
        else
        {
            if (!AnimRecExists(true, static_cast<AnimId>(i)))
            {
                continue;
            }
            record = AnimRec(static_cast<AnimId>(!gIsAo, i));
        }

        if (record.mBanName == nullptr)
            continue;

        if (sLvlArchive_5BC520.Find_File_Record_433160(record.mBanName) == nullptr)
        {
            continue;
        }

        std::string listItemName = static_cast<std::string>(magic_enum::enum_name(static_cast<AnimId>(i)));

        gLoadedAnimRecords.push_back({ listItemName, record });
    }
}

static void InitResourceManager()
{
    // Hacky stuff to get the resource manager to load.
    sObjectIds_5C1B70.ctor_449AE0(101);

    gBaseGameObject_list_BB47C4 = ae_new<DynamicArrayT<BaseGameObject>>();
    gBaseGameObject_list_BB47C4->ctor_40CA60(50);

    gObjList_drawables_5C1124 = ae_new<DynamicArrayT<BaseGameObject>>();
    gObjList_drawables_5C1124->ctor_40CA60(30);

    gObjList_animations_5C1A24 = ae_new<DynamicArrayT<AnimationBase>>();
    gObjList_animations_5C1A24->ctor_40CA60(30);

    pResourceManager_5C1BB0 = ae_new<ResourceManager>();
    pResourceManager_5C1BB0->ctor_464910();

    ResourceManager::Init_49BCE0();

    if (FileExists("mi.lvl"))
    {
        LoadLevel("mi.lvl");
        gIsAo = false;
    }
    else if (FileExists("r1.lvl"))
    {
        LoadLevel("r1.lvl");
        gIsAo = true;
    }
    else
    {
        ALIVE_FATAL("mi.lvl or r1.lvl not found! Please run this program from the root of the game directory.");
    }

    if (FileExists("mi.lvl"))
    {
        gHasAeFiles = true;
    }
    if (FileExists("r1.lvl"))
    {
        gHasAoFiles = true;
    }
}

static Bounds BoundsUnion(Bounds a, Bounds b)
{
    Bounds result;
    result.x = min(a.x, b.x);
    result.y = min(a.y, b.y);
    result.w = max(a.x + a.w, b.x + b.w) - result.x;
    result.h = max(a.y + a.h, b.y + b.h) - result.y;
    return result;
}

// Takes an anim and returns the bounds of the animation
static Bounds CalculateAnimationBounds(std::vector<Bounds>& boundsList, int padding = 0)
{
    Bounds bounds = { 0,0,0,0 };
    bool first = true;

    for (auto& frame : boundsList)
    {
        Bounds frameBounds = { frame.x - padding, frame.y - padding, frame.w + (padding * 2), frame.h + (padding * 2) };

        if (first)
        {
            bounds = frameBounds;
            first = false;
        }

        bounds = BoundsUnion(bounds, frameBounds);
    }

    return bounds;
}

static PreviewTexture DecodedFrameToGL(std::vector<RGBAPixel>& pixelData, int width, int height)
{
    GLuint newTex = GenTexture();
    glBindTexture(GL_TEXTURE_2D, newTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData.data());

    return { newTex, width, height };
}

static std::vector<RGBAPixel> DecodeFrame(const FrameHeader* pFrameHeader, int * width, int height, std::vector<RGBAPixel>& palette)
{
    bool uncompressed = false;
    std::vector<RGBAPixel> pixelData;
    pixelData.resize(*width * height);

    std::vector<u8> convertedData;
    convertedData.resize(*width * height * 3);

    std::vector<u8> newData;

    switch (pFrameHeader->field_7_compression_type)
    {
    case CompressionType::eType_0_NoCompression:
        
        if (palette.size() == 16)
        {
            *width = ((int)(ceil(*width / 8.0))) * 8;
            pixelData.resize(*width * height);
            for (int i = 0; i < *width * height; i++)
            {
                newData.push_back(reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2)[i] & 0x0f);
                newData.push_back((reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2)[i] & 0xf0) >> 4);
            }
        }
        else
        {
            pixelData.resize(*width * height);
            for (int i = 0; i < *width * height; i++)
            {
                newData.push_back(reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2)[i]);
            }
        }
        convertedData = newData;
        break;
    case CompressionType::eType_1_NotUsed:
        // This isn't in any of the animation data files on disk, therefore can't ever be used.
        ALIVE_FATAL("Compression type 1 never expected to be used.");
        break;

    case CompressionType::eType_2_ThreeToFourBytes:
        CompressionType2_Decompress_40AA50(
            reinterpret_cast<const u8*>(&pFrameHeader[1]),
            convertedData.data(),
            *width * pFrameHeader->field_5_height * 2);
        break;

    case CompressionType::eType_3_RLE_Blocks:
        if (gIsAo)
        {
            AO::Decompress_Type_3_4031E0(
                (u8*)&pFrameHeader[1],
                convertedData.data(),
                *(u32*)&pFrameHeader->field_8_width2,
                2 * pFrameHeader->field_5_height * *width);
        }
        else
        {
            CompressionType_3Ae_Decompress_40A6A0(reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2), convertedData.data());
        }

        break;

    case CompressionType::eType_4_RLE:
    case CompressionType::eType_5_RLE:
        CompressionType_4Or5_Decompress_4ABAB0(reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2), convertedData.data());
        
        if (palette.size() == 16)
        {
            *width = ((int)(ceil(*width / 8.0))) * 8;
            pixelData.resize(*width * height);
            for (int i = 0; i < *width * height; i++)
            {
                newData.push_back(convertedData[i] & 0x0f);
                newData.push_back((convertedData[i] & 0xf0) >> 4);
            }
            convertedData = newData;
        }
        
        break;

    case CompressionType::eType_6_RLE:
        CompressionType6Ae_Decompress_40A8A0(reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2), convertedData.data());
        *width = ((int)(ceil(*width / 8.0))) * 8;
        pixelData.resize(*width * height);
        for (int i = 0; i < *width * height; i++)
        {
            newData.push_back(convertedData[i] & 0x0f);
            newData.push_back((convertedData[i] & 0xf0) >> 4);
        }
        convertedData = newData;
        break;

    case CompressionType::eType_7_NotUsed:
    case CompressionType::eType_8_NotUsed:
        ALIVE_FATAL("Decompression 7 and 8 never expected to be used");
        break;
    }

    if (!uncompressed)
    {
        for (int p = 0; p < pixelData.size(); p++)
        {
            pixelData[p] = palette[convertedData[p]];
        }
    }

    return pixelData;
}

static std::vector<RGBAPixel> ReadPalette(u8** ppRes, const FrameHeader* pFrameHeader)
{
    u8* pClut = &(*ppRes)[pFrameHeader->field_0_clut_offset];

    int palDepth = 0;

    switch (pFrameHeader->field_6_colour_depth)
    {
    case 4:
        palDepth = 16;
        break;
    case 8:
        if (*(u32*)pClut != 64)
        {
            palDepth = 256;
        }
        else
        {
            palDepth = 64;
        }
        break;
    case 16:
        ALIVE_FATAL("Todo: this");
        break;
    }

    std::vector<RGBAPixel> palette;
    palette.resize(palDepth);
    ConvertPalette(pClut + 4, palette.data(), palDepth);

    return palette;
}

static bool DumpAnim(std::string path, AnimRecord& rec)
{
    ResourceManager::Free_Resource_Of_Type_49C6B0(ResourceManager::Resource_Animation);
    ResourceManager::Free_Resource_Of_Type_49C6B0(ResourceManager::Resource_Palt);

    LOG_INFO("Dumping Anim to " << path);
    std::vector<Bounds> textureBoundList;

    if (sLvlArchive_5BC520.Find_File_Record_433160(rec.mBanName) == nullptr)
        return false;

    ResourceManager::LoadResourceFile_49C170(rec.mBanName, nullptr);
    u8** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, rec.mResourceId, 0, 0);

    if (ppRes == nullptr)
        return false;

    AnimationHeader* pAnimHeader = reinterpret_cast<AnimationHeader*>(*ppRes + rec.mFrameTableOffset);
    gPreviewFPS = pAnimHeader->field_0_fps; 
    gResetFrame = pAnimHeader->field_4_loop_start_frame;

    if (pAnimHeader->field_0_fps > 100 || pAnimHeader->field_0_fps < 0 || gResetFrame > 100 || gResetFrame < 0 ||
        pAnimHeader->field_2_num_frames < 0 || pAnimHeader->field_2_num_frames > 255)
    {
        // These are not valid values, and so something is up. Try not to crash!
        return false;
    }

    // We have to calculate the bounds first.
    for (int i = 0; i < pAnimHeader->field_2_num_frames; i++)
    {
        FrameInfoHeader* pFrameInfo = reinterpret_cast<FrameInfoHeader*>(*ppRes + pAnimHeader->mFrameOffsets[i]);
        const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*ppRes)[pFrameInfo->field_0_frame_header_offset]);

        int width = static_cast<int>(ceil(pFrameHeader->field_4_width / 4.0f) * 4);
        int height = pFrameHeader->field_5_height;
        int offsetX = pFrameInfo->field_8_data.offsetAndRect.mOffset.x;
        int offsetY = pFrameInfo->field_8_data.offsetAndRect.mOffset.y;

        if (gIsAo)
        {
            offsetX = PsxToPCX(offsetX);
        }
        textureBoundList.push_back({ offsetX, offsetY, width, height });
    }

    Bounds bounds = CalculateAnimationBounds(textureBoundList, 8);
    CheckDirectory(path);
    AssetMeta metaInfo = {
        bounds.w, // width
        bounds.h * 2, // height
        (-bounds.x), // offset x
        ((-bounds.y) * 2), // offset y,
        static_cast<int>(pAnimHeader->field_2_num_frames) // frame count
    };
    metaInfo.SaveJSONToFile(path + "/meta.json");

    // Now we start to convert stuff
    for (int i = 0; i < pAnimHeader->field_2_num_frames; i++)
    {
        FrameInfoHeader* pFrameInfo = reinterpret_cast<FrameInfoHeader*>(*ppRes + pAnimHeader->mFrameOffsets[i]);
        const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*ppRes)[pFrameInfo->field_0_frame_header_offset]);


        std::vector<RGBAPixel> palette = ReadPalette(ppRes, pFrameHeader);

        if (rec.mPalOverride != PalId::Default)
        {
            PalRecord palOverride = PalRec(rec.mPalOverride);

            if (sLvlArchive_5BC520.Find_File_Record_433160(palOverride.mBanName) == nullptr)
            {
                LOG_ERROR("Couldn't find palette resource ban " << palOverride.mBanName);
                ResourceManager::FreeResource_49C330(ppRes);
                return false;
            }

            ResourceManager::LoadResourceFile_49C170(palOverride.mBanName, nullptr);
            u8** ppPalResource = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, palOverride.mResourceId, 1, 1);

            ConvertPalette(*ppPalResource + 4, palette.data(), static_cast<int>(palette.size()));

            ResourceManager::FreeResource_49C330(ppPalResource);
        }

        gLastPalette = palette;

        int width = static_cast<int>(ceil(pFrameHeader->field_4_width / 4.0f) * 4);
        int height = pFrameHeader->field_5_height;

        auto frameConverted = DecodeFrame(pFrameHeader, &width, height, palette);

        int offsetX = pFrameInfo->field_8_data.offsetAndRect.mOffset.x;
        int offsetY = pFrameInfo->field_8_data.offsetAndRect.mOffset.y;

        if (gIsAo)
        {
            offsetX = PsxToPCX(offsetX);
        }

        std::vector<RGBAPixel> dstImage;
        dstImage.resize(bounds.w * (bounds.h * 2));

        int copyX = offsetX - bounds.x;
        int copyY = (offsetY - bounds.y) * 2;

        int srcY = 0;
        for (int y = copyY; y < copyY + (height * 2); y++)
        {
            memcpy(&dstImage[(y * bounds.w) + copyX], &frameConverted[srcY * width], width * sizeof(RGBAPixel));
            memcpy(&dstImage[((y + 1) * bounds.w) + copyX], &frameConverted[srcY * width], width * sizeof(RGBAPixel));

            y++;
            srcY++;
        }

        stbi_write_png((path + "/" + std::to_string(i) + ".png").c_str(), bounds.w, bounds.h * 2, 4, dstImage.data(), bounds.w * 4);
    }

    ResourceManager::FreeResource_49C330(ppRes);

    return true;
}

static GLuint LoadImageToGL(std::string path, int * width, int * height)
{
    int x = 0, y = 0, comp = 0;

    std::vector<BYTE> fileData;

    // Try to keep all paths and filenames lowercase for our linux friends.
    std::ifstream file(path, std::ios::binary);

    if (file.is_open())
    {
        fileData = std::vector<BYTE>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
    }

    const unsigned char* data = stbi_load_from_memory(fileData.data(), static_cast<int>(fileData.size()), &x, &y, &comp, 4);

    GLuint newTexture = 0;
    glGenTextures(1, &newTexture);

    glBindTexture(GL_TEXTURE_2D, newTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free((void*)data);

    if (width != nullptr)
        *width = x;

    if (height != nullptr)
        *height = y;

    return newTexture;
}

static void LoadAnimExternalPreview(AnimRecord& rec)
{
    for (auto& cache : gExternalPreviewTextures)
    {
        glDeleteTextures(1, &cache.id);
    }

    gExternalPreviewTextures.clear();

    std::string folderName = std::string(magic_enum::enum_name<AnimId>(rec.mId));
    std::string folderPath = "hd/sprites/" + folderName;

    // check if folder path exists
    if (!FolderExists(folderPath))
    {
        return;
    }

    gExternalPreviewMeta.LoadJSONFromFile(folderPath + "/meta.json");

    for (int i = 0; i < gExternalPreviewMeta.frame_count; i++)
    {
        gExternalPreviewTextures.push_back({ LoadImageToGL(folderPath + "/" + std::to_string(i) + ".png", nullptr, nullptr) });
    }
}

static void LoadAnimPreview(AnimRecord& rec)
{
    for (auto& cache : gPreviewTextures)
    {
        glDeleteTextures(1, &cache.id);
    }

    LoadAnimExternalPreview(rec);

    gPreviewTextures.clear();
    gPreviewTexturesBounds.clear();
    gLastPalette.clear();
    gCurrentFrame = 0;
    gPonging = false;
    gPingPong = false;
    gResetFrame = 0;

    if (sLvlArchive_5BC520.Find_File_Record_433160(rec.mBanName) == nullptr)
        return;

    ResourceManager::LoadResourceFile_49C170(rec.mBanName, nullptr);
    u8** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 1);

    if (ppRes == nullptr)
        return;

    AnimationHeader* pAnimHeader = reinterpret_cast<AnimationHeader*>(*ppRes + rec.mFrameTableOffset);
    gPreviewFPS = pAnimHeader->field_0_fps;
    gResetFrame = pAnimHeader->field_4_loop_start_frame;

    if (pAnimHeader->field_0_fps > 100 || pAnimHeader->field_0_fps < 0 || gResetFrame > 100 || gResetFrame < 0 ||
        pAnimHeader->field_2_num_frames < 0 || pAnimHeader->field_2_num_frames > 255)
    {
        // These are not valid values, and so something is up. Try not to crash!
        return;
    }

    for (int i = 0; i < pAnimHeader->field_2_num_frames; i++)
    {
        FrameInfoHeader* pFrameInfo = reinterpret_cast<FrameInfoHeader*>(*ppRes + pAnimHeader->mFrameOffsets[i]);
        const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*ppRes)[pFrameInfo->field_0_frame_header_offset]);

        std::vector<RGBAPixel> palette = ReadPalette(ppRes, pFrameHeader);

        if (rec.mPalOverride != PalId::Default)
        {
            PalRecord palOverride = PalRec(rec.mPalOverride);

            if (sLvlArchive_5BC520.Find_File_Record_433160(palOverride.mBanName) == nullptr)
            {
                LOG_ERROR("Couldn't find palette resource ban " << palOverride.mBanName);
                return;
            }

            ResourceManager::LoadResourceFile_49C170(palOverride.mBanName, nullptr);
            u8** ppPalResource = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, palOverride.mResourceId, 1, 1);

            ConvertPalette(*ppPalResource + 4, palette.data(), static_cast<int>(palette.size()));

            ResourceManager::FreeResource_49C330(ppPalResource);
        }

        gLastPalette = palette;

        int width = static_cast<int>(ceil(pFrameHeader->field_4_width / 4.0f) * 4);
        int height = pFrameHeader->field_5_height;

        auto frameConverted = DecodeFrame(pFrameHeader, &width, height, palette);
        PreviewTexture newTexture = DecodedFrameToGL(frameConverted, width, height);

        newTexture.offsetX = pFrameInfo->field_8_data.offsetAndRect.mOffset.x;
        newTexture.offsetY = pFrameInfo->field_8_data.offsetAndRect.mOffset.y;

        if (gIsAo)
        {
            newTexture.offsetX = PsxToPCX(newTexture.offsetX);
        }

        gPreviewTextures.push_back(newTexture);
        gPreviewTexturesBounds.push_back({ newTexture.offsetX, newTexture.offsetY, width, height });
    }

    ResourceManager::FreeResource_49C330(ppRes);
}

static void ToolTip(std::string& message)
{
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(message.c_str());
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
}

static void AppAnimationViewer(ImGuiWindowFlags flags, float elapsedTime)
{
    ImVec2 windowPadding = ImGui::GetStyle().WindowPadding;

    if (ImGui::Begin("Animation Viewer", 0, flags))
    {
        ImVec2 windowSize = ImGui::GetContentRegionAvail();
        float listBoxSize = 268.0f;
        if (ImGui::BeginChild("##list_frame", { listBoxSize , windowSize.y }, false))
        {
            static std::string searchQuery;
            ImGui::PushItemWidth(-1);
            ImGui::InputTextWithHint("##list_search", "Search...", &searchQuery);
            ImGui::PopItemWidth();

            if (ImGui::BeginListBox("##resource_list", ImVec2(-FLT_MIN, ImGui::GetContentRegionAvail().y - windowPadding.y)))
            {
                for (auto i = 0; i < gLoadedAnimRecords.size(); i++)
                {
                    if (!searchQuery.empty() && StringToLowerCase(gLoadedAnimRecords[i].name).find(StringToLowerCase(searchQuery)) == std::string::npos)
                        continue;

                    if (ImGui::Selectable(gLoadedAnimRecords[i].name.c_str(), i == gTableIndex))
                    {
                        gTableIndex = i;
                        gSelectedRecord = gLoadedAnimRecords[i].record;
                        LoadAnimPreview(gLoadedAnimRecords[i].record);
                    }
                }

                ImGui::EndListBox();
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        if (ImGui::BeginChild("##content_panel", { windowSize.x - listBoxSize , windowSize.y }, false))
        {
            ImVec2 subContentViewSize = ImGui::GetContentRegionAvail();
            float animFramesChildSize = 150;

            if (ImGui::BeginChild("##frame_view", { -FLT_MIN , animFramesChildSize - windowPadding.y }, true))
            {
                float frameHeight = ImGui::GetContentRegionAvail().y;

                int i = 0;
                for (auto t : gPreviewTextures)
                {
                    ImGui::SetCursorPosY((frameHeight * 0.5f) - t.height);
                    ImGui::Image(reinterpret_cast<ImTextureID>(t.id), { (float)(t.width), (float)(t.height * 2) }, { 0,0 }, { 1,1 }, { 1,1,1,1 }, ((gCurrentFrame == i) ? ImVec4{ 1, 1, 0.5f, 0.7f } : ImVec4{ 1, 1, 1, 0.5f }));
                    ImGui::SameLine();

                    if (ImGui::IsItemHovered())
                    {
                        std::string tipText = "Frame: " + std::to_string(i + 1);
                        tipText += " Size: " + std::to_string(t.width) + "x" + std::to_string(t.height);
                        tipText += " Offset: " + std::to_string(t.offsetX) + ", " + std::to_string(t.offsetY);

                        ToolTip(tipText);
                    }

                    i++;
                }
            }
            ImGui::EndChild();

            if (ImGui::BeginChild("##preview", { subContentViewSize.x , subContentViewSize.y - animFramesChildSize }, true) && gTableIndex >= 0)
            {

                static float scale = 1.0f;
                static bool gShowBounds = false;
                ImVec2 windowPos = ImGui::GetCursorPos();
                ImVec2 availableWindowSpace = ImGui::GetContentRegionAvail();

                // Animation Scale buttons
                if (ImGui::Button("-"))
                {
                    if (scale > 1)
                    {
                        scale--;
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("+"))
                {
                    scale++;
                }
                ImGui::SameLine();
                ImGui::Text("|");
                ImGui::SameLine();
                ImGui::Checkbox("Show Bounds", &gShowBounds);
                ImGui::SameLine();
                ImGui::Text("|");
                ImGui::SameLine();
                
                if (gExternalPreviewTextures.size() > 0)
                {
                    ImGui::Checkbox("Show External", &gExternalPreviewEnabled);
                }
                else
                {
                    ImGui::TextDisabled("No External");
                }

                ImGui::SameLine();
                ImGui::Text("|");
                ImGui::SameLine();

                std::string animExportPath = "hd/sprites/" + gLoadedAnimRecords[gTableIndex].name;

                if (ImGui::Button("Export"))
                {
                    // check if export folder (animExportPath) already exists
                    if (FolderExists(animExportPath))
                    {
                        ImGui::OpenPopup("Are you sure?##export_single_sure");
                    }
                    {
                        DumpAnim(animExportPath, gSelectedRecord);
                    }
                }

                if (ImGui::BeginPopup("Are you sure?##export_single_sure", ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::Text("There is an existing export of the current animation.\n Are you sure you want to overwrite it?");

                    if (ImGui::Button("Yes"))
                    {
                        DumpAnim(animExportPath, gSelectedRecord);
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("No"))
                    {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }

                const int toolSectionWidth = 300;

                if (gExternalPreviewTextures.size() > 0)
                {
                    ImGui::SetCursorPos({ windowPos.x + (availableWindowSpace.x - toolSectionWidth), windowPos.y });
                    ImGui::BeginChild("##external_editor", { toolSectionWidth, -FLT_MIN }, false);
                    ImGui::Separator();

                    // edit all the attributes of the selected meta
                    static int step = 1;
                    static int stepFast = 5;
                    ImGui::InputScalarN("Ref Image Size", ImGuiDataType_S32, &gExternalPreviewMeta.size_width, 2, &step, &stepFast);
                    ImGui::InputScalarN("Offset", ImGuiDataType_S32, &gExternalPreviewMeta.offset_x, 2, &step, &stepFast);

                    ImGui::Separator();

                    if (ImGui::Button("Save"))
                    {
                        gExternalPreviewMeta.SaveJSONToFile("hd/sprites/" + std::string(magic_enum::enum_name<AnimId>(gSelectedRecord.mId)) + "/meta.json");
                    }
                    ImGui::EndChild();
                }

                static ImVec2 previewWindowOffset(0, 0);

                ImGui::SetCursorPos(windowPos);
                ImVec2 windowScreenPos = ImGui::GetCursorScreenPos();
                ImDrawList* draw_list = ImGui::GetWindowDrawList();


                // Render palette colors to the bottom of the preview
                ImVec2 paletteSize = { availableWindowSpace.x / gLastPalette.size(), 32 };

                int i = 0;
                for (auto p : gLastPalette)
                {
                    ImVec2 offset = { windowScreenPos.x + (i * (availableWindowSpace.x / gLastPalette.size())), windowScreenPos.y + availableWindowSpace.y - paletteSize.x };

                    draw_list->AddRectFilled(offset, { offset.x + paletteSize.x, offset.y + paletteSize.x }, IM_COL32(p.R, p.G, p.B, p.A));

                    i++;
                }

                // Animate and render the anim
                if (gPreviewTextures.size() > 0)
                {
                    Bounds bounds = CalculateAnimationBounds(gPreviewTexturesBounds, 8);
                    bounds.y *= 2;
                    bounds.h *= 2;

                    // Render the background reference lines
                    float floorOffset = (availableWindowSpace.y / 2) - ((bounds.h * scale) / 2) - (bounds.y * scale);

                    draw_list->AddLine({ windowScreenPos.x + (availableWindowSpace.x / 2),  windowScreenPos.y }, { windowScreenPos.x + (availableWindowSpace.x / 2),  windowScreenPos.y + availableWindowSpace.y }, IM_COL32(255, 255, 255, 60), 2.0f);
                    draw_list->AddLine({ windowScreenPos.x,  windowScreenPos.y + floorOffset }, { windowScreenPos.x + availableWindowSpace.x,  windowScreenPos.y + floorOffset }, IM_COL32(255, 255, 255, 120), 2.0f);

                    static float frameSkipDelta = 0;

                    frameSkipDelta += gElapsedSeconds * 30;

                    if (frameSkipDelta >= gPreviewFPS) // todo get proper frame delays
                    {
                        frameSkipDelta = 0;

                        if (gPonging)
                        {
                            gCurrentFrame--;

                            if (gCurrentFrame < 0)
                            {
                                gCurrentFrame = static_cast<int>(gPreviewTextures.size());
                            }
                        }
                        else
                        {
                            gCurrentFrame++;

                            if (gCurrentFrame >= gPreviewTextures.size())
                            {
                                gCurrentFrame = gResetFrame;
                            }
                        }
                    }

                    float offsetXScaled = gPreviewTextures[gCurrentFrame].offsetX * scale;
                    float offsetYScaled = gPreviewTextures[gCurrentFrame].offsetY * scale;

                    ImVec2 imagePos = { (availableWindowSpace.x / 2.0f) + offsetXScaled, floorOffset + (offsetYScaled * 2) };
                    ImVec2 imageSize = { (float)(gPreviewTextures[gCurrentFrame].width * scale), (float)(gPreviewTextures[gCurrentFrame].height * 2 * scale) };
                    draw_list->AddImage(reinterpret_cast<ImTextureID>(gPreviewTextures[gCurrentFrame].id), { windowScreenPos.x + imagePos.x, windowScreenPos.y + imagePos.y },
                        { windowScreenPos.x + imagePos.x + imageSize.x, windowScreenPos.y + imagePos.y + imageSize.y });

                    if (gExternalPreviewTextures.size() > 0 && gExternalPreviewEnabled)
                    {
                        float offsetXScaled_External = gExternalPreviewMeta.offset_x * scale;
                        float offsetYScaled_External = gExternalPreviewMeta.offset_y * scale;

                        ImVec2 imagePos_External = { (availableWindowSpace.x / 2.0f) - offsetXScaled_External, floorOffset - offsetYScaled_External };
                        ImVec2 imageSize_External = { (float)(gExternalPreviewMeta.size_width * scale), (float)(gExternalPreviewMeta.size_height * scale) };

                        if (gCurrentFrame >= gExternalPreviewTextures.size())
                        {
                            draw_list->AddText({ windowScreenPos.x + imagePos_External.x, windowScreenPos.y + imagePos_External.y }, IM_COL32(255, 0, 0, 255), "MISSING TEXTURE FILE!");
                        }
                        else
                        {
                            draw_list->AddImage(reinterpret_cast<ImTextureID>(gExternalPreviewTextures[gCurrentFrame].id), { windowScreenPos.x + imagePos_External.x, windowScreenPos.y + imagePos_External.y },
                                { windowScreenPos.x + imagePos_External.x + imageSize_External.x, windowScreenPos.y + imagePos_External.y + imageSize_External.y });
                        }
                    }

                    if (gShowBounds)
                    {
                        draw_list->AddRect({ windowScreenPos.x + imagePos.x, windowScreenPos.y + imagePos.y },
                            { windowScreenPos.x + imagePos.x + imageSize.x, windowScreenPos.y + imagePos.y + imageSize.y }, IM_COL32(255, 255, 255, 200));

                        draw_list->AddRect(
                            { windowScreenPos.x + (availableWindowSpace.x / 2.0f) + (bounds.x * scale), windowScreenPos.y + floorOffset + (bounds.y * scale) },
                            { windowScreenPos.x + (availableWindowSpace.x / 2.0f) + ((bounds.x + bounds.w) * scale), windowScreenPos.y + floorOffset + ((bounds.y + bounds.h) * scale) },
                            IM_COL32(50, 255, 50, 200));
                    }
                }
            }
            ImGui::EndChild();
        }
        ImGui::EndChild();

    }
    ImGui::End();
}

static void ShowMessageBox(std::string title, std::string message)
{
    gShowMessageBox = true;
    gMessageBoxTitle = title;
    gMessageBoxText = message;
}

static void export_thread()
{
    std::vector<std::string> dumpList;
    std::vector<std::string> failedDumpList;
    int dumpedAnimsCount = 0;

    int levelCount = sizeof(AeLvls) / sizeof(AeLvls[0]);
    float baseLevelPercent = 1.0f / levelCount;
    const LevelInfo* pLvlInfos = AeLvls;

    if (gIsAo)
    {
        pLvlInfos = AoLvls;
        levelCount = sizeof(AoLvls) / sizeof(AoLvls[0]);
        baseLevelPercent = 1.0f / levelCount;
    }

    for(int l = 0; l < levelCount; l++)
    {
        gExportMessage = "Loading LVL " + std::string(pLvlInfos[l].lvl_file);
        LoadLevel(pLvlInfos[l].lvl_file);

        int i = -1;
        for (auto& record : gLoadedAnimRecords)
        {
            if (!gExporting) // cancel exporting
                return;

            i++;
            gExportProgress = (baseLevelPercent * l) + (baseLevelPercent * (i / (float)gLoadedAnimRecords.size()));
            
            // if we already have this record, skip it
            if (std::find(dumpList.begin(), dumpList.end(), record.name) != dumpList.end())
            {
                continue;
            }

            gExportMessage = "Exporting " + record.name;
            
            if (DumpAnim("hd/sprites/" + record.name, record.record))
            {
                dumpedAnimsCount++;
            }
            else
            {
                failedDumpList.push_back(record.name);
            }

            dumpList.push_back(record.name);
        }
    }

    gExportMessage = "Exported " + std::to_string(dumpedAnimsCount) + " animations!";
    
    if (failedDumpList.size() > 0)
    {
        gExportMessage += "\nFailed to export the following anims:\n";

        for (auto& anim : failedDumpList)
        {
            gExportMessage += anim + "\n";
        }
    }

    gExporting = false;
    gExportProgress = 1.0f;
}

static void ui_popup_export()
{
    // imgui popup that shows the export progress and lets the user cancel it
    // it also shows the current export message

    if (gExporting)
    {
        ImGui::OpenPopup("Export Progress");
    }

    if (ImGui::BeginPopupModal("Export Progress", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::Text("Exporting...");
        ImGui::ProgressBar(gExportProgress, ImVec2(500.0f, 0.0f));

        ImGui::Text("%s", gExportMessage.c_str());

        if (gExportProgress >= 1.0f)
        {
            if (ImGui::Button("Close"))
            {
                ImGui::CloseCurrentPopup();
            }
        }
        else
        {
            if (ImGui::Button("Cancel"))
            {
                gExporting = false;
                gExportThread.detach();
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }

    // create a 'are you sure?' popup that lets the user stop before the export starts
    if (ImGui::BeginPopupModal("Are you sure?##export_popup", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::Text("Are you sure you want to export the selected animations?");
        ImGui::Text("Exporting all will overwrite any existing files you have!");
        ImGui::Text("This means ones you have edited. Backup first!");

        if (ImGui::Button("Yes, I have backed up my edited files."))
        {
            gExporting = true;
            gExportProgress = 0;
            
            // check if the export thread is running already and if so, detach it
            if (gExportThread.joinable())
            {
                gExportThread.detach();
            }
            
            gExportThread = std::thread(export_thread);
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::Button("No - Get me outta here!"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

static void ui_loop()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Tools"))
        {
            for (auto i = 0; i < gTools.size(); i++)
            {
                const AppTool& tool = gTools[i];

                if (ImGui::MenuItem(tool.name))
                {
                    gActiveToolIndex = i;
                }
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Load Level"))
        {
            if (gHasAoFiles && ImGui::BeginMenu("Oddysee"))
            {
                for (auto& lvl : AoLvls)
                {
                    if (ImGui::MenuItem(lvl.lvl_file))
                    {
                        gIsAo = true;
                        LoadLevel(lvl.lvl_file);
                    }
                }
                ImGui::EndMenu();
            }
            if (gHasAeFiles && ImGui::BeginMenu("Exoddus"))
            {
                for (auto& lvl : AeLvls)
                {
                    if (ImGui::MenuItem(lvl.lvl_file))
                    {
                        gIsAo = false;
                        LoadLevel(lvl.lvl_file);
                    }
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Utilities"))
        {
            if (ImGui::MenuItem("Export All Anims"))
            {
                gShowExportPreDialog = true;
            }

            if (ImGui::MenuItem("Copy old->new name map"))
            {
                std::string newMap = "";
                auto records = GetAnimRecords();
                for(auto oldEntry : frametableMatches)
                {
                    auto frameTableOffset = oldEntry.first;
                    auto frameData = oldEntry.second;
                    // converts file name. ex: "EXPLODE.BND_0" to "EXPLODE.BND"
                    std::string fileName = frameData.file_prefix.substr(0, frameData.file_prefix.find_last_of('_'));

                    for(auto r : records)
                    {
                        AnimDetails& details = (gIsAo ? r.mAOData : r.mAEData);

                        if (details.mBanName != nullptr && details.mBanName == fileName && details.mFrameTableOffset == frameTableOffset)
                        {
                            std::string newName = std::string(magic_enum::enum_name(r.mId));
                            newMap += frameData.file_prefix + "_* -> " + newName + "\n";
                        }
                    }
                }

                // imgui copy to clipboard
                ImGui::SetClipboardText(newMap.c_str());

                ShowMessageBox("Copied!", "Copied map to clipboard!");
            }

            ImGui::EndMenu();
        }

        if (ImGui::MenuItem("About"))
        {
            ShowMessageBox("About", "Asset Tool v0.2 by mlgthatsme (milk man)");
        }

        ImGui::EndMainMenuBar();
    }

    if (gShowMessageBox)
    {
        gShowMessageBox = false;
        ImGui::OpenPopup((gMessageBoxTitle + "##message_box_title").c_str());
    }

    if (ImGui::BeginPopupModal((gMessageBoxTitle + "##message_box_title").c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::Text(gMessageBoxText.c_str());

        if (ImGui::Button("Cool"))
        {
            ImGui::CloseCurrentPopup();
        } 

        ImGui::EndPopup();
    }

    if (gShowExportPreDialog)
    {
        ImGui::OpenPopup("Are you sure?##export_popup");
        gShowExportPreDialog=false;
    }

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    const ImGuiWindowFlags appFlags = kImGuiFullscreenFlags;

    if (gActiveToolIndex >= 0)
    {
        gTools[gActiveToolIndex].update_func(appFlags, gElapsedSeconds);
    }

    ui_popup_export();
}

void main_loop()
{
    // init tools
    gTools.push_back({ "Animation Viewer", AppAnimationViewer });
    gTools.push_back({ "FMV Viewer", AppVideoViewer });
    
    gTools.push_back({ "ImGui Demo Window", [](ImGuiWindowFlags /*flags*/, float) { ImGui::ShowDemoWindow(); } });

    // init all game resource management
    InitResourceManager();

    SDL_Window* window = NULL;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        return;
    }

    window = SDL_CreateWindow(
        "Asset Extractor",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        1280, 720,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create context
    SDL_GLContext mContext = SDL_GL_CreateContext(window);
    glewInit();

    if (mContext == NULL)
    {
        LOG_ERROR("OpenGL context could not be created! SDL Error: " << SDL_GetError());
        return;
    }
    else
    {
        // Initialize GLEW
        glewExperimental = GL_TRUE;
        GLenum glewError = glewInit();
        if (glewError != GLEW_OK)
        {
            LOG_ERROR("Error initializing GLEW! " << glewGetErrorString(glewError));
        }

        // Use Vsync
        if (SDL_GL_SetSwapInterval(1) < 0)
        {
            LOG_ERROR("Warning: Unable to set VSync! SDL Error: " << SDL_GetError());
        }
    }

    ImGui::CreateContext();

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup IMGUI for texture debugging
    ImGui_ImplSDL2_InitForOpenGL(window, mContext);
    ImGui_ImplOpenGL3_Init("#version 150");

    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }

            ImGui_ImplSDL2_ProcessEvent(&e);
        }
        
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        ui_loop();

        SDL_SetWindowTitle(window, ("Asset Tool (" + gLoadedLevelName + ") | " + ((gIsAo) ? "Oddysee" : "Exoddus") + " mode").c_str());

        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
        
        static int lastTime = SDL_GetTicks();
        int currentTime = SDL_GetTicks();
        int deltaTime = currentTime - lastTime;

        gElapsedSeconds = deltaTime / 1000.0f;

        lastTime = currentTime;
    }
    ImGui_ImplSDL2_Shutdown();

    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(window);

    SDL_PauseAudio(1);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);

    SDL_Quit();
    return;
}

s32 main(s32 /*argc*/, char_type** /*argv*/)
{
#if _WIN32
    ::AllocConsole();
    ::freopen("CONOUT$", "w", stdout);
    ::SetConsoleTitleA("Debug Console");
    ::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
    RedirectIoStream(true);
#endif
    main_loop();
    return 0;
}
