#include "platform.hpp"
#include "logger.hpp"

#if defined(LULU_LINUX)
#include "platform_specific/linux.cpp"
#elif defined(WIN32)
#include "platform_specific/windows.cpp"
#elif defined(APPLE)
#include "platform_specific/macos.cpp"
#endif

namespace Lulu::Program
{
    std::unique_ptr<Platform> Platform::GetPlatform()
    {
#if defined(WIN32)
        LULU_TRACE("Platform: Windows detected!");
        return std::make_unique<PlatformWindows>();
#elif defined(APPLE)
        LULU_TRACE("Platform: MacOS detected!");
        return std::make_unique<PlatformMacOS>();
#elif defined(LULU_LINUX)
        LULU_TRACE("Platform: Linux detected!");
        return std::make_unique<PlatformLinux>();
#else
        LULU_ERROR("Unsupported platform!");
        return nullptr;
#endif
    }

}
