#include "stdafx.h"
#include "PathData.hpp"
#include "Function.hpp"
#include <sstream>
#include <assert.h>

class Collisions
{
public:
    EXPORT static void __cdecl sub_4188A0(CollisionInfo *pCollisionInfo, void *pPathRes);
};

void __cdecl Collisions::sub_4188A0(CollisionInfo *pCollisionInfo, void *pPathRes)
{
    NOT_IMPLEMENTED();
}


const PathData kNullPathEntry = {};

// TODO: Populate
void* kObjectFactory[256] = {};

const PathData ST_PathData[] =
{
    kNullPathEntry, 
    { 0, 0, 2625, 2340, 375, 260, 375, 260, 504, 660, 375, 260, kObjectFactory }, 
};

const PathData MI_PathData[] =
{
    kNullPathEntry, 
    { 0, 0, 5625, 2340, 375, 260, 375, 260, 2980, 11796, 1875, 0, kObjectFactory }, 
    { 0, 0, 4125, 2600, 375, 260, 375, 260, 3860, 8772, 0, 0, kObjectFactory }, 
    { 0, 0, 4500, 1820, 375, 260, 375, 260, 1752, 3652, 375, 1300, kObjectFactory }, 
    { 0, 0, 4500, 3640, 375, 260, 375, 260, 4404, 15424, 0, 260, kObjectFactory }, 
    { 0, 0, 4500, 1040, 375, 260, 375, 260, 1384, 5584, 0, 0, kObjectFactory }, 
    { 0, 0, 4875, 1820, 375, 260, 375, 260, 2048, 5212, 375, 1300, kObjectFactory }, 
    { 0, 0, 2250, 1300, 375, 260, 375, 260, 1320, 3320, 1125, 1300, kObjectFactory }, 
    { 0, 0, 3375, 1300, 375, 260, 375, 260, 1280, 6580, 3000, 520, kObjectFactory }, 
    { 0, 0, 2625, 1560, 375, 260, 375, 260, 1536, 3760, 1500, 520, kObjectFactory }, 
    { 0, 0, 3375, 1040, 375, 260, 375, 260, 1588, 4792, 2250, 1040, kObjectFactory }, 
    { 0, 0, 1500, 1300, 375, 260, 375, 260, 460, 2160, 2625, 2600, kObjectFactory }, 
    { 0, 0, 3750, 780, 375, 260, 375, 260, 1160, 4816, 375, 0, kObjectFactory }, 
};

const PathData NE_PathData[] =
{
    kNullPathEntry, 
    { 0, 0, 1875, 1300, 375, 260, 375, 260, 740, 2268, 0, 0, kObjectFactory }, 
    { 0, 0, 2250, 2080, 375, 260, 375, 260, 2404, 5496, 1875, 520, kObjectFactory }, 
    { 0, 0, 4500, 2600, 375, 260, 375, 260, 1700, 4628, 0, 0, kObjectFactory }, 
    { 0, 0, 1875, 2080, 375, 260, 375, 260, 1020, 3016, 0, 260, kObjectFactory }, 
    { 0, 0, 3375, 1820, 375, 260, 375, 260, 1444, 3852, 0, 0, kObjectFactory }, 
    { 0, 0, 3000, 1040, 375, 260, 375, 260, 676, 3448, 375, 260, kObjectFactory }, 
    { 0, 0, 2625, 780, 375, 260, 375, 260, 588, 3356, 750, 520, kObjectFactory }, 
};

const PathData PV_PathData[] =
{
    kNullPathEntry, 
    { 0, 0, 5250, 1560, 375, 260, 375, 260, 3192, 6548, 0, 520, kObjectFactory }, 
    { 0, 0, 1500, 260, 375, 260, 375, 260, 212, 772, 3375, 1820, kObjectFactory }, 
    { 0, 0, 3000, 1560, 375, 260, 375, 260, 1384, 3492, 0, 0, kObjectFactory }, 
    { 0, 0, 1875, 1040, 375, 260, 375, 260, 1540, 3488, 0, 0, kObjectFactory }, 
    { 0, 0, 2625, 780, 375, 260, 375, 260, 1568, 3376, 0, 520, kObjectFactory }, 
    { 0, 0, 1875, 520, 375, 260, 375, 260, 560, 2200, 1125, 520, kObjectFactory }, 
    { 0, 0, 2250, 1560, 375, 260, 375, 260, 1108, 3568, 750, 260, kObjectFactory }, 
    { 0, 0, 1500, 780, 375, 260, 375, 260, 736, 2032, 0, 0, kObjectFactory }, 
    { 0, 0, 2250, 1040, 375, 260, 375, 260, 932, 3256, 0, 260, kObjectFactory }, 
    { 0, 0, 3375, 1300, 375, 260, 375, 260, 1580, 3408, 375, 1040, kObjectFactory }, 
    { 0, 0, 2250, 520, 375, 260, 375, 260, 576, 2696, 0, 260, kObjectFactory }, 
    { 0, 0, 1125, 1560, 375, 260, 375, 260, 1104, 3444, 750, 0, kObjectFactory }, 
    { 0, 0, 4500, 2600, 375, 260, 375, 260, 3060, 6696, 0, 0, kObjectFactory }, 
    { 0, 0, 1125, 1040, 375, 260, 375, 260, 656, 3084, 0, 0, kObjectFactory }, 
    { 0, 0, 1500, 1040, 375, 260, 375, 260, 648, 2128, 1125, 520, kObjectFactory }, 
};

const PathData SV_PathData[] =
{
    kNullPathEntry, 
    { 0, 0, 3000, 520, 375, 260, 375, 260, 768, 3076, 375, 0, kObjectFactory }, 
    { 0, 0, 1500, 1560, 375, 260, 375, 260, 1872, 5068, 375, 0, kObjectFactory }, 
    { 0, 0, 1875, 780, 375, 260, 375, 260, 960, 3544, 375, 260, kObjectFactory }, 
    { 0, 0, 2250, 520, 375, 260, 375, 260, 776, 3052, 750, 0, kObjectFactory }, 
    { 0, 0, 1500, 1300, 375, 260, 375, 260, 1720, 5440, 375, 260, kObjectFactory }, 
    { 0, 0, 4500, 1820, 375, 260, 375, 260, 2532, 8392, 0, 260, kObjectFactory }, 
    { 0, 0, 2250, 1300, 375, 260, 375, 260, 1200, 4808, 0, 0, kObjectFactory }, 
    { 0, 0, 1500, 1300, 375, 260, 375, 260, 1420, 4316, 375, 260, kObjectFactory }, 
    { 0, 0, 2250, 1300, 375, 260, 375, 260, 1300, 3172, 375, 260, kObjectFactory }, 
    { 0, 0, 1875, 780, 375, 260, 375, 260, 800, 3584, 375, 260, kObjectFactory }, 
    { 0, 0, 750, 260, 375, 260, 375, 260, 156, 1024, 375, 260, kObjectFactory }, 
    { 0, 0, 750, 520, 375, 260, 375, 260, 332, 1324, 750, 0, kObjectFactory }, 
    { 0, 0, 1875, 520, 375, 260, 375, 260, 520, 1700, 2250, 520, kObjectFactory }, 
    { 0, 0, 2250, 780, 375, 260, 375, 260, 1204, 3024, 375, 520, kObjectFactory }, 
};

const PathData FD_PathData[] =
{
    kNullPathEntry, 
    { 0, 0, 3000, 1560, 375, 260, 375, 260, 1124, 3992, 3375, 520, kObjectFactory }, 
    { 0, 0, 3000, 2080, 375, 260, 375, 260, 2372, 7184, 2625, 1560, kObjectFactory }, 
    { 0, 0, 3375, 2600, 375, 260, 375, 260, 2920, 7080, 1875, 520, kObjectFactory }, 
    { 0, 0, 3750, 1820, 375, 260, 375, 260, 3200, 8196, 2625, 1300, kObjectFactory }, 
    { 0, 0, 3000, 1820, 375, 260, 375, 260, 2308, 5968, 2250, 1300, kObjectFactory }, 
    { 0, 0, 1500, 520, 375, 260, 375, 260, 284, 1348, 4500, 1560, kObjectFactory }, 
    { 0, 0, 1875, 1040, 375, 260, 375, 260, 1580, 3680, 2625, 1820, kObjectFactory }, 
    { 0, 0, 1875, 1040, 375, 260, 375, 260, 960, 2940, 3000, 1560, kObjectFactory }, 
    { 0, 0, 2250, 1300, 375, 260, 375, 260, 1240, 3260, 3000, 2080, kObjectFactory }, 
    { 0, 0, 1875, 1040, 375, 260, 375, 260, 740, 2096, 3000, 2080, kObjectFactory }, 
    { 0, 0, 2625, 1560, 375, 260, 375, 260, 2436, 7148, 375, 260, kObjectFactory }, 
    { 0, 0, 1875, 780, 375, 260, 375, 260, 1540, 3684, 2625, 2080, kObjectFactory }, 
    { 0, 0, 1500, 260, 375, 260, 375, 260, 1732, 4316, 750, 520, kObjectFactory }, 
    { 0, 0, 1500, 780, 375, 260, 375, 260, 396, 1256, 375, 1300, kObjectFactory }, 
};

