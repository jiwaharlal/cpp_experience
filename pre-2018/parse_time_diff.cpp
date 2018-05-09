#include <regex>
#include <cstdlib>
#include <iostream>

int64_t parseTimeString(const std::string& str)
{
    const char* expression = "([+-])(\\d+)([ms]?)";
    const std::regex re(expression);

    auto match_begin = std::sregex_iterator(str.begin(), str.end(), re);
    if (match_begin == std::sregex_iterator())
    {
        std::cout << "Wrong value" << std::endl;
        return 0;
    }
    else
    {
        const int64_t sign = ((*match_begin)[1] == '+') ? 1 : -1;

        int64_t value = std::atoi((*match_begin)[2].str().c_str());

        int64_t multiplier = 1; // default is seconds

        switch ((*match_begin)[3].str()[0])
        {
            case 'm':
                multiplier = 60;
                break;
            case 's':
                // any other cases?
            default:
                break;
        }

        return sign * value * multiplier;
    }
}

//void parseTimeString(const std::string& str)
//{
    //const char* expression = "([+-])(\\d+)([ms]?)";
    //const std::regex re(expression);

    //auto match_begin = std::sregex_iterator(str.begin(), str.end(), re);
    //if (match_begin == std::sregex_iterator())
    //{
        //std::cout << "Could not parse this line" << std::endl;
    //}
    //else
    //{
        //for (const auto& match : *match_begin)
        //{
            //std::cout << "match : " << match << std::endl;
        //}

        //const int64_t sign = ((*match_begin)[1] == '+') ? 1 : -1;

        //int64_t value = std::atoi((*match_begin)[2].str().c_str());

        //int64_t multiplier = 1; // default is seconds

        //switch ((*match_begin)[3].str()[0])
        //{
            //case 'm':
                //multiplier = 60;
                //break;
            //case 's':
                //// any other cases?
            //default:
                //break;
        //}

        //std::cout << "Adjustment: " << sign * value * multiplier << " seconds" << std::endl;
    //}

    //auto match = std::regex_match(str, re);
//}

int main(int argc, char** argv)
{
    int64_t diff;

    if (argc == 2)
    {
        diff = parseTimeString(argv[1]);
    }
    else
    {
        diff = parseTimeString("-5m");
    }

    std::cout << "Diff : " << diff << std::endl;

    return 0;
}
