#include <filesystem>
#include <iostream>
#include <stdlib.h>
#include <string>

int main()
{
    char *pValue;
    size_t len;

    errno_t err = _dupenv_s( &pValue, &len, "USERPROFILE" );

    if ( err )
    {
        return -1;
    }

    std::string home_path(pValue);
    free( pValue );

    std::cout << home_path << std::endl;

    std::string program_files_path = "C:\\Program Files";

    namespace fs = std::experimental::filesystem;

    auto s = fs::status(program_files_path);
    std::cout << static_cast<long long>(s.permissions()) << std::endl;
    s = fs::status(home_path);
    std::cout << static_cast<long long>(s.permissions()) << std::endl;

    return 0;
}
