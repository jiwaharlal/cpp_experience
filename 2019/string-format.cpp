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
    va_list arguments;
    va_start(arguments, format);

    auto char_count = vsnprintf(nullptr, 0, format, arguments);
    std::vector<char> buffer(char_count + 1);

    auto real_char_count = vsnprintf(buffer.data(), buffer.size(), format, arguments);
    va_end(arguments);

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
