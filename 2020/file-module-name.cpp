#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>

#include <windows.h>

std::string getErrorByCode(DWORD errorCode)
{
    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;
}

  std::wstring GetExecutableFileName()
  {
    const std::size_t buf_len = 2048;
    wchar_t buf[buf_len];
    const auto res = GetModuleFileNameW(0, buf, buf_len);

    return std::filesystem::path{ buf, buf + res }.filename();
  }

int main()
{
    //wchar_t buf[4096];
    //auto res = GetModuleFileNameW(NULL, buf, 4096);
    //if (res != ERROR_SUCCESS)
    //{
        //std::cout << "Error occured: " << getErrorByCode(GetLastError()) << std::endl;
    //}

    auto exe_name = GetExecutableFileName();
    std::replace(exe_name.begin(), exe_name.end(), L'.', L'_');

    std::wcout << exe_name << std::endl;
    //std::wcout << buf << std::endl;

    return 0;
}