const PathData BA_PathData[] =
{
    kNullPathEntry, 
    { 0, 0, 2250, 2080, 375, 260, 375, 260, 2444, 7612, 375, 0, kObjectFactory }, 
    { 0, 0, 1125, 1040, 375, 260, 375, 260, 336, 1536, 0, 0, kObjectFactory }, 
    { 0, 0, 1500, 1300, 375, 260, 375, 260, 1820, 4464, 750, 520, kObjectFactory }, 
    { 0, 0, 1125, 780, 375, 260, 375, 260, 1772, 5320, 375, 520, kObjectFactory }, 
    { 0, 0, 4875, 2600, 375, 260, 375, 260, 5240, 12220, 375, 0, kObjectFactory }, 
    { 0, 0, 4875, 2600, 375, 260, 375, 260, 5240, 12220, 375, 0, kObjectFactory }, 
    { 0, 0, 2250, 1040, 375, 260, 375, 260, 2312, 8224, 0, 260, kObjectFactory }, 
    { 0, 0, 2250, 2080, 375, 260, 375, 260, 2444, 7612, 375, 0, kObjectFactory }, 
    { 0, 0, 2250, 780, 375, 260, 375, 260, 964, 3008, 375, 260, kObjectFactory }, 
    { 0, 0, 2625, 780, 375, 260, 375, 260, 3848, 8624, 375, 260, kObjectFactory }, 
    { 0, 0, 2250, 1820, 375, 260, 375, 260, 1136, 5136, 0, 0, kObjectFactory }, 
    { 0, 0, 2250, 1820, 375, 260, 375, 260, 1136, 5136, 0, 0, kObjectFactory }, 
    { 0, 0, 750, 780, 375, 260, 375, 260, 288, 1644, 375, 260, kObjectFactory }, 
    { 0, 0, 2250, 1820, 375, 260, 375, 260, 4016, 7480, 0, 0, kObjectFactory }, 
    { 0, 0, 750, 780, 375, 260, 375, 260, 368, 2964, 375, 260, kObjectFactory }, 
    { 0, 0, 750, 260, 375, 260, 375, 260, 156, 936, 375, 260, kObjectFactory }, 
};

const PathData BW_PathData[] =
{
    kNullPathEntry, 
    { 0, 0, 3750, 1820, 375, 260, 375, 260, 1820, 4412, 0, 0, kObjectFactory }, 
    { 0, 0, 3750, 2080, 375, 260, 375, 260, 3160, 9076, 0, 0, kObjectFactory }, 
    { 0, 0, 3750, 1300, 375, 260, 375, 260, 2820, 7724, 0, 260, kObjectFactory }, 
    { 0, 0, 3375, 2080, 375, 260, 375, 260, 2696, 9732, 375, 0, kObjectFactory }, 
    { 0, 0, 3375, 2080, 375, 260, 375, 260, 2696, 9732, 375, 0, kObjectFactory }, 
    { 0, 0, 3375, 2080, 375, 260, 375, 260, 2696, 9732, 375, 0, kObjectFactory }, 
    { 0, 0, 3375, 1560, 375, 260, 375, 260, 1512, 6184, 375, 0, kObjectFactory }, 
    { 0, 0, 3375, 1560, 375, 260, 375, 260, 1512, 6184, 375, 0, kObjectFactory }, 
    { 0, 0, 2625, 1040, 375, 260, 375, 260, 1364, 4980, 375, 780, kObjectFactory }, 
    { 0, 0, 3375, 1560, 375, 260, 375, 260, 1512, 6184, 375, 0, kObjectFactory }, 
    { 0, 0, 1875, 520, 375, 260, 375, 260, 300, 1396, 1875, 1300, kObjectFactory }, 
    { 0, 0, 1500, 520, 375, 260, 375, 260, 244, 1104, 1125, 520, kObjectFactory }, 
    { 0, 0, 1500, 520, 375, 260, 375, 260, 604, 2348, 375, 1040, kObjectFactory }, 
    { 0, 0, 3000, 780, 375, 260, 375, 260, 832, 2180, 750, 520, kObjectFactory }, 
};

const PathData BR_PathData[] =
{
    kNullPathEntry, 
    { 0, 0, 3000, 1040, 375, 260, 375, 260, 1836, 6272, 0, 0, kObjectFactory }, 
    { 0, 0, 4500, 1820, 375, 260, 375, 260, 2512, 5592, 375, 780, kObjectFactory }, 
    { 0, 0, 3375, 1040, 375, 260, 375, 260, 1768, 4284, 2625, 780, kObjectFactory }, 
    { 0, 0, 3750, 1820, 375, 260, 375, 260, 1380, 5704, 1125, 780, kObjectFactory }, 
    { 0, 0, 3375, 1300, 375, 260, 375, 260, 1200, 4272, 2625, 780, kObjectFactory }, 
    { 0, 0, 2625, 1820, 375, 260, 375, 260, 2552, 5360, 750, 520, kObjectFactory }, 
    { 0, 0, 2250, 780, 375, 260, 375, 260, 1344, 4328, 375, 0, kObjectFactory }, 
    { 0, 0, 2625, 520, 375, 260, 375, 260, 852, 2340, 1875, 1820, kObjectFactory }, 
    { 0, 0, 3000, 1820, 375, 260, 375, 260, 3128, 6272, 375, 1040, kObjectFactory }, 
    { 0, 0, 3750, 1820, 375, 260, 375, 260, 1860, 7256, 0, 0, kObjectFactory }, 
    { 0, 0, 3375, 3120, 375, 260, 375, 260, 2364, 5328, 375, 0, kObjectFactory }, 
    { 0, 0, 2625, 780, 375, 260, 375, 260, 768, 3344, 0, 0, kObjectFactory }, 
    { 0, 0, 3000, 260, 375, 260, 375, 260, 684, 2568, 3000, 1560, kObjectFactory }, 
    { 0, 0, 4125, 780, 375, 260, 375, 260, 1224, 3928, 375, 0, kObjectFactory }, 
    { 0, 0, 2625, 780, 375, 260, 375, 260, 748, 3052, 1125, 520, kObjectFactory }, 
    { 0, 0, 2250, 1300, 375, 260, 375, 260, 760, 2492, 1125, 780, kObjectFactory }, 
    { 0, 0, 2250, 2600, 375, 260, 375, 260, 2820, 10656, 0, 0, kObjectFactory }, 
    { 0, 0, 2625, 1040, 375, 260, 375, 260, 824, 3776, 0, 780, kObjectFactory }, 
    { 0, 0, 2250, 1040, 375, 260, 375, 260, 1712, 5260, 0, 520, kObjectFactory }, 
    { 0, 0, 3000, 1560, 375, 260, 375, 260, 884, 4436, 0, 260, kObjectFactory }, 
    { 0, 0, 1500, 520, 375, 260, 375, 260, 304, 1284, 0, 0, kObjectFactory }, 
    { 0, 0, 1500, 520, 375, 260, 375, 260, 304, 1284, 0, 0, kObjectFactory }, 
    { 0, 0, 1125, 1040, 375, 260, 375, 260, 196, 1364, 0, 0, kObjectFactory }, 
    { 0, 0, 1125, 1040, 375, 260, 375, 260, 476, 1756, 0, 0, kObjectFactory }, 
    { 0, 0, 1125, 1040, 375, 260, 375, 260, 576, 1616, 0, 0, kObjectFactory }, 
    { 0, 0, 2250, 1040, 375, 260, 375, 260, 712, 3928, 1125, 0, kObjectFactory }, 
    { 0, 0, 3000, 2340, 375, 260, 375, 260, 1076, 2644, 750, 780, kObjectFactory }, 
    { 0, 0, 1500, 520, 375, 260, 375, 260, 384, 1964, 375, 780, kObjectFactory }, 
    { 0, 0, 3000, 780, 375, 260, 375, 260, 552, 3284, 0, 1040, kObjectFactory }, 
};

