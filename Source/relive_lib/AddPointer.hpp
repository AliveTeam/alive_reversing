#pragma once

// Faster to compile than `std::add_pointer_t`
template <typename T>
using AddPointer_t = T*;
