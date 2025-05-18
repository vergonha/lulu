#ifndef LULU_LOGGER_HPP
#define LULU_LOGGER_HPP

#include <cstdarg> // Para usar va_list
#include <cstdio>  // Para usar vsnprintf

#define LULU_WARN(msg, ...) Lulu::Logger::warn(msg, ##__VA_ARGS__)
#define LULU_ERROR(msg, ...) Lulu::Logger::error(msg, ##__VA_ARGS__)
#define LULU_TRACE(msg, ...) Lulu::Logger::trace(msg, ##__VA_ARGS__)
#define LULU_ASSERT(c, msg, ...)            \
    {                                       \
        if (!(c))                           \
        {                                   \
            LULU_ERROR(msg, ##__VA_ARGS__); \
            exit(1);                        \
        }                                   \
    }

#define LULU_ASSERT_EQ(x, y, msg, ...)      \
    {                                       \
        if (x != y)                         \
        {                                   \
            LULU_ERROR(msg, ##__VA_ARGS__); \
        }                                   \
    }

namespace Lulu::Logger
{
    enum class TextColor
    {
        Red,
        Green,
        Blue,
        Yellow,
        Default
    };

    static const int TEXT_COLOR_COUNT = 5;
    static const char *TEXT_COLOR_TABLE[TEXT_COLOR_COUNT] = {
        "\033[31m", // Red
        "\033[32m", // Green
        "\033[34m", // Blue
        "\033[33m", // Yellow
        "\033[0m"   // Default (reset)
    };

    const char *getColorCode(TextColor color);
    void message(const char *prefix, const char *msg, TextColor color, va_list args);

    void trace(const char *msg, ...);
    void warn(const char *msg, ...);
    void error(const char *msg, ...);
}

#endif // LULU_LOGGER_HPP
