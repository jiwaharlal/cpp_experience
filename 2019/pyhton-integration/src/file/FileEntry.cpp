#include <FileEntry.hpp>

namespace file
{

FileEntry::FileEntry(std::ifstream&& f)
    : m_f(std::move(f))
{
}

FileEntry::FileEntry(FileEntry&& other)
    : m_f(std::move(other.m_f))
{
}

} // namespace file
