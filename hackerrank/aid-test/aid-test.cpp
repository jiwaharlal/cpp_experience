//
// Character Filter
//
// (C) By Autonomous Intelligent Driving GmbH
//
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


//
// Filter function
//
size_t filter(void* destination,
              const void* source,
              size_t source_size,
              size_t history_size)
{
    // Fill implementation here
    const size_t size_t_max = 0xffffffffffffffff;
    const char *src_chars = reinterpret_cast<const char *>(source);
    char *dst_chars = reinterpret_cast<char *>(destination);

    size_t prev_positions[256];
    std::fill(prev_positions, prev_positions + 256, size_t_max);
    size_t dst_pos = 0u;

    for (std::size_t pos = 0u; pos != source_size; ++pos) {
      char c = src_chars[pos];
      std::int32_t prev_pos = prev_positions[static_cast<size_t>(c)];
      if (prev_pos == size_t_max || pos - prev_pos > history_size) {
        dst_chars[dst_pos] = c;
        ++dst_pos;
      }

      prev_positions[c] = pos;
    }

    return dst_pos;
}
//
// Filter string. Helper for mapping binary blobs to strings.
//
std::string filter_string(const std::string& source, size_t history_size)
{
    std::vector<char> buffer(source.length());
    size_t filtered_size = filter(buffer.data(), source.c_str(), source.length(), history_size);
    std::string dest(buffer.data(), filtered_size);
    return dest;
}

//
// Main entry point.
//
int main()
{
    while (!std::cin.eof())
    {
        size_t history_size = 0;
        std::cin >> history_size;
        std::string source;
        std::getline(std::cin, source);
        if (history_size == 123)
        {
            source = "b";
            source.resize(500*1024*1024, 'a');
            history_size = 500*1024*1024-1;
        }
        std::cout << filter_string(source, history_size) << std::endl;
    }
    return 0;
}
