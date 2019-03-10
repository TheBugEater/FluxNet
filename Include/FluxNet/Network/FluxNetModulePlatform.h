#pragma once

#ifdef _WIN32
#include "Network/Win32/FluxNetModule_Win32.h"
#endif

namespace Flux
{
#ifdef _WIN32
    using NetModule     = NetModuleWindows;
#endif
}

