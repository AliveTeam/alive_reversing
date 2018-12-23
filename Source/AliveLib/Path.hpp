#pragma once

#include "FunctionFwd.hpp"
#include "Psx.hpp"
#include "BitField.hpp"
#include "FixedPoint.hpp"

struct PathData;

enum TLV_Flags
{
    eBit1_Created = 0x1,
    eBit2_Unknown = 0x2,
    eBit3_End_TLV_List = 0x4,
};

struct Path_TLV
{
    BitField8<TLV_Flags> field_0_flags;
    BYTE field_1_unknown;
    __int16 field_2_length;
    int field_4_type;
    PSX_Point field_8_top_left;
    PSX_Point field_C_bottom_right;
};
ALIVE_ASSERT_SIZEOF(Path_TLV, 0x10);

enum class LevelIds : __int16;

class Path
{
public:
    EXPORT void ctor_4DB170();
    EXPORT void dtor_4DB1A0();
    EXPORT void Free_4DB1C0();
    EXPORT void Init_4DB200(const PathData* pPathData, LevelIds level, __int16 path, __int16 cameraId, BYTE** ppPathRes);


    EXPORT void Loader_4DB800(__int16 xpos, __int16 ypos, __int16 loadMode, __int16 typeToLoad);

    EXPORT Path_TLV* Get_First_TLV_For_Offsetted_Camera_4DB610(__int16 cam_x_idx, __int16 cam_y_idx);
    EXPORT static Path_TLV* __stdcall Next_TLV_4DB6A0(Path_TLV* pTlv);
    EXPORT Path_TLV* TLV_First_Of_Type_In_Camera_4DB6D0(unsigned __int16 objectType, __int16 camX);
    EXPORT Path_TLV* TLV_Get_At_4DB4B0(__int16 xpos, __int16 ypos, __int16 width, __int16 height, unsigned __int16 objectType);
    EXPORT Path_TLV* TLV_Get_At_4DB290(Path_TLV* pTlv, FP xpos, FP ypos, FP w, FP h);
    EXPORT Path_TLV* TLV_From_Offset_Lvl_Cam_4DB770(unsigned int tlvOffset_levelId_PathId);
    EXPORT static Path_TLV* __stdcall TLV_Next_Of_Type_4DB720(Path_TLV* pTlv, unsigned __int16 type);
    EXPORT static void __stdcall TLV_Reset_4DB8E0(unsigned int tlvOffset_levelId_PathId, __int16 hiFlags, char bSetCreated, char bBit2);
    EXPORT static void CC Start_Sounds_For_Objects_In_Camera_4CBAF0(__int16 type, __int16 cam_x_idx, __int16 cam_y_idx);

    LevelIds field_0_levelId;
    unsigned __int16 field_2_pathId;
    __int16 field_4_cameraId;
    unsigned __int16 field_6_cams_on_x;
    unsigned __int16 field_8_cams_on_y;
    __int16 field_A;
    const PathData* field_C_pPathData;
    BYTE** field_10_ppRes;
};
ALIVE_ASSERT_SIZEOF(Path, 0x14);

struct Path_Door : public Path_TLV
{
    const static unsigned short kType = 5;

    __int16 field_10_level;
    __int16 field_12_path;
    __int16 field_14_camera;
    __int16 field_16_scale;
    __int16 field_18_door_number;
    __int16 field_1A_id;
    __int16 field_1C_target_door_number;
    __int16 field_1E_type;
    __int16 field_20_start_state;
    __int16 field_22_hubs[8];
    __int16 field_32_wipe_effect;
    __int16 field_34_movie_number;
    __int16 field_36_x_offset;
    __int16 field_38_y_offset;
    __int16 field_3A_wipe_x_org;
    __int16 field_3C_wipe_y_org;
    __int16 field_3E_abe_direction;
    __int16 field_40_close_after_use;
    __int16 field_42_cancel_throwables;
};
ALIVE_ASSERT_SIZEOF(Path_Door, 0x44);

struct Path_Teleporter_Data
{
    __int16 field_10_id;
    __int16 field_12_target_id;
    __int16 field_14_camera;
    __int16 field_16_path;
    __int16 field_18_level;
    __int16 field_1A_trigger_id;
    __int16 field_1C_scale;
    __int16 field_1E_wipe;
    __int16 field_20_movie_number;
    __int16 field_22_eletric_x;
    __int16 field_24_electric_y;
};
ALIVE_ASSERT_SIZEOF(Path_Teleporter_Data, 0x16);

struct Path_Teleporter : public Path_TLV
{
    static constexpr unsigned short kType = 88;
    Path_Teleporter_Data field_10_data;
    __int16 field_26_pad; // Actually padding here as the game won't copy these 2 bytes, but its included in the TLV length
};
ALIVE_ASSERT_SIZEOF(Path_Teleporter, 0x28); // 0x10 for base

struct Path_Hoist : public Path_TLV
{
    static constexpr unsigned short kType = 2;

    enum class Type : __int16
    {
        eNextFloor = 0,
        eNextEdge = 1,
        eOffScreen = 2,
    };
    Type field_10_type;

    enum class EdgeType : __int16
    {
        eLeft = 0,
        eRight = 1,
        eBoth = 2,
    };
    EdgeType field_12_edge_type;

    __int16 field_14_id;

    enum class Scale : __int16
    {
        eFull = 0,
        eHalf = 1,
    };
    Scale field_16_scale;
};

struct Path_Edge : public Path_TLV
{
    static constexpr unsigned short kType = 3;

    enum class Type : __int16
    {
        eLeft = 0,
        eRight = 1,
        eBoth = 2,
    };
    Type field_10_type;
    __int16 field_12_can_grab;

    enum class Scale : int
    {
        eFull = 0,
        eHalf = 1,
    };
    Scale field_14_scale;
};

struct Path_SoftLanding : public Path_TLV
{
    static constexpr unsigned short kType = 75;
    int field_10_id;
};

struct Path_Well_Base : public Path_TLV
{
    __int16 field_0_scale;
    __int16 field_2_trigger_id;
    __int16 field_4_well_id;
    __int16 field_6_res_id;
};

struct Path_Well_Local : public Path_Well_Base
{
    static constexpr unsigned short kType = 8;

    __int16 field_18_off_dx;
    __int16 field_1A_off_dy;
    __int16 field_1C_on_dx;
    __int16 field_1E_on_dy;
    __int16 field_20_emit_leaves;
    __int16 field_22_leaf_x;
    __int16 field_24_leaf_y;
    __int16 field_26;
};

struct Path_Well_Express : public Path_Well_Base
{
    static constexpr unsigned short kType = 23;

    __int16 field_18_exit_x;
    __int16 field_1A_exit_y;
    __int16 field_1C_off_level;
    __int16 field_1E_off_path;
    __int16 field_20_off_camera;
    __int16 field_22_on_level;
    __int16 field_24_on_path;
    __int16 field_26_on_camera;
    __int16 field_28_target_well;
    __int16 field_2A_emit_leaves;
    __int16 field_2C_leaf_x;
    __int16 field_2E_leaf_y;
    __int16 field_30_movie_id;
    __int16 pad;
};

struct Path_Alarm : public Path_TLV
{
    short field_10_id;
    WORD field_12_duration;
};
ALIVE_ASSERT_SIZEOF(Path_Alarm, 0x14);

ALIVE_VAR_EXTERN(Path*, sPath_dword_BB47C0);