const PathData BM_PathData[] =
{
    kNullPathEntry, 
    { 0, 0, 2625, 2600, 375, 260, 375, 260, 2560, 8488, 375, 0, kObjectFactory }, 
};

const PathData TL_PathData[] =
{
    kNullPathEntry, 
    { 0, 0, 1125, 260, 375, 260, 375, 260, 424, 740, 375, 260, kObjectFactory }, 
    { 0, 0, 1875, 1820, 375, 260, 375, 260, 860, 1672, 375, 0, kObjectFactory }, 
};

const PathData CR_PathData[] =
{
    kNullPathEntry, 
    { 0, 0, 3750, 1820, 375, 260, 375, 260, 560, 592, 375, 520, kObjectFactory }, 
    { 0, 0, 3750, 1300, 375, 260, 375, 260, 400, 416, 375, 260, kObjectFactory }, 
};

// ===========================================================================

const CollisionInfo kNullCollisionInfo = {};

const CollisionInfo ST_CollisionInfo[] =
{
    kNullCollisionInfo, 
    { Collisions::sub_4188A0, 0, 0, 2625, 2340, 504, 0, 375, 260}
};

const CollisionInfo MI_CollisionInfo[] =
{
    kNullCollisionInfo, 
    { Collisions::sub_4188A0, 0, 0, 5625, 2340, 1080, 95, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 4125, 2600, 880, 149, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 4500, 1820, 672, 54, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 4500, 3640, 1344, 153, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 4500, 1040, 384, 50, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 4875, 1820, 728, 66, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2250, 1300, 240, 54, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3375, 1300, 360, 46, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2625, 1560, 336, 60, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3375, 1040, 288, 65, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1500, 1300, 160, 15, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3750, 780, 240, 46, 375, 260}
};

const CollisionInfo NE_CollisionInfo[] =
{
    kNullCollisionInfo, 
    { Collisions::sub_4188A0, 0, 0, 1875, 1300, 200, 27, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2250, 2080, 384, 101, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 4500, 2600, 960, 37, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1875, 2080, 320, 35, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3375, 1820, 504, 47, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3000, 1040, 256, 21, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2625, 780, 168, 21, 375, 260}
};

const CollisionInfo PV_CollisionInfo[] =
{
    kNullCollisionInfo, 
    { Collisions::sub_4188A0, 0, 0, 5250, 1560, 672, 126, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1500, 260, 32, 9, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3000, 1560, 384, 50, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1875, 1040, 160, 69, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2625, 780, 168, 70, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1875, 520, 80, 24, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2250, 1560, 288, 41, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1500, 780, 96, 32, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2250, 1040, 192, 37, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3375, 1300, 360, 61, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2250, 520, 96, 24, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1125, 1560, 144, 48, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 4500, 2600, 960, 105, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1125, 1040, 96, 28, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1500, 1040, 128, 26, 375, 260}
};

const CollisionInfo SV_CollisionInfo[] =
{
    kNullCollisionInfo, 
    { Collisions::sub_4188A0, 0, 0, 3000, 520, 128, 32, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1500, 1560, 192, 84, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1875, 780, 120, 42, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2250, 520, 96, 34, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1500, 1300, 160, 78, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 4500, 1820, 672, 93, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2250, 1300, 240, 48, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1500, 1300, 160, 63, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2250, 1300, 240, 53, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1875, 780, 120, 34, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 750, 260, 16, 7, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 750, 520, 32, 15, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1875, 520, 80, 22, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2250, 780, 144, 53, 375, 260}
};

const CollisionInfo FD_CollisionInfo[] =
{
    kNullCollisionInfo, 
    { Collisions::sub_4188A0, 0, 0, 3000, 1560, 384, 37, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3000, 2080, 512, 93, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3375, 2600, 720, 110, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3750, 1820, 560, 132, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3000, 1820, 448, 93, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1500, 520, 64, 11, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1875, 1040, 160, 71, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1875, 1040, 160, 40, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2250, 1300, 240, 50, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1875, 1040, 160, 29, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2625, 1560, 336, 105, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1875, 780, 120, 71, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1500, 260, 32, 85, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1500, 780, 96, 15, 375, 260}
};

const CollisionInfo BA_CollisionInfo[] =
{
    kNullCollisionInfo, 
    { Collisions::sub_4188A0, 0, 0, 2250, 2080, 384, 103, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1125, 1040, 96, 12, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1500, 1300, 160, 83, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1125, 780, 72, 85, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 4875, 2600, 1040, 210, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 4875, 2600, 1040, 210, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2250, 1040, 192, 106, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2250, 2080, 384, 103, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2250, 780, 144, 41, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2625, 780, 168, 184, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2250, 1820, 336, 40, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2250, 1820, 336, 40, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 750, 780, 48, 12, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2250, 1820, 336, 184, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 750, 780, 48, 16, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 750, 260, 16, 7, 375, 260}
};

const CollisionInfo BW_CollisionInfo[] =
{
    kNullCollisionInfo, 
    { Collisions::sub_4188A0, 0, 0, 3750, 1820, 560, 63, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3750, 2080, 640, 126, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3750, 1300, 400, 121, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3375, 2080, 576, 106, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3375, 2080, 576, 106, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3375, 2080, 576, 106, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3375, 1560, 432, 54, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3375, 1560, 432, 54, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2625, 1040, 224, 57, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3375, 1560, 432, 54, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1875, 520, 80, 11, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1500, 520, 64, 9, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1500, 520, 64, 27, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3000, 780, 192, 32, 375, 260}
};

const CollisionInfo BR_CollisionInfo[] =
{
    kNullCollisionInfo, 
    { Collisions::sub_4188A0, 0, 0, 3000, 1040, 256, 79, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 4500, 1820, 672, 92, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3375, 1040, 288, 74, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3750, 1820, 560, 41, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3375, 1300, 360, 42, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2625, 1820, 392, 108, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2250, 780, 144, 60, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2625, 520, 112, 37, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3000, 1820, 448, 134, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3750, 1820, 560, 65, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3375, 3120, 864, 75, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2625, 780, 168, 30, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3000, 260, 64, 31, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 4125, 780, 264, 48, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2625, 780, 168, 29, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2250, 1300, 240, 26, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2250, 2600, 480, 117, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2625, 1040, 224, 30, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2250, 1040, 192, 76, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3000, 1560, 384, 25, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1500, 520, 64, 12, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1500, 520, 64, 12, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1125, 1040, 96, 5, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1125, 1040, 96, 19, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1125, 1040, 96, 24, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 2250, 1040, 192, 26, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3000, 2340, 576, 25, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1500, 520, 64, 16, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3000, 780, 192, 18, 375, 260}
};

const CollisionInfo BM_CollisionInfo[] =
{
    kNullCollisionInfo, 
    { Collisions::sub_4188A0, 0, 0, 2625, 2600, 560, 100, 375, 260}
};

const CollisionInfo TL_CollisionInfo[] =
{
    kNullCollisionInfo, 
    { Collisions::sub_4188A0, 0, 0, 1125, 260, 24, 20, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 1875, 1820, 280, 29, 375, 260}
};

const CollisionInfo CR_CollisionInfo[] =
{
    kNullCollisionInfo, 
    { Collisions::sub_4188A0, 0, 0, 3750, 1820, 560, 0, 375, 260}, 
    { Collisions::sub_4188A0, 0, 0, 3750, 1300, 400, 0, 375, 260}
};

// ========================================================================

const PathBlyRec kNullPathBlyRec = {};

const PathBlyRec ST_PathBlyRecInfo[] =
{
    kNullPathBlyRec,
    { "STP1.BLY", ST_PathData, ST_CollisionInfo, 256, 0 }
};

