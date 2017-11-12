#include <cstdint>
#include <fstream>
#include <iostream>
#include <regex>

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: <exe> <input osm map> <output osm map>" << std::endl;
        return 1;
    }

    std::ifstream fin(argv[1]);

    char buf[256];
    std::regex way_regex("way id=\\\"([-]?\\d+)\\\"");
    std::smatch way_match;

    //std::int32_t way_count = 0;
    while (fin.getline(buf, 256)) //  && way_count < 10)
    {
        std::string str(buf);
        std::cout << str << std::endl;
        if (std::regex_search(str, way_match, way_regex))
        {
            //way_count++;

            std::stringstream ss;
            ss << "    <tag k=\"id\" v=\"" << way_match[1] << "\"/>";
            //std::cout << "id = " << way_match[1] << std::endl;
            std::cout << ss.str() << std::endl;
        }
    }

    //std::cout << "Found ways: " << way_count << std::endl;

    return 0;
}
