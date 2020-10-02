#include "stdafx_common.h"
#include "AnimResources.hpp"
#include "Sys_common.hpp"

constexpr AnimRecord kAnimRecords[] =
{
    { AnimId::AbeHeadGib, "ABEBLOW.BAN", 7732, 50, 25, 25 },
    { AnimId::AbeArmGib, "ABEBLOW.BAN", 7772, 50, 25, 25 },
    { AnimId::AbeBodyGib, "ABEBLOW.BAN", 7812, 50, 25, 25 },
};

const AnimRecord& AnimRec(AnimId toFind)
{
    for (const auto& anim : kAnimRecords)
    {
        if (anim.mId == toFind)
        {
            return anim;
        }
    }
    ALIVE_FATAL("Missing animation entry");
}
