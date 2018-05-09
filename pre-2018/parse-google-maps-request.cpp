#include <iostream>
#include <fstream>
#include <regex>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "You probably forgot the request" << std::endl;
        return 1;
    }

    std::ifstream fin(argv[1]);
    std::string request;

    if (fin.is_open())
    {
        fin >> request;
        std::cout << "Request: " << request << std::endl;
    }
    else
    {
        std::cout << "Error parsing request file" << std::endl;
        return 1;
    }

    std::string expression_double("-{0,1}\\d{2,3}\\.\\d{1,7}");
    std::string normal_coord = "(" + expression_double + "),(" + expression_double + ")";
    std::string reverse_coord = "(" + expression_double + ")!2d(" + expression_double + ")";

    std::regex double_ex("-{0,1}\\d{2,3}\\.\\d{1,7}");
    using regex_iterator_str = std::regex_iterator<std::string::iterator>;
    regex_iterator_str rit(request.begin(), request.end(), double_ex);
    regex_iterator_str rend;

    std::regex normal_ex(normal_coord);
    regex_iterator_str nrit(request.begin(), request.end(), normal_ex);

    for (;rit != rend; ++rit)
    {
        std::cout << "Found: " << rit->str() << std::endl;
    }
    //std::cout << "Request:\n" << argv[1] << std::endl;

    for ( ;nrit != rend; ++nrit)
    {
        std::cout << "Normal: " << nrit->str() << " : " << (*nrit)[1] << "," << (*nrit)[2] << std::endl;
    }

    std::regex reverse_ex(reverse_coord);
    regex_iterator_str rrit(request.begin(), request.end(), reverse_ex);

    for ( ;rrit != rend; ++rrit)
    {
        std::cout << "Reverse: " << rrit->str() << " : " << (*rrit)[2] << "," << (*rrit)[1] << std::endl;
    }

    return 0;
}
