#include <string>
#include <iostream>
#include <cstdlib>

const char* getExeName(const char* pathname)
{
    const auto last_slash = std::string(pathname).rfind('/');
    const auto name_offset = last_slash == std::string::npos ? 0 : last_slash + 1;
    const char* exe_name = pathname + name_offset;
}

int main(int argc, char** argv)
{
    std::cout << argv[0] << std::endl;
    std::cout << getExeName(argv[0]) << std::endl;
    std::cout << std::getenv("_") << '\n';

    return 0;
}
