#include <bits/stdc++.h>

using namespace std;

// Complete the timeInWords function below.
static const std::array<std::string, 21> digits =
{
    "zero",
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine",
    "ten",
    "eleven",
    "twelve",
    "thirteen",
    "fourteen",
    "quarter",
    "sixteen",
    "seventeen",
    "eighteen",
    "nineteen",
    "twenty"
};

string timeInWords(int h, int m) {
    auto hours_word = digits[h];
    if (m == 0)
    {
        return hours_word + " o' clock";
    }

    const auto minute_relation = [&]() -> std::string
    {
        if (m <= 30)
        {
            return "past";
        }
        m = 60 - m;
        hours_word = digits[h + 1];
        return "to";
    }();

    const auto minute_word = [&]() -> std::string
    {
        if (m == 15)
        {
            return "quarter";
        }
        if (m == 30)
        {
            return "half";
        }

        const auto digits_word = [&]() -> std::string
        {
            if (m <= 20)
            {
                return digits[m];
            }
            return digits[20] + ' ' + digits[m - 20];
        }();

        return digits_word + (m == 1 || m == 21 ? " minute" : " minutes");
    }();

    return minute_word + ' ' + minute_relation + ' ' + hours_word;
}

int main()
{
    //ofstream fout(getenv("OUTPUT_PATH"));

    int h;
    cin >> h;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int m;
    cin >> m;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string result = timeInWords(h, m);

    std::cout << result << "\n";

    //std::cout.close();

    return 0;
}