const PathBlyRec MI_PathBlyRecInfo[] =
{
    kNullPathBlyRec,
    { "MIP1.BLY",  MI_PathData, MI_CollisionInfo, 1796, 2312 },
    { "MIP2.BLY",  MI_PathData, MI_CollisionInfo, 2565, 3083 },
    { "MIP3.BLY",  MI_PathData, MI_CollisionInfo, 3334, 3854 },
    { "MIP4.BLY",  MI_PathData, MI_CollisionInfo, 1796, 2312 },
    { "MIP5.BLY",  MI_PathData, MI_CollisionInfo, 4882, 5396 },
    { "MIP6.BLY",  MI_PathData, MI_CollisionInfo, 5910, 6424 },
    { "MIP7.BLY",  MI_PathData, MI_CollisionInfo, 5910, 6424 },
    { "MIP8.BLY",  MI_PathData, MI_CollisionInfo, 1796, 2312 },
    { "MIP9.BLY",  MI_PathData, MI_CollisionInfo, 2565, 3083 },
    { "MIP10.BLY", MI_PathData, MI_CollisionInfo, 6938, 7452 },
    { "MIP11.BLY", MI_PathData, MI_CollisionInfo, 1796, 2312 },
    { "MIP12.BLY", MI_PathData, MI_CollisionInfo, 4882, 5396 }
};

const PathBlyRec NE_PathBlyRecInfo[] =
{
    kNullPathBlyRec,
    { "NEP1.BLY", NE_PathData, NE_CollisionInfo, 7966, 8480 },
    { "NEP2.BLY", NE_PathData, NE_CollisionInfo, 9508, 10022 },
    { "NEP3.BLY", NE_PathData, NE_CollisionInfo, 7966, 8480 },
    { "NEP4.BLY", NE_PathData, NE_CollisionInfo, 7966, 8480 },
    { "NEP5.BLY", NE_PathData, NE_CollisionInfo, 9508, 10022 },
    { "NEP6.BLY", NE_PathData, NE_CollisionInfo, 7966, 8480 },
    { "NEP7.BLY", NE_PathData, NE_CollisionInfo, 7966, 8480 }
};

const PathBlyRec PV_PathBlyRecInfo[] =
{
    kNullPathBlyRec,
    { "PVP1.BLY",  PV_PathData, PV_CollisionInfo, 10536, 11050 },
    { "PVP2.BLY",  PV_PathData, PV_CollisionInfo, 10536, 11050 },
    { "PVP3.BLY",  PV_PathData, PV_CollisionInfo, 10536, 11050 },
    { "PVP4.BLY",  PV_PathData, PV_CollisionInfo, 10536, 11050 },
    { "PVP5.BLY",  PV_PathData, PV_CollisionInfo, 10536, 11050 },
    { "PVP6.BLY",  PV_PathData, PV_CollisionInfo, 10536, 11050 },
    { "PVP7.BLY",  PV_PathData, PV_CollisionInfo, 10536, 11050 },
    { "PVP8.BLY",  PV_PathData, PV_CollisionInfo, 10536, 11050 },
    { "PVP9.BLY",  PV_PathData, PV_CollisionInfo, 10536, 11050 },
    { "PVP10.BLY", PV_PathData, PV_CollisionInfo, 10536, 11050 },
    { "PVP11.BLY", PV_PathData, PV_CollisionInfo, 10536, 11050 },
    { "PVP12.BLY", PV_PathData, PV_CollisionInfo, 10536, 11050 },
    { "PVP13.BLY", PV_PathData, PV_CollisionInfo, 10536, 11050 },
    { "PVP14.BLY", PV_PathData, PV_CollisionInfo, 12078, 12592 },
    { "PVP15.BLY", PV_PathData, PV_CollisionInfo, 13106, 13620 }
};

const PathBlyRec SV_PathBlyRecInfo[] =
{
    kNullPathBlyRec,
    { "SVP1.BLY",  SV_PathData, SV_CollisionInfo, 14134, 14648 },
    { "SVP2.BLY",  SV_PathData, SV_CollisionInfo, 14134, 14648 },
    { "SVP3.BLY",  SV_PathData, SV_CollisionInfo, 14134, 14648 },
    { "SVP4.BLY",  SV_PathData, SV_CollisionInfo, 14134, 14648 },
    { "SVP5.BLY",  SV_PathData, SV_CollisionInfo, 14134, 14648 },
    { "SVP6.BLY",  SV_PathData, SV_CollisionInfo, 14134, 14648 },
    { "SVP7.BLY",  SV_PathData, SV_CollisionInfo, 15676, 16190 },
    { "SVP8.BLY",  SV_PathData, SV_CollisionInfo, 14134, 14648 },
    { "SVP9.BLY",  SV_PathData, SV_CollisionInfo, 14134, 14648 },
    { "SVP10.BLY", SV_PathData, SV_CollisionInfo, 16704, 17218 },
    { "SVP11.BLY", SV_PathData, SV_CollisionInfo, 14134, 14648 },
    { "SVP12.BLY", SV_PathData, SV_CollisionInfo, 14134, 14648 },
    { "SVP13.BLY", SV_PathData, SV_CollisionInfo, 14134, 14648 },
    { "SVP14.BLY", SV_PathData, SV_CollisionInfo, 14134, 14648 }
};

const PathBlyRec BW_PathBlyRecInfo[] =
{
    kNullPathBlyRec,
    { "BWP1.BLY",  BW_PathData, BW_CollisionInfo, 30068, 30582 },
    { "BWP2.BLY",  BW_PathData, BW_CollisionInfo, 31610, 32124 },
    { "BWP3.BLY",  BW_PathData, BW_CollisionInfo, 33666, 34180 },
    { "BWP4.BLY",  BW_PathData, BW_CollisionInfo, 32638, 33152 },
    { nullptr,     BW_PathData, BW_CollisionInfo, 30068, 30582 },
    { nullptr,     BW_PathData, BW_CollisionInfo, 30068, 30582 },
    { "BWP7.BLY",  BW_PathData, BW_CollisionInfo, 34694, 35208 },
    { nullptr,     BW_PathData, BW_CollisionInfo, 30068, 30582 },
    { "BWP9.BLY",  BW_PathData, BW_CollisionInfo, 35722, 36236 },
    { nullptr,     BW_PathData, BW_CollisionInfo, 30068, 30582 },
    { "BWP11.BLY", BW_PathData, BW_CollisionInfo, 32638, 33152 },
    { "BWP12.BLY", BW_PathData, BW_CollisionInfo, 32638, 33152 },
    { "BWP13.BLY", BW_PathData, BW_CollisionInfo, 35722, 36236 },
    { "BWP14.BLY", BW_PathData, BW_CollisionInfo, 30068, 30582 }
};

const PathBlyRec BR_PathBlyRecInfo[] =
{
    kNullPathBlyRec,
    { "BRP1.BLY",  BR_PathData, BR_CollisionInfo, 36750, 37264 },
    { "BRP2.BLY",  BR_PathData, BR_CollisionInfo, 38292, 38806 },
    { "BRP3.BLY",  BR_PathData, BR_CollisionInfo, 39320, 39834 },
    { "BRP4.BLY",  BR_PathData, BR_CollisionInfo, 40348, 40862 },
    { "BRP5.BLY",  BR_PathData, BR_CollisionInfo, 41376, 41890 },
    { "BRP6.BLY",  BR_PathData, BR_CollisionInfo, 39320, 39834 },
    { "BRP7.BLY",  BR_PathData, BR_CollisionInfo, 36750, 37264 },
    { "BRP8.BLY",  BR_PathData, BR_CollisionInfo, 38292, 38806 },
    { "BRP9.BLY",  BR_PathData, BR_CollisionInfo, 42404, 42918 },
    { "BRP10.BLY", BR_PathData, BR_CollisionInfo, 43432, 43946 },
    { "BRP11.BLY", BR_PathData, BR_CollisionInfo, 44460, 44974 },
    { "BRP12.BLY", BR_PathData, BR_CollisionInfo, 41376, 41890 },
    { "BRP13.BLY", BR_PathData, BR_CollisionInfo, 41376, 41890 },
    { "BRP14.BLY", BR_PathData, BR_CollisionInfo, 45488, 46002 },
    { "BRP15.BLY", BR_PathData, BR_CollisionInfo, 46516, 47030 },
    { "BRP16.BLY", BR_PathData, BR_CollisionInfo, 51656, 52170 },
    { "BRP17.BLY", BR_PathData, BR_CollisionInfo, 50628, 51142 },
    { "BRP18.BLY", BR_PathData, BR_CollisionInfo, 47544, 48058 },
    { "BRP19.BLY", BR_PathData, BR_CollisionInfo, 48572, 49086 },
    { "BRP20.BLY", BR_PathData, BR_CollisionInfo, 49600, 50114 },
    { "BRP21.BLY", BR_PathData, BR_CollisionInfo, 49600, 50114 },
    { nullptr,     BR_PathData, BR_CollisionInfo, 36750, 37264 },
    { "BRP23.BLY", BR_PathData, BR_CollisionInfo, 49600, 50114 },
    { "BRP24.BLY", BR_PathData, BR_CollisionInfo, 49600, 50114 },
    { "BRP25.BLY", BR_PathData, BR_CollisionInfo, 49600, 50114 },
    { "BRP26.BLY", BR_PathData, BR_CollisionInfo, 43432, 43946 },
    { "BRP27.BLY", BR_PathData, BR_CollisionInfo, 44460, 44974 },
    { "BRP28.BLY", BR_PathData, BR_CollisionInfo, 47544, 48058 },
    { "BRP29.BLY", BR_PathData, BR_CollisionInfo, 49600, 50114 }
};

