#include <cstdarg>
#include <cstdio>
#include "logger.hpp"

namespace Lulu::Logger
{

    const char *getColorCode(TextColor color)
    {
        return TEXT_COLOR_TABLE[static_cast<int>(color)];
    }

    void message(const char *prefix, const char *msg, TextColor color, va_list args)
    {
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), msg, args);
        printf("%s%s%s: %s\033[0m\n", getColorCode(color), prefix, "", buffer);
    }

    void trace(const char *msg, ...)
    {
        va_list args;
        va_start(args, msg);
        message("TRACE", msg, Lulu::Logger::TextColor::Default, args);
        va_end(args);
    }

    void warn(const char *msg, ...)
    {
        va_list args;
        va_start(args, msg);
        message("WARN", msg, Lulu::Logger::TextColor::Yellow, args);
        va_end(args);
    }

    void error(const char *msg, ...)
    {
        va_list args;
        va_start(args, msg);
        message("ERROR", msg, Lulu::Logger::TextColor::Red, args);
        va_end(args);
    }

}
