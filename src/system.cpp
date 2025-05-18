#include "system.hpp"
#include "logger.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "sys/stat.h"

namespace Lulu::System::Memory
{

    LuluAllocator createAllocator(size_t size)
    {
        LuluAllocator _ = {};
        _.memory = malloc(size);
        LULU_ASSERT(_.memory, "Failed to allocate memory.");

        LULU_TRACE("Created Allocator! %zuMB Memory allocated", size / MB(1));
        _.capacity = size;
        _.used = 0;

        return _;
    };

    void *alloc(LuluAllocator *allocator, size_t size)
    {
        void *result = nullptr;

        // new and malloc, by default, align address to 8 bytes (x86) or 16 bytes (x64), which is the optimal for most complex data.
        // https://stackoverflow.com/questions/8752546/how-does-malloc-understand-alignment

        LULU_ASSERT(allocator->used + size <= allocator->capacity, "Allocator is full.");

        // it will work like a dynamically sized array of random pieces of data.
        // the result will always be the initial address where the requested amount of memory is allocated.

        result = (char *)allocator->memory + allocator->used;
        allocator->used += size;

        LULU_TRACE("Memory allocated: %zuK (%.2f%% used)", allocator->used, (allocator->used * 100.0) / allocator->capacity);
        return result;
    }

    void destroyAllocator(LuluAllocator *allocator)
    {
        free(allocator->memory);
        allocator->memory = NULL;
        allocator->capacity = 0;
        allocator->used = 0;
    }

}

namespace Lulu::System::IO
{
    // get the time, in milliseconds, when the file was created.
    long long getTimestamp(const char *file_path)
    {
        struct stat s = {};
        stat(file_path, &s);
        return s.st_mtime;
    }

    bool fileExists(const char *file_path)
    {
        LULU_ASSERT(file_path, "You should provide a valid path to fileExists function.");
        FILE *f = fopen(file_path, "rb");
        if (f)
            fclose(f);
        return f != NULL;
    }

    // https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-copyfile?redirectedfrom=MSDN
    bool copyFile(const char *input_path, const char *output_path)
    {
        LULU_ASSERT(input_path, "You should provide a valid path to copy_file function.");
        LULU_ASSERT(output_path, "You should provide a valid path to copy_file function.");
        LULU_ASSERT(fileExists(input_path), "You should provide an existent file path to copy_file function.");

        FILE *input = fopen(input_path, "rb");
        FILE *output = fopen(output_path, "wb+");

        int buffer_size = 8192;
        void *buffer = malloc(buffer_size);

        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, buffer_size, input)) > 0)
        {
            fwrite(buffer, 1, bytes_read, output);
        }

        fclose(input);
        fclose(output);

        return true;
    }

    long getFileSize(const char *file_path)
    {
        LULU_ASSERT(file_path, "You should provide a valid path to getFileSize function.");
        LULU_ASSERT(fileExists(file_path), "You should provide an existent file path to getFileSize function.");

        FILE *f = fopen(file_path, "rb");
        fseek(f, 0, SEEK_END);
        long size = ftell(f);
        fclose(f);

        return size;
    }

    void *getFileContent(const char *file_path)
    {
        LULU_ASSERT(file_path, "You should provide a valid path to get_file_content function.");
        LULU_ASSERT(fileExists(file_path), "You should provide an existent file path to get_file_content function.");

        void *content = 0;
        long size = getFileSize(file_path);

        // https://learn.microsoft.com/pt-br/cpp/cpp/static-cast-operator?view=msvc-170

        // + 1 for null terminator.
        void *buffer = calloc(1, size + 1);

        content = getFileContent(file_path, (int *)&size, buffer);
        return content;
    }

    void *getFileContent(const char *file_path, Memory::LuluAllocator *allocator)
    {
        LULU_ASSERT(file_path, "You should provide a valid path to get_file_content function.");
        LULU_ASSERT(fileExists(file_path), "You should provide an existent file path to get_file_content function.");

        void *content = 0;
        long size = getFileSize(file_path);

        // https://learn.microsoft.com/pt-br/cpp/cpp/static-cast-operator?view=msvc-170

        // + 1 for null terminator.
        void *buffer = Memory::alloc(allocator, static_cast<size_t>(size + 1));

        content = getFileContent(file_path, (int *)&size, buffer);
        return content;
    }

    void *getFileContent(const char *file_path, int *file_size, void *buffer)
    {
        LULU_ASSERT(file_path, "You should provide a valid path to get_file_content function.");
        LULU_ASSERT(fileExists(file_path), "You should provide an existent file path to get_file_content function.");
        LULU_ASSERT(file_size, "You should provide a valid size to get_file_content function.");
        LULU_ASSERT(buffer, "You should provide an allocated block of memory to get_file_content function.");

        FILE *f = fopen(file_path, "rb");

        fseek(f, 0, SEEK_END);
        *file_size = ftell(f);

        fseek(f, 0, SEEK_SET);
        std::memset(buffer, '\0', (*file_size) + 1);
        int read = fread(buffer, 1, *file_size, f);

        LULU_ASSERT(read, "FREAD function failed.");
        fclose(f);
        return buffer;
    }

    void writeFile(const char *file_path, int content_size, void *buffer)
    {
        LULU_ASSERT(file_path, "You should provide a valid path to write_file function.");
        LULU_ASSERT(fileExists(file_path), "You should provide an existent file path to write_file function.");
        LULU_ASSERT(content_size, "You should provide a valid size to write_file function.");
        LULU_ASSERT(buffer, "You should provide an allocated block of memory to write_file function.");

        FILE *f = fopen(file_path, "wb");
        fwrite(buffer, 1, content_size, f);
        fclose(f);
    }
}
