#pragma once

class BaseGameObject;
struct IndexedPoint;

using TFrameCallBackType = void(*)(BaseGameObject*, u32& pointIdx, const IndexedPoint&);

namespace AO {
extern TFrameCallBackType gAbe_Anim_Frame_Fns[];
extern TFrameCallBackType gSlig_Anim_Frame_Fns[];
extern TFrameCallBackType gSlog_Anim_Frame_Fns[];
extern TFrameCallBackType gZBall_Anim_Frame_Fns[];
} // namespace AO
