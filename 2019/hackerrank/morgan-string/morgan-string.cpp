#include <iostream>
#include <algorithm>
#include <iterator>
#include <sstream>

using namespace std;

// Complete the morganAndString function below.
bool isCapitalChar(char c)
{
    return c >= 'A' && c <= 'Z';
}

string morganAndString(string a, string b) {
    auto a_it = a.begin();
    auto b_it = b.begin();
    std::stringstream ss;

    for (; a_it != a.end() || b_it != b.end(); )
    {
        if (a_it != a.end() && !isCapitalChar(*a_it))
        {
            ++a_it;
            continue;
        }

        if (b_it != a.end() && !isCapitalChar(*b_it))
        {
            ++b_it;
            continue;
        }

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

int main()
{
    //ofstream fout(getenv("OUTPUT_PATH"));

    int t;
    cin >> t;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int t_itr = 0; t_itr < t; t_itr++) {
        string a;
        getline(cin, a);

        string b;
        getline(cin, b);

        string result = morganAndString(a, b);

        std::cout << result << "\n";
    }

    //fout.close();

    return 0;
}