const PathBlyRec BM_PathBlyRecInfo[] =
{
    kNullPathBlyRec,
    { "BMP1.BLY", BM_PathData, BM_CollisionInfo, 52684, 53198 }
};

const PathBlyRec FD_PathBlyRecInfo[] =
{
    kNullPathBlyRec,
    { "FDP1.BLY",  FD_PathData, FD_CollisionInfo, 17732, 18246 },
    { "FDP2.BLY",  FD_PathData, FD_CollisionInfo, 17732, 18246 },
    { "FDP3.BLY",  FD_PathData, FD_CollisionInfo, 20302, 20816 },
    { "FDP4.BLY",  FD_PathData, FD_CollisionInfo, 21330, 21844 },
    { "FDP5.BLY",  FD_PathData, FD_CollisionInfo, 19274, 19788 },
    { "FDP6.BLY",  FD_PathData, FD_CollisionInfo, 17732, 18246 },
    { "FDP7.BLY",  FD_PathData, FD_CollisionInfo, 20302, 20816 },
    { "FDP8.BLY",  FD_PathData, FD_CollisionInfo, 21330, 21844 },
    { "FDP9.BLY",  FD_PathData, FD_CollisionInfo, 21330, 21844 },
    { "FDP10.BLY", FD_PathData, FD_CollisionInfo, 22358, 22872 },
    { "FDP11.BLY", FD_PathData, FD_CollisionInfo, 23386, 23900 },
    { "FDP12.BLY", FD_PathData, FD_CollisionInfo, 20302, 20816 },
    { "FDP13.BLY", FD_PathData, FD_CollisionInfo, 23386, 23900 },
    { "FDP14.BLY", FD_PathData, FD_CollisionInfo, 17732, 18246 }
};

const PathBlyRec BA_PathBlyRecInfo[] =
{
    kNullPathBlyRec,
    { "BAP1.BLY",  BA_PathData, BA_CollisionInfo, 24414, 24928 },
    { "BAP2.BLY",  BA_PathData, BA_CollisionInfo, 24414, 24928 },
    { "BAP3.BLY",  BA_PathData, BA_CollisionInfo, 28012, 28526 },
    { "BAP4.BLY",  BA_PathData, BA_CollisionInfo, 24414, 24928 },
    { "BAP5.BLY",  BA_PathData, BA_CollisionInfo, 28012, 28526 },
    { nullptr,     BA_PathData, BA_CollisionInfo, 24414, 24928 },
    { "BAP7.BLY",  BA_PathData, BA_CollisionInfo, 24414, 24928 },
    { nullptr,     BA_PathData, BA_CollisionInfo, 24414, 24928 },
    { "BAP9.BLY",  BA_PathData, BA_CollisionInfo, 25956, 26470 },
    { "BAP10.BLY", BA_PathData, BA_CollisionInfo, 28012, 28526 },
    { "BAP11.BLY", BA_PathData, BA_CollisionInfo, 29040, 29554 },
    { nullptr,     BA_PathData, BA_CollisionInfo, 24414, 24928 },
    { "BAP13.BLY", BA_PathData, BA_CollisionInfo, 25956, 26470 },
    { "BAP14.BLY", BA_PathData, BA_CollisionInfo, 26984, 27498 },
    { "BAP15.BLY", BA_PathData, BA_CollisionInfo, 25956, 26470 },
    { "BAP16.BLY", BA_PathData, BA_CollisionInfo, 25956, 26470 }
};

const PathBlyRec TL_PathBlyRecInfo[] =
{
    kNullPathBlyRec,
    { "TLP1.BLY", TL_PathData, TL_CollisionInfo, 209, 0 },
    { "TLP2.BLY", TL_PathData, TL_CollisionInfo, 209, 0, },
};

const PathBlyRec CR_PathBlyRecInfo[] =
{
    kNullPathBlyRec,
    { "CRP1.BLY", CR_PathData, CR_CollisionInfo, 256, 0 },
    { "CRP2.BLY", CR_PathData, CR_CollisionInfo, 256, 0, },
};

// =======================================================================

const FmvInfo kNullFmvInfo = { nullptr, 0, 1, 0, 0 };

const FmvInfo ST_FmvInfo[] =
{
    kNullFmvInfo,
    { "LOGO.STR", 205, 2, 1, 127, },
    { "OPTION.STR", 45, 2, 2, 127, },
    { "GTILOGO.STR", 95, 2, 1, 80, },
    { "BACKSTRY.STR", 1600, 3, 32769, 127, },
    { "INTRO.STR", 530, 2, 1, 127, },
    { "DDLOGO.STR", 95, 2, 1, 127 },
};

const FmvInfo MI_FmvInfo[] =
{
    kNullFmvInfo,
    { "PROPHECY.STR", 440, 3, 1, 127, },
    { "V1A4S01.STR", 449, 3, 2, 127, },
    { "MP1C2C1.STR", 90, 3, 2, 127, },
    { "MP1C1C2.STR", 90, 3, 2, 127, },
    { "MIP01C03.STR", 60, 3, 2, 127, },
    { "MIP01C3R.STR", 60, 3, 2, 127, },
    { "MI122711.STR", 45, 3, 2, 127, },
    { "MI218227.STR", 45, 3, 2, 127, },
    { "MI227218.STR", 45, 3, 2, 127, },
    { "MI313401.STR", 45, 3, 2, 127, },
    { "MI410313.STR", 45, 3, 2, 127, },
    { "MI420432.STR", 45, 3, 2, 127, },
    { "MI404430.STR", 45, 3, 2, 127, },
    { "MI227221.STR", 45, 3, 2, 127, },
    { "MI422437.STR", 45, 3, 2, 127, },
    { "MI222221.STR", 45, 3, 2, 127, },
    { "MI426427.STR", 45, 3, 2, 127, },
    { "MI221227.STR", 45, 3, 2, 127, },
    { "MI430044.STR", 45, 3, 2, 127, },
    { "MI221222.STR", 45, 3, 2, 127, },
    { "MI437422.STR", 45, 3, 2, 127, },
    { "MI122111.STR", 45, 3, 2, 127, },
    { "GAMEBGN.STR", 300, 3, 2, 127, },
    { "VISION.STR", 2739, 3, 32769, 127, },
    { "ESCAPE.STR", 143, 2, 1, 127, },
    { "BREW.STR", 1595, 3, 32769, 127, },
    { "MI111122.STR", 45, 3, 2, 127, },
    { "M1018310.STR", 45, 3, 2, 127, },
    { "M1018517.STR", 45, 3, 2, 127, },
    { "M5171018.STR", 45, 3, 2, 127, },
    { "BREWCAMT.STR", 164, 2, 2, 127, },
    { "MI422423.STR", 45, 3, 2, 127, },
    { "MI711122.STR", 45, 3, 2, 127, },
    { "MI419431.STR", 45, 3, 2, 127, },
    { "MI431419.STR", 45, 3, 2, 127, },
    { "NEBLK201.STR", 45, 3, 2, 127 },
};

