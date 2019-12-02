#pragma once

#include <fstream>

namespace file
{

class FileEntry
{
public:
    FileEntry(const FileEntry&) = delete;
    FileEntry& operator=(const FileEntry&) = delete;

    FileEntry(std::ifstream&& f);
    FileEntry(FileEntry&& other);

private:
    std::ifstream m_f;
};

} // namespace file
