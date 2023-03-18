#pragma once

class AudioStream
{
public:
    void BeginWrite(s16** buffer_ptr, u32* num_frames);
    void EndWrite(u32 num_frames);
};