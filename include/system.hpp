#ifndef LULU_SYSTEM_HPP
#define LULU_SYSTEM_HPP

#pragma once
#include <cstddef>
#include <cstdlib>
#include <stdlib.h>
#include <string.h>

#define BIT(x) 1 << (x)
#define KB(x) ((unsigned long long)1024 * x)
#define MB(x) ((unsigned long long)1024 * KB(x))
#define GB(x) ((unsigned long long)1024 * MB(x))

namespace Lulu::System::Memory
{
    struct LuluAllocator
    {
        size_t capacity;
        size_t used;
        void *memory;
    };

    void *alloc(LuluAllocator *allocator, size_t size);
    void destroAllocator(LuluAllocator *allocator);
    LuluAllocator createAllocator(size_t size);

}

namespace Lulu::System::IO
{
    long long getTimestamp(const char *file_path);
    bool fileExists(const char *file_path);
    long getFileSize(const char *file_path);
    void *getFileContent(const char *file_path, Memory::LuluAllocator *allocator);
    void *getFileContent(const char *file_path);
    void *getFileContent(const char *file_path, int *file_size, void *buffer);
    bool copyFile(const char *input_path, const char *output_path);
    void writeFile(const char *file_path, int content_size, void *buffer);

}

#endif // LULU_SYSTEM_HPP
