#include "stdafx.h"
#include "Error.hpp"
#include "Function.hpp"
#include "../AliveLibAE/Error.hpp"

namespace AO {

EXPORT void Error_WarningMessageBox_48E470(const s8* pWarningMsg, ...)
{
    AE_IMPLEMENTED();
    // HACK: VA_ARGS can't be forwarded, but no big deal as this will be resolved when engines are merged
    Error_WarningMessageBox_4F2D80(pWarningMsg);
}

EXPORT void CC Error_ShowErrorStackToUser_48DF10(bool bDisplayAsOneMessage)
{
    AE_IMPLEMENTED();
    Error_ShowErrorStackToUser_4F2A70(bDisplayAsOneMessage);
}

} // namespace AO