const FmvInfo NE_FmvInfo[] =
{
    kNullFmvInfo,
    { "NEP1C7.STR", 60, 3, 2, 127, },
    { "NEP1C11.STR", 60, 3, 2, 127, },
    { "NEP1C12.STR", 60, 3, 2, 127, },
    { "NEP1C13.STR", 60, 3, 2, 127, },
    { "NE330309.STR", 45, 3, 2, 127, },
    { "NE140601.STR", 45, 3, 2, 127, },
    { "NE310033.STR", 45, 3, 2, 127, },
    { "NEP01C08.STR", 60, 3, 2, 127, },
    { "NEP01C09.STR", 60, 3, 2, 127, },
    { "NEP01C10.STR", 60, 3, 2, 127, },
    { "NEP01C8R.STR", 60, 3, 2, 127, },
    { "NEP01C9R.STR", 60, 3, 2, 127, },
    { "NEP1C10R.STR", 60, 3, 2, 127, },
    { "NE140604.STR", 45, 3, 2, 127, },
    { "NE410404.STR", 45, 3, 2, 127, },
    { "NE440401.STR", 45, 3, 2, 127, },
    { "NE610104.STR", 45, 3, 2, 127, },
    { "NE610602.STR", 45, 3, 2, 127, },
    { "NE620603.STR", 45, 3, 2, 127, },
    { "NE630604.STR", 45, 3, 2, 127, },
    { "NE640401.STR", 45, 3, 2, 127, },
    { "REWARD.STR", 2140, 3, 32769, 127, },
    { "NE640601.STR", 45, 3, 2, 127, },
    { "PV131452.STR", 45, 3, 2, 127, },
    { "S1102N58.STR", 45, 3, 2, 127, },
};

const FmvInfo PV_FmvInfo[] =
{
    kNullFmvInfo,
    { "PVP1C5.STR", 45, 3, 2, 127, },
    { "PVP7P10.STR", 60, 3, 2, 127, },
    { "PVP7P10R.STR", 60, 3, 2, 127, },
    { "P1314139.STR", 45, 3, 2, 127, },
    { "P1391314.STR", 45, 3, 2, 127, },
    { "PV12181R.STR", 45, 3, 2, 127, },
    { "PV111081.STR", 45, 3, 2, 127, },
    { "PV112076.STR", 45, 3, 2, 127, },
    { "PV114081.STR", 45, 3, 2, 127, },
    { "PV121081.STR", 45, 3, 2, 127, },
    { "PV131082.STR", 45, 3, 2, 127, },
    { "PV310801.STR", 45, 3, 2, 127, },
    { "PV370801.STR", 45, 3, 2, 127, },
    { "PV410801.STR", 45, 3, 2, 127, },
    { "PV470801.STR", 45, 3, 2, 127, },
    { "PV510801.STR", 45, 3, 2, 127, },
    { "PV550801.STR", 45, 3, 2, 127, },
    { "PV760112.STR", 45, 3, 2, 127, },
    { "PV810301.STR", 45, 3, 2, 127, },
    { "PV810401.STR", 45, 3, 2, 127, },
    { "PV810501.STR", 45, 3, 2, 127, },
    { "PV810901.STR", 45, 3, 2, 127, },
    { "PV811101.STR", 45, 3, 2, 127, },
    { "PV811201.STR", 45, 3, 2, 127, },
    { "NEP02C01.STR", 60, 3, 2, 127, },
    { "PV821301.STR", 45, 3, 2, 127, },
    { "PV910801.STR", 45, 3, 2, 127, },
    { "PVP01C05.STR", 60, 3, 2, 127, },
    { "PVP01C5R.STR", 60, 3, 2, 127, },
    { "REWARD.STR", 2140, 3, 32769, 127, },
    { "PV910801.STR", 45, 3, 2, 127, },
    { "PV107071.STR", 45, 3, 2, 127, },
    { "PV712101.STR", 45, 3, 2, 127, },
    { "PV101712.STR", 45, 3, 2, 127, },
    { "PV131452.STR", 45, 3, 2, 127, },
    { "PV430402.STR", 45, 3, 2, 127 },
};

const FmvInfo FD_FmvInfo[] =
{
    kNullFmvInfo,
    { "ASLIKEXP.STR", 321, 3, 1, 127, },
    { "ASLIKINF.STR", 596, 3, 1, 127, },
    { "INFBOOTH.STR", 567, 3, 1, 127, },
    { "FEECO.STR", 1406, 3, 32769, 127 },
};

const FmvInfo BA_FmvInfo[] =
{
    kNullFmvInfo,
    { "SBP01C01.STR", 60, 3, 2, 127, },
    { "SBP01C1R.STR", 60, 3, 2, 127, },
    { "DRIPEXP.STR", 256, 3, 1, 127, },
    { "DRIPINF.STR", 538, 3, 1, 127, },
    { "TRAIN1.STR", 441, 3, 2, 127, },
    { "BA1114.STR", 60, 3, 2, 127, },
    { "BA1114R.STR", 60, 3, 2, 127, },
};

const FmvInfo SV_FmvInfo[] =
{
    kNullFmvInfo,
    { "SVP1C5.STR", 60, 3, 2, 127, },
    { "SV22073L.STR", 45, 3, 2, 127, },
    { "SV36073R.STR", 45, 3, 2, 127, },
    { "SV110703.STR", 45, 3, 2, 127, },
    { "SV140106.STR", 45, 3, 2, 127, },
    { "SV160703.STR", 45, 3, 2, 127, },
    { "SV220703.STR", 45, 3, 2, 127, },
    { "SV360703.STR", 45, 3, 2, 127, },
    { "SV410703.STR", 45, 3, 2, 127, },
    { "SV420405.STR", 45, 3, 2, 127, },
    { "SV430703.STR", 45, 3, 2, 127, },
    { "SV520703.STR", 45, 3, 2, 127, },
    { "SV550507.STR", 45, 3, 2, 127, },
    { "SV590703.STR", 45, 3, 2, 127, },
    { "SV730101.STR", 45, 3, 2, 127, },
    { "SV730202.STR", 45, 3, 2, 127, },
    { "SV730306.STR", 45, 3, 2, 127, },
    { "SV730401.STR", 45, 3, 2, 127, },
    { "SV730502.STR", 45, 3, 2, 127, },
    { "SV730801.STR", 45, 3, 2, 127, },
    { "SV740904.STR", 45, 3, 2, 127, },
    { "SV810703.STR", 45, 3, 2, 127, },
    { "SV830703.STR", 45, 3, 2, 127, },
    { "SV940704.STR", 45, 3, 2, 127, },
    { "SV630609.STR", 45, 3, 2, 127, },
    { "SV101112.STR", 45, 3, 2, 127, },
    { "SV624063.STR", 45, 3, 2, 127, },
    { "SV616618.STR", 45, 3, 2, 127, },
    { "SV619622.STR", 45, 3, 2, 127, },
    { "SV690613.STR", 45, 3, 2, 127, },
    { "SV520504.STR", 45, 3, 2, 127, },
    { nullptr,        45, 3, 2, 127, },
    { nullptr,        45, 3, 2, 127, },
    { "NEP02C02.STR", 60, 3, 2, 127, },
    { "SVP01C05.STR", 60, 3, 2, 127, },
    { "SVP01C5R.STR", 60, 3, 2, 127, },
    { "REWARD.STR", 2140, 3, 32769, 127, },
};


const FmvInfo BR_FmvInfo[] =
{
    kNullFmvInfo,
    { "BRP01C01.STR", 60, 3, 2, 127, },
    { "BRP01C02.STR", 60, 3, 2, 127, },
    { "BRP01C03.STR", 60, 3, 2, 127, },
    { "BRP01C04.STR", 60, 3, 2, 127, },
    { "BRP01C05.STR", 60, 3, 2, 127, },
    { "BRP01C06.STR", 120, 3, 2, 127, },
    { "BRP01C1R.STR", 60, 3, 2, 127, },
    { "BRP01C2R.STR", 60, 3, 2, 127, },
    { "BRP01C3R.STR", 60, 3, 2, 127, },
    { "BRP01C4R.STR", 60, 3, 2, 127, },
    { "BRP01C5R.STR", 60, 3, 2, 127, },
    { "BRP01C6R.STR", 120, 3, 2, 127, },
    { "CONFRNCE.STR", 424, 3, 1, 127, },
    { "SSINFO.STR", 525, 3, 1, 127, },
    { "TRAIN2.STR", 501, 3, 2, 127, },
    { "INGRDNT.STR", 658, 3, 1, 127, },
    { "GDENDING.STR", 2380, 3, 32769, 127, },
    { "BDENDING.STR", 965, 3, 32769, 127 },
};

