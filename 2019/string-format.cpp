#include <cstdio>
#include <cstdarg>
#include <iostream>
#include <vector>

//template <typename TChar, typename ... ArgsT>
//std::basic_string<TChar> FormatString(const TChar* format, ArgsT ... args)
//{
    //auto char_count = snprintf(nullptr, 0, format, args...);
    //std::vector<TChar> buffer(char_count + 1);

    //auto real_char_count = snprintf(buffer.data(), buffer.size(), format, args...);

    //return buffer.data();
//}

std::string FormatString(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    auto char_count = vsnprintf(nullptr, 0, format, args);
    va_end(args);

    std::cout << "char_count=" << char_count << std::endl;
    std::vector<char> buffer(char_count + 1);

    va_start(args, format);
    auto real_char_count = vsnprintf(buffer.data(), buffer.size(), format, args);
    va_end(args);

    return buffer.data();
}

int main()
{
    auto formatted = FormatString("qqq %d %f %s", 12, 10.5, "aaaaaaaaaa");
    std::cout << formatted << std::endl;

    auto em = FormatString("qqq");
    std::cout << em << std::endl;

    return 0;
}
