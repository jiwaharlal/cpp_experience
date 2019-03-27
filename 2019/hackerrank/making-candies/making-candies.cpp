#include <algorithm>
#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

vector<string> split_string(string);

std::pair<long, long> calcNewProds(long m, long w, long add_prod)
{
    std::pair<long, long> result = std::minmax(m, w);
    result.first += add_prod / 2 + add_prod % 2;
    result.second += add_prod / 2;

    return result;
}

template <typename T>
T upperDiv(T a, T b)
{
    T result = a / b;
    if (a % b != 0)
    {
        ++result;
    }

    return result;
}

long daysToProduce(long items, std::pair<long, long> prod)
{
    return upperDiv(items, prod.first * prod.second);
}

// Complete the minimumPasses function below.
long minimumPasses(long m, long w, long p, long n)
{
    std::vector<std::pair<long, long>> prod;
    long cache = 0;

    long min_total_days = std::numeric_limits<long>::max();
    int min_day_idx = 0;
    long days_passed = 0;
    do
    {
        long prod_rate = m * w;

        long total_days = upperDiv(n - cache, prod_rate) + days_passed;

        if (total_days < min_total_days)
        {
            min_total_days = total_days;
            min_day_idx = days_passed;
        }

        prod.emplace_back(m, w);

        cache += prod_rate;
        long add_prod = cache / p;
        cache %= p;

        std::tie(m, w) = calcNewProds(m, w, add_prod);
        ++days_passed;
    } while (m * w < n);

    return min_total_days;
}

struct DayData
{
    unsigned long day_idx;
    unsigned long w;
    unsigned long m;
    unsigned long cache;
};

unsigned long detailedEstimate(const DayData& day_data, unsigned long p, unsigned long n)
{
    auto cache = day_data.cache;
    auto w = day_data.w;
    auto m = day_data.m;

    unsigned long min_days = std::numeric_limits<unsigned long>::max();

    while (cache >= p)
    {
        auto modified_prod = calcNewProds(m, w, 1);

        unsigned long original_days = daysToProduce(n - cache, std::make_pair(m, w));
        unsigned long modified_days = daysToProduce(n - (cache - p), modified_prod);

        if (modified_days > original_days)
        {
            return original_days;
        }

        min_days = std::min(min_days, modified_days);

        cache -= p;
        std::tie(m, w) = modified_prod;
    }

    return min_days;
}

long minDaysROI(unsigned long m, unsigned long w, unsigned long p, unsigned long n)
{
    if ((n / m) / w == 0)
    {
        return 1;
    }

    unsigned long cache = 0;

    unsigned long day_profit = m * w;
    cache += day_profit;

    DayData prev_day_data;

    for (long day_idx = 1; day_idx < n; )
    {
        if (cache >= n)
        {
            return day_idx;
        }

        if (cache >= p)
        {
            auto prod_inc = cache / p;
            auto modified_prod = calcNewProds(m, w, prod_inc);

            auto original_days = daysToProduce(n - cache, std::make_pair(m, w));
            auto modified_days = daysToProduce(n - (cache - p * prod_inc), modified_prod);

            if (modified_days == original_days)
            {
                auto detailed_days = detailedEstimate({day_idx, w, m, cache}, p, n);
                if (detailed_days < original_days)
                {
                    return day_idx + detailed_days;
                }
            }

            if (modified_days > original_days)
            {
                return prev_day_data.day_idx + detailedEstimate(prev_day_data, p, n);
            }

            prev_day_data = DayData{day_idx, w, m, cache};

            cache -= p * prod_inc;
            std::tie(m, w) = modified_prod;
        }

//        std::cout << "Invest in units: " << prod_inc << " cache left: " << cache << std::endl;

        day_profit = m * w;

        unsigned long add_days = 1;
        if (cache + day_profit < p)
        {
            add_days = upperDiv(std::min(n, p) - cache, day_profit);
        }

        day_idx += add_days;
        cache += add_days * day_profit;
    }

    return n;
}

int main()
{
    string mwpn_temp;
    getline(cin, mwpn_temp);

    vector<string> mwpn = split_string(mwpn_temp);

    long m = stol(mwpn[0]);

    long w = stol(mwpn[1]);

    long p = stol(mwpn[2]);

    long n = stol(mwpn[3]);

    long result = minDaysROI(m, w, p, n);

    std::cout << result << "\n";

    return 0;
}

vector<string> split_string(string input_string)
{
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}