const FmvInfo BM_FmvInfo[] =
{
    kNullFmvInfo,
    { "BRP01C01.STR", 60, 3, 2, 127, },
    { "BRP01C02.STR", 60, 3, 2, 127, },
    { "BRP01C03.STR", 60, 3, 2, 127, },
    { "BRP01C04.STR", 60, 3, 2, 127, },
    { "BRP01C05.STR", 60, 3, 2, 127, },
    { "BRP01C06.STR", 120, 3, 2, 127, },
    { "BRP01C1R.STR", 60, 3, 2, 127, },
    { "BRP01C2R.STR", 60, 3, 2, 127, },
    { "BRP01C3R.STR", 60, 3, 2, 127, },
    { "BRP01C4R.STR", 60, 3, 2, 127, },
    { "BRP01C5R.STR", 60, 3, 2, 127, },
    { "BRP01C6R.STR", 120, 3, 2, 127, },
    { "CONFRNCE.STR", 424, 3, 1, 127, },
    { "SSINFO.STR", 525, 3, 1, 127, },
    { "TRAIN2.STR", 501, 3, 2, 127, },
    { "INGRDNT.STR", 658, 3, 1, 127, },
    { "GDENDING.STR", 2380, 3, 32769, 127, },
    { "BDENDING.STR", 965, 3, 32769, 127 },
};

const FmvInfo BW_FmvInfo[] =
{
    kNullFmvInfo,
    { "BWP1C2.STR", 60, 3, 2, 127, },
    { "BWP3P4.STR", 60, 3, 2, 127, },
    { "BWP3P4R.STR", 60, 3, 2, 127, },
    { "PHLEGINF.STR", 674, 3, 1, 127, },
    { "PHLEGEXP.STR", 425, 3, 1, 127, },
    { "TRAIN3.STR", 499, 3, 2, 127 },
};

const FmvInfo TL_FmvInfo[] =
{
    kNullFmvInfo
};

const FmvInfo CR_FmvInfo[] =
{
    kNullFmvInfo,
    { "LOGO.STR", 205, 2, 1, 127, }
};

// ===================================================================

SoundBlockInfo ST_SoundBlockInfo[] =
{
    { "OPTION.VH", "OPTION.VB", -1, 0 },
};

SoundBlockInfo MI_SoundBlockInfo[] =
{
    { "MINES.VH", "MINES.VB", -1, 0 },
};

SoundBlockInfo NE_SoundBlockInfo[] =
{
    { "NECRUM.VH", "NECRUM.VB", -1, 0 },
};

SoundBlockInfo PV_SoundBlockInfo[] =
{
    { "PARVAULT.VH", "PARVAULT.VB", -1, 0 },
};

SoundBlockInfo SV_SoundBlockInfo[] =
{
    { "SCRVAULT.VH", "SCRVAULT.VB", -1, 0 },
};

SoundBlockInfo FD_SoundBlockInfo[] =
{
    { "FEECO.VH", "FEECO.VB", -1, 0 },
};

SoundBlockInfo BA_SoundBlockInfo[] =
{
    { "BARRACKS.VH", "BARRACKS.VB", -1, 0 },
};

SoundBlockInfo SV_Ender_SoundBlockInfo[] =
{
    { "SVENDER.VH", "SVENDER.VB", -1, 0 },
};

SoundBlockInfo BW_SoundBlockInfo[] =
{
    { "BONEWERK.VH", "BONEWERK.VB", -1, 0 },
};

SoundBlockInfo BR_SoundBlockInfo[] =
{
    { "BREWERY.VH", "BREWERY.VB", -1, 0 },
};

SoundBlockInfo BM_SoundBlockInfo[] =
{
    { "BRENDER.VH", "BRENDER.VB", -1, 0 },
};

SoundBlockInfo PV_Ender_SoundBlockInfo[] =
{
    { "PVENDER.VH", "PVENDER.VB", -1, 0 },
};

SoundBlockInfo FD_Ender_SoundBlockInfo[] =
{
    { "FEENDER.VH", "FEENDER.VB", -1, 0 },
};

SoundBlockInfo BA_Ender_SoundBlockInfo[] =
{
    { "BAENDER.VH", "BAENDER.VB", -1, 0 },
};

SoundBlockInfo BW_Ender_SoundBlockInfo[] =
{
    { "BWENDER.VH", "BWENDER.VB", -1, 0 },
};

SoundBlockInfo TL_SoundBlockInfo[] =
{
    { "MINES.VH", "MINES.VB", -1, 0 },
};

SoundBlockInfo CR_SoundBlockInfo[] =
{
    { "OPTION.VH", "OPTION.VB", -1, 0 },
};



ALIVE_ARY(1, 0x559660, PathRoot, 17, sPathData_559660, {});


static void OutputIndent(int indent, std::ostream& s)
{
    std::string sIdent(indent * 4, ' ');
    s << sIdent;
}

static void OutputArrayStart(int indent, std::ostream& s)
{
    OutputIndent(indent, s);
    s << "{\n";
}

static void OutputArrayEnd(int indent, std::ostream& s, bool isEnd = false)
{
    OutputIndent(indent, s);
    if (isEnd)
    {
        s << "}\n";
    }
    else
    {
        s << "}, \n";
    }
}


template<class T>
static void OutputArrayLine(int indent, std::ostream& s, const T& item, bool isEnd = false)
{
    OutputIndent(indent, s);
    s << item;
    if (isEnd)
    {
        //s << "\n";
    }
    else
    {
        s << ", ";
    }
}

static void OutputArrayLine(int indent, std::ostream& s, const char* item, bool isEnd = false)
{
    OutputIndent(indent, s);
    s << "\"";
    s << (item ? item : "");
    if (isEnd)
    {
        s << "";
    }
    else
    {
        s << "\", ";
    }
}

/*
static void Dump(int indent, std::ostream& s, const PathBlyRec* paths, int count, const char* lvlName)
{
    //OutputArrayStart(indent, s);
    //indent++;
    s << "const PathBlyRec " << lvlName << "_PathBlyRecInfo[] =\n";
    s << "{\n";
    for (int i = 0; i < count; i++)
    {
        s << "{ ";
        OutputArrayLine(0, s, paths[i].field_0_blyName);
        s << lvlName << "_PathData, ";
        s << lvlName << "_CollisionInfo, ";
        OutputArrayLine(0, s, paths[i].field_C);
        const bool bEnd = i + 1 != count;
        OutputArrayLine(0, s, paths[i].field_E, bEnd);
        s << "},\n";
    }
    s << "};\n\n";

    //indent--;
   // OutputArrayEnd(indent, s, true);
}
*/

static void Dump(int indent, std::ostream& s, const SoundBlockInfo* sound, const char* lvlName)
{
    s << "SoundBlockInfo " << lvlName << "_SoundBlockInfo[] =\n";
    s << "{\n";
    while (sound->field_0_vab_header_name && sound->field_4_vab_body_name)
    {
        s << "{ ";
        indent = 0;
        OutputArrayLine(indent, s, sound->field_0_vab_header_name);
        OutputArrayLine(indent, s, sound->field_4_vab_body_name);
        OutputArrayLine(indent, s, sound->field_8_vab_id);
        const bool bEnd = (!(sound + 1)->field_0_vab_header_name) && (!(sound + 1)->field_4_vab_body_name);
        OutputArrayLine(indent, s, sound->field_C_pVabHeader, bEnd);
        indent--;
        s << "},\n";
        sound++;
    }
    s << "};\n\n";
}

static void Dump(int indent, std::ostream& s, const FmvInfo* fmv, const char* lvlName)
{
    s << "const FmvInfo " << lvlName << "_FmvInfo[] =\n";
    s << "{\n";
    bool first = true;
    while (fmv->field_0_pName || first)
    {
        first = false;
      
        s << "{ ";
        OutputArrayLine(0, s, fmv->field_0_pName);
        OutputArrayLine(0, s, fmv->field_4_id);
        OutputArrayLine(indent, s, fmv->field_6_flags);
        OutputArrayLine(indent, s, fmv->field_8);
        OutputArrayLine(indent, s, fmv->field_A_volume, (fmv + 1)->field_0_pName == nullptr);
        s << "},\n";
        fmv++;
    }
    s << "};\n\n";
}

