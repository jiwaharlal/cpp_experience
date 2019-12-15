#include <fstream>
#include <iostream>

int main(int argc, char** argv)
{
    const std::string version =
    "SWIG Version 4.0.2\n\
    \n\
    Compiled with g++ [x86_64-pc-linux-gnu]\n\
    \n\
    Configured options: +pcre\n\
    \n\
    Please see http://www.swig.org for reporting bugs and further information";

    if (argv[1] == std::string("-version"))
    {
        std::cout << version << std::endl;
        return 0;
    }
    else if (argv[1] == std::string("-swiglib"))
    {
        std::cout << "/usr/local/share/swig/4.0.2" << std::endl;
    }

    std::ofstream of("/home/maxim/tmp/args.txt", std::ofstream::out);
    for (int i = 0; i < argc; ++i)
    {
        of << argv[i] << ' ';
    }
    of << std::endl;

    return 0;
}
