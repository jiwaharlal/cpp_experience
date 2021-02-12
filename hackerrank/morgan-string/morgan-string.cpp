#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

using namespace std;

// Complete the morganAndString function below.

using StrIter = std::string::const_iterator;

enum class TriBool
{
    True,
    False,
    Unclear
};

std::pair<string, string> morganAndString(StrIter a_it, StrIter a_end, StrIter b_it, StrIter b_end)
{
    std::stringstream s;
    std::stringstream source;

    for (; a_it != a_end && b_it != b_end; )
    {
        if (*a_it < *b_it)
        {
            s << *a_it;
            source << 'a';
            ++a_it;
            continue;
        }

        if (*b_it < *a_it)
        {
            s << *b_it;
            source << 'b';
            ++b_it;
            continue;
        }

        const auto first_non_equal_in_a = std::find_if(a_it, a_end, [&](char c){ return c != *a_it; });
        const auto first_non_equal_in_b = std::find_if(b_it, b_end, [&](char c){ return c != *b_it; });

        const char best_a = first_non_equal_in_a == a_end ? *a_it : *first_non_equal_in_a;
        const char best_b = first_non_equal_in_b == b_end ? *b_it : *first_non_equal_in_b;

        const auto distance_a = std::distance(a_it, first_non_equal_in_a);
        const auto distance_b = std::distance(b_it, first_non_equal_in_b);

        // if both are better than current char, choose shorter one
        // if both are better and the distance is the same, choose the one which is comparatively better
        // if the only one is better compared to current, choose it
        TriBool is_a_first = [&]()
        {
            if (best_a < *a_it && best_b < *b_it)
            {
                if (distance_a < distance_b)
                {
                    return TriBool::True;
                }
                else if (distance_b < distance_a)
                {
                    return TriBool::False;
                }

                // is it needed or will be covered by mismatch section?
                if (best_a < best_b)
                {
                    return TriBool::True;
                }
                else if (best_b < best_a)
                {
                    return TriBool::False;
                }
            }
            else if (best_a < *a_it)
            {
                return TriBool::True;
            }
            else if (best_b < *b_it)
            {
                return TriBool::False;
            }

            return TriBool::Unclear;
        }();

        if (is_a_first == TriBool::True)
        {
            std::copy(a_it, first_non_equal_in_a, std::ostream_iterator<char>(s));
            std::fill_n(std::ostream_iterator<char>(source), std::distance(a_it, first_non_equal_in_a), 'a');
            a_it = first_non_equal_in_a;
        }
        else if (is_a_first == TriBool::False)
        {
            std::copy(b_it, first_non_equal_in_b, std::ostream_iterator<char>(s));
            std::fill_n(std::ostream_iterator<char>(source), std::distance(b_it, first_non_equal_in_b), 'b');
            b_it = first_non_equal_in_b;
        }
        else // unclear
        {
            const auto option_a = morganAndString(first_non_equal_in_a, a_end, b_it, b_end);
            const auto option_b = morganAndString(a_it, a_end, first_non_equal_in_b, b_end);

            const auto mm = std::mismatch(option_a.first.begin(), option_a.first.end(), option_b.first.begin());

            const auto& src = [&]() -> const std::pair<std::string, std::string>&
            {
                if (mm.first != option_a.first.end() && *mm.first < *mm.second)
                {
                    std::fill_n(std::ostream_iterator<char>(source), std::distance(b_it, first_non_equal_in_b), 'a');
                    return option_a;
                }
                std::fill_n(std::ostream_iterator<char>(source), std::distance(b_it, first_non_equal_in_b), 'b');
                return option_b;
            }();
            std::copy(a_it, first_non_equal_in_a, std::ostream_iterator<char>(s));
            s << src.first;
            source << src.second;

            return std::make_pair(s.str(), source.str());
        }
    }

    std::copy(b_it, b_end, std::ostream_iterator<char>(s));
    std::fill_n(std::ostream_iterator<char>(source), std::distance(b_it, b_end), 'b');
    std::copy(a_it, a_end, std::ostream_iterator<char>(s));
    std::fill_n(std::ostream_iterator<char>(source), std::distance(a_it, a_end), 'a');

    return std::make_pair(s.str(), source.str());
}

std::pair<string, string> morganAndString(const std::string& a, const std::string& b)
{
    return morganAndString(a.begin(), a.end(), b.begin(), b.end());
}


string morganAndStringOld(string a, string b)
{
    auto a_it = a.begin();
    auto b_it = b.begin();
    std::stringstream ss;

    for (; a_it != a.end() || b_it != b.end(); )
    {
        if (a_it == a.end())
        {
            std::copy(b_it, b.end(), std::ostream_iterator<char>(ss));
            break;
        }
        if (b_it == b.end())
        {
            std::copy(a_it, a.end(), std::ostream_iterator<char>(ss));
            break;
        }

        if (*a_it < *b_it)
        {
            ss << *a_it++;
            continue;
        }
        else if (*b_it < *a_it)
        {
            ss << *b_it++;
            continue;
        }

        auto a_begin = a_it;
        auto b_begin = b_it;

        while (a_it != a.end() && b_it != b.end() && *a_it == *a_begin && *a_it++ == *b_it++);

        if (a_it == a.end())
        {
            std::copy(b_begin, b_it, std::ostream_iterator<char>(ss));
            a_it = a_begin;
            continue;
        }

        if (b_it == b.end())
        {
            std::copy(a_begin, a_it, std::ostream_iterator<char>(ss));
            b_it = b_begin;
            continue;
        }

        if (*a_it < *b_it)
        {
            std::copy(a_begin, a_it, std::ostream_iterator<char>(ss));
            b_it = b_begin;
            continue;
        }

        if (*b_it < *a_it)
        {
            std::copy(b_begin, b_it, std::ostream_iterator<char>(ss));
            a_it = a_begin;
            continue;
        }

        a_it = a_begin;
        b_it = b_begin;

        ss << *a_it++;
    }

    return ss.str();
}

int main(int argc, char** argv)
{
    //ofstream fout(getenv("OUTPUT_PATH"));


    int t;
    cin >> t;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    std::vector<std::string> ref_values;
    if (argc == 2)
    {
        auto results_fin = std::ifstream(argv[1]);
        for (int i = 0; i < t; ++i)
        {
            string s;
            getline(results_fin, s);
            ref_values.push_back(s);
        }
    }

    for (int t_itr = 0; t_itr < t; t_itr++) {
        string a;
        getline(cin, a);

        string b;
        getline(cin, b);

        const auto result = morganAndString(a, b);

        if (!ref_values.empty())
        {
            if (result.first == ref_values[t_itr])
            {
                std::cout << "OK" << std::endl;
            }
            else
            {
                std::cout << "FAIL" << std::endl;
                std::cout << "EXPECTED:\n" << ref_values[t_itr] << "\nACTUAL:\n" << result.first << '\n' << result.second << std::endl;
                std::cout << "a:\n" << a << "\nb:\n" << b << std::endl;
            }
        }
        else
        {
            std::cout << result.first << "\n";
        }
    }

    //fout.close();

    return 0;
}
