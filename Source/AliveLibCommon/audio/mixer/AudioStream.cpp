#pragma once

#include "AudioStream.hpp"

// Constructor will prepare SDL and SDL callback
// Will need to maintain a buffer that gets copied to SDL
// Will probably have to be some 20ms behind so SDL
// will always have samples available

void AudioStream::BeginWrite(s16** buffer_ptr, u32* num_frames)
{
    buffer_ptr;
    num_frames;
}

void AudioStream::EndWrite(u32 num_frames)
{
    num_frames;
}