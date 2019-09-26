#include <cstdio>
#include <iostream>
#include <vector>

template <typename CharT, typename ... ArgsT>
std::basic_string<CharT> FormatString(const CharT* format, ArgsT ... args)
{
    auto char_count = snprintf(nullptr, 0, format, args...);
    std::vector<CharT> buffer(char_count + 1);

    auto real_char_count = snprintf(buffer.data(), buffer.size(), format, args...);

    return buffer.data();
}

int main()
{
    auto formatted = FormatString("qqq %d %f %s", 12, 10.5, "aaa");
    std::cout << formatted << std::endl;

    auto em = FormatString("qqq");
    std::cout << em << std::endl;


    return 0;
}
