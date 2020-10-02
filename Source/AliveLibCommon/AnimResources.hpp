#pragma once

enum class AnimId
{
    AbeHeadGib,
    AbeArmGib,
    AbeBodyGib,
};

struct AnimRecord
{
    AnimId mId;
    const char* mBanName;
    int mFrameTableOffset;
    short mMaxW;
    short mMaxH;
    int mResourceId;
};

const AnimRecord& AnimRec(AnimId toFind);
