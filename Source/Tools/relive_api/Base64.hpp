#pragma once

#include "../../AliveLibCommon/Types.hpp"
#include <vector>
#include <string>

namespace ReliveAPI {
std::string ToBase64(const std::vector<u8>& vec);
std::vector<u8> FromBase64(const std::string& vec);
} // namespace ReliveAPI
