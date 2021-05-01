#include <iostream>
#include <optional>
#include <string>

#include <boost/program_options.hpp>

struct CmdLineArgs
{
    std::string search_string;
};

std::optional<CmdLineArgs> parseCmdLine(int argc, char** argv)
{
    namespace po = boost::program_options;

    std::optional<CmdLineArgs> args = CmdLineArgs{};

    po::options_description desc("Find substring in standard input");
    desc.add_options()
        ("help,?", "Produce help message")
        ("search-string", po::value(&args->search_string)->required(), "String to search")
        ;

    po::positional_options_description p;
    p.add("search-string", -1);
    po::variables_map vm;
    try
    {
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        po::notify(vm);
    }
    catch (const po::required_option& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << desc << std::endl;

        return args;
    }

    return args;
}

class Matcher
{
public:
    explicit Matcher(const std::string& target)
        : m_hash(computeHash(target))
    {
    }

    template <typename Iter>
    Iter match(Iter first, Iter last)
    {
    }

    void updateHash(
private:
    long long m_hash;
};

int main(int argc, char** argv)
{
    auto args = parseCmdLine(argc, argv);

    const auto& target = args->search_string;
    auto target_hash = initHash(target.begin(), target.end());
    std::vector<std::string::const_iterator> match_positions;
    while (!std::cin.eof())
    {
        std::string line;
        std::getline(std::cin, line);

        if (line.size() < target.size())
        {
            continue;
        }

        auto line_hash = initHash(line.begin(), std::next(line.begin(), target.size()));
        for (
            auto line_it = line.begin(), match_end_it = std::min(line_it + target.size(), line.end());
            match_end_it != line.end();
            )
        {
            if (line_hash == target_hash && std::equal(target.begin(), target.end(), line_it))
            {
                match_positions.push_back(line_it);
                line_it += target.size();
                match_end_it += target.size();
                line_hash = initHash(line_it, match_end_it);
            }
            else
            {
                ++line_it;
                ++match_end_it;
                line_hash = updateHash(line_hash, *match_end_it);
            }
        }

        //std::cout << "New line: " << line << std::endl;
        if (line.find(args->search_string) != std::string::npos)
        {
            std::cout << line << std::endl;
        }
    }
}