static void Dump(std::ostream& s, const PathRoot& pr)
{
    int indent = 0;

    // s << "{\n";
     //auto calculated = (((BYTE*)pr.field_4_pFmvArray - (BYTE*)pr.field_0_pBlyArrayPtr) / sizeof(PathBlyRec));
     //assert(pr.field_1A_num_paths+1 == calculated);
//    Dump(indent, s, pr.field_0_pBlyArrayPtr, pr.field_1A_num_paths + 1, pr.field_18_lvl_name);


    Dump(indent, s, pr.field_8_pMusicInfo, pr.field_18_lvl_name);

/*
    s << ", ";
    s << ", \n";

    s << ", \n";

    OutputArrayLine(indent, s, pr.field_C_bsq_file_name);
    OutputArrayLine(indent, s, pr.field_10_reverb);
    OutputArrayLine(indent, s, pr.field_14_bg_music_id);
    OutputArrayLine(indent, s, pr.field_18_lvl_name);
    OutputArrayLine(indent, s, pr.field_1A_num_paths);
    OutputArrayLine(indent, s, pr.field_1C_unused);
    OutputArrayLine(indent, s, pr.field_1E);
    OutputArrayLine(indent, s, pr.field_22_lvl_name_cd);
    OutputArrayLine(indent, s, pr.field_26);
    OutputArrayLine(indent, s, pr.field_2A_ovl_name_cd);
    OutputArrayLine(indent, s, pr.field_2E);
    OutputArrayLine(indent, s, pr.field_32_mov_name_cd);
    OutputArrayLine(indent, s, pr.field_36_idx_name);
    OutputArrayLine(indent, s, pr.field_3A_bnd_name, true);
    OutputArrayEnd(indent, s);*/
}



const PathBlyRec* CC Path_Get_Bly_Record_460F30(unsigned __int16 lvlId, unsigned __int16 pathId)
{
    
    std::stringstream s;
    for (int i = 0; i < 17; i++)
    {
        Dump(s, sPathData_559660[i]);
    }

    std::string str = s.str();
    // ::OutputDebugString(str.c_str());

    std::ofstream out("output.txt");
    out << str;
    out.close();
    
    abort();

    return &sPathData_559660[lvlId].field_0_pBlyArrayPtr[pathId];
}


struct PerLvlData
{
    const char* field_0_display_name;
    WORD field_4_level;
    WORD field_6_path;
    WORD field_8_camera;
    WORD field_A_scale;
    WORD field_C_abe_x_off;
    WORD field_E_abe_y_off;
};

const static PerLvlData gMovieMenuInfos_561540[28] =
{
    { "GT Logo", 0, 65535, 65535, 3u, 65535, 65535 }, 
    { "Oddworld Intro", 0, 65535, 65535, 1u, 65535, 65535 }, 
    { "Abe's Exoddus", 0, 65535, 65535, 5u, 65535, 65535 }, 
    { "Backstory", 0, 65535, 65535, 4u, 65535, 65535 }, 
    { "Prophecy", 1, 65535, 65535, 1u, 65535, 65535 }, 
    { "Vision", 1, 65535, 65535, 24u, 65535, 65535 }, 
    { "Game Opening", 1, 65535, 65535, 2u, 65535, 65535 }, 
    { "Brew", 1, 65535, 65535, 26u, 65535, 65535 }, 
    { "Brew Transition", 1, 65535, 65535, 31u, 65535, 65535 }, 
    { "Escape", 1, 65535, 65535, 25u, 65535, 65535 }, 
    { "Reward", 2, 65535, 65535, 22u, 65535, 65535 }, 
    { "FeeCo", 5, 65535, 65535, 4u, 65535, 65535 }, 
    { "Information Booth", 5, 65535, 65535, 3u, 65535, 65535 }, 
    { "Train 1", 6, 65535, 65535, 5u, 65535, 65535 }, 
    { "Train 2", 9, 65535, 65535, 15u, 65535, 65535 }, 
    { "Train 3", 8, 65535, 65535, 6u, 65535, 65535 }, 
    { "Aslik Info", 5, 65535, 65535, 2u, 65535, 65535 }, 
    { "Aslik Explodes", 5, 65535, 65535, 1u, 65535, 65535 }, 
    { "Dripek Info", 6, 65535, 65535, 4u, 65535, 65535 }, 
    { "Dripek Explodes", 6, 65535, 65535, 3u, 65535, 65535 }, 
    { "Phleg Info", 8, 65535, 65535, 4u, 65535, 65535 }, 
    { "Phleg Explodes", 8, 65535, 65535, 5u, 65535, 65535 }, 
    { "Soulstorm Info", 9, 65535, 65535, 14u, 65535, 65535 }, 
    { "Ingredient", 9, 65535, 65535, 16u, 65535, 65535 }, 
    { "Conference", 9, 65535, 65535, 13u, 65535, 65535 }, 
    { "Happy Ending", 9, 65535, 65535, 17u, 65535, 65535 }, 
    { "Sad Ending", 9, 65535, 65535, 18u, 65535, 65535 }, 
    { "Credits", 16, 65535, 65535, 65535u, 65535, 65535 }
};


const static PerLvlData gDemoData_off_5617F0[23] =
{
    { "Mudokons 1", 1, 8, 5, 0u, 0, 0 },            // MI P8
    { "Mudokons 2", 1, 8, 32, 1u, 0, 0 },           // MI P8
    { "Mudokons 3", 1, 8, 21, 2u, 0, 0 },           // MI P8
    { "Flying Slig", 1, 9, 18, 4u, 0, 0 },          // MI P9
    { "Blind Mudokons 1", 1, 11, 27, 5u, 0, 0 },    // MI P11
    { "Blind Mudokons 2", 1, 11, 22, 3u, 0, 0 },    // MI P11
    { "Minecar", 1, 12, 2, 6u, 0, 0 },              // MI P12
    { "Fleeches", 2, 7, 1, 7u, 0, 0 },              // NE P7
    { "Paramite Chase", 3, 2, 13, 8u, 0, 0 },       // PV P2
    { "Paramites Talk", 3, 6, 8, 27u, 0, 0 },       // PV P6
    { "Scrab and Fleeches", 4, 12, 2, 9u, 0, 0 },   // SV P12
    { "Invisibility", 4, 13, 5, 10u, 0, 0 },        // SV P13
    { "Farts-a-poppin'", 5, 6, 3, 12u, 0, 0 },      // FD P6
    { "Flying Sligs 2", 5, 12, 1, 14u, 0, 0 },      // FD P12
    { "Baggage Claim", 12, 13, 1, 15u, 0, 0 },      // FD ENDER P13
    { "Shrykull", 6, 3, 10, 16u, 0, 0 },            // BA P3
    { "Crawling Sligs", 6, 4, 6, 17u, 0, 0 },       // BA P4
    { "Slogs Attack", 8, 11, 7, 18u, 0, 0 },        // BW P11
    { "Glukkon", 14, 13, 9, 19u, 0, 0 },            // BW ENDER P13
    { "Angry Mudokons", 9, 13, 10, 22u, 0, 0 },     // BR P13
    { "Sligs", 9, 26, 4, 23u, 0, 0 },               // BR P26
    { "Tortured Mudokons", 9, 27, 7, 24u, 0, 0 },   // BR P27
    { "Greeters Go Boom", 9, 28, 4, 25u, 0, 0 }     // BR P28
};

// Used by the level skip cheat/ui/menu
const static PerLvlData gPerLvlData_561700[17] =
{
    { "Mines", 1, 1, 4, 65535u, 2712, 1300 }, 
    { "Mines Ender", 1, 6, 10, 65535u, 2935, 2525 }, 
    { "Necrum", 2, 2, 1, 65535u, 2885, 1388 }, 
    { "Mudomo Vault", 3, 1, 1, 65535u, 110, 917 }, 
    { "Mudomo Vault Ender", 11, 13, 1, 65535u, 437, 454 }, 
    { "Mudanchee Vault", 4, 6, 3, 65535u, 836, 873 }, 
    { "Mudanchee Vault Ender", 7, 9, 4, 65534u, 1600, 550 }, 
    { "FeeCo Depot", 5, 1, 1, 65535u, 4563, 972 }, 
    { "FeeCo Depot Ender", 12, 11, 5, 65535u, 1965, 1650 }, 
    { "Barracks", 6, 1, 4, 65535u, 1562, 1651 }, 
    { "Barracks Ender", 13, 11, 5, 65535u, 961, 1132 }, 
    { "Bonewerkz", 8, 1, 1, 65535u, 813, 451 }, 
    { "Bonewerkz Ender", 14, 14, 10, 65535u, 810, 710 }, 
    { "Brewery", 9, 16, 6, 65535u, 1962, 1232 }, 
    { "Game Ender", 10, 1, 1, 65535u, 460, 968 }, 
    { "Credits", 16, 1, 1, 65535u, 0, 0 }, 
    { "Menu", 0, 1, 1, 65535u, 0, 0 }
};
