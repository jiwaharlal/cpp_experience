#include <iostream>

#ifdef _WIN32
#define MATSDK_OS_WIN
#endif

#ifdef MATSDK_OS_WIN
#include <tchar.h>
typedef wchar_t  char_t;
#define MATSDK_CHAR_WIDE
#else
typedef char char_t;

#define _T(x) x
#endif

namespace std
  {
  typedef basic_string<char_t> string_t;
  typedef basic_istream<char_t> istream_t;
  typedef basic_ostream<char_t> ostream_t;
  typedef basic_ifstream<char_t> ifstream_t;
  typedef basic_ofstream<char_t> ofstream_t;
  typedef basic_ostringstream<char_t> ostringstream_t;
  typedef basic_istringstream<char_t> istringstream_t;
  typedef basic_stringstream<char_t> stringstream_t;
  typedef basic_stringbuf<char_t> stringbuf_t;
  }

#if defined MATSDK_CHAR_WIDE
#define mat_open_t _wopen
#define mat_fopen_t _wfopen
#define mat_fprintf_t fwprintf
#define mat_printf_t wprintf
#define mat_remove_t _wremove
#define mat_cout_t std::wcout
#define mat_cerr_t std::wcerr
#define mat_rename_t _wrename
#else
#define mat_open_t open
#define mat_fopen_t fopen
#define mat_fprintf_t fprintf
#define mat_printf_t printf
#define mat_remove_t remove
#define mat_cout_t std::cout
#define mat_cerr_t std::cerr
#define mat_rename_t rename
#endif

enum class ConsoleColor
{
    Red = 9,
    Green = 2, 
    Gray = 8,
    Normal = 0
};

std::ostream_t& operator <<(std::ostream_t& out, ConsoleColor c)
{
    if (c == ConsoleColor::Normal)
    {
        return out << _T("\033[0m");
    }

    return out << _T("\033[38;5;") << static_cast<int>(c) << _T('m');
}

int main()
{
    mat_cout_t << _T("\033[1;31mbold red text\033[0m\n");

    //mat_cout_t << "\033[38;5;9m";
    mat_cout_t << ConsoleColor::Red << _T("Should be red\n");

    //mat_cout_t << "\033[0m";
    mat_cout_t << ConsoleColor::Normal << _T("Should be normal again\n");

    mat_cout_t << ConsoleColor::Gray << _T("Should be gray\n") << ConsoleColor::Normal;

    mat_cout_t << ConsoleColor::Green << _T("Hello!\n");

    return 0;
}
