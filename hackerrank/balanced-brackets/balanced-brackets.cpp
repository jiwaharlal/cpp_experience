#include <iostream>
#include <vector>
#include <fstream>
#include <limits>

using namespace std;

// Complete the isBalanced function below.
char pair_for(char c)
{
    switch (c)
    {
        case ']':
            return '[';
        case ')':
            return '(';
        case '}':
        default:
            return '{';
    }
    return '?';
}

string isBalanced(string s) {
    std::vector<char> opened;
    for (char c : s)
    {
        if (c == '[' || c == '(' || c == '{')
        {
            opened.push_back(c);
            continue;
        }

        if (opened.empty() || opened.back() != pair_for(c))
        {
            return "NO";
        }

        opened.pop_back();
    }

    if (opened.empty())
    {
        return "YES";
    }

    return "NO";
}

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: <executable> <input> <output>" << std::endl;
        return 1;
    }

    std::ifstream input(argv[1]);
    std::ifstream expected_out(argv[2]);

    int t;
    input >> t;
    input.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int t_itr = 0; t_itr < t; t_itr++) {
        string s;
        getline(input, s);

        string result = isBalanced(s);

        std::string expected_result;
        expected_out >> expected_result;

        if (result != expected_result)
        {
            std::cout << "Expected: " << expected_result
                << "\nReal: " << result
                << "\nLine: " << s << std::endl;
        }

        //fout << result << "\n";
    }

    //fout.close();

    return 0;
}
