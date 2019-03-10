#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

// Complete the countTriplets function below.
struct TripleIndices
{
    std::vector<int> first;
    std::vector<int> second;
    std::vector<int> third;
};

std::ostream& operator <<(std::ostream& out, const TripleIndices& t)
{
    for (const auto& v : {t.first, t.second, t.third})
    {
        for (auto i : v)
        {
            out << i << ' ';
        }
        out << '\n';
    }
    return out;
}

using TriplesMap = std::unordered_map<long, TripleIndices>;

TriplesMap collectAppearances(const std::vector<long>& v, long r)
{
    TriplesMap result;

    for (int i = 0; i < v.size(); ++i)
    {
        auto value = v[i];
        result[value].first.push_back(i);

        if (value % r == 0)
        {
            auto value_second = value / r;
            result[value_second].second.push_back(i);

            if (value_second % r == 0)
            {
                auto value_third = value_second / r;
                result[value_third].third.push_back(i);
            }
        }
    }

    for (auto& indices_pair : result)
    {
        auto& indices = indices_pair.second;
        std::sort(indices.first.begin(), indices.first.end());
        std::sort(indices.second.begin(), indices.second.end());
        std::sort(indices.third.begin(), indices.third.end());
    }

    return result;
}

long countPossibleTriples(const TripleIndices& indices)
{
    if (indices.first.empty() || indices.second.empty() || indices.third.empty())
        return 0l;
    const auto& second = indices.second;
    const auto& third = indices.third;

    long result = 0l;
    for (auto i1 : indices.first)
    {
        auto it2 = std::upper_bound(second.begin(), second.end(), i1);

        for (; it2 != second.end(); ++it2)
        {
            auto i2 = *it2;
            auto it3 = std::upper_bound(third.begin(), third.end(), i2);

            result += third.end() - it3;
        }
    }

    return result;
}

long countPossibleTriplesFast(const TripleIndices& indices)
{
    if (indices.first.empty() || indices.second.empty() || indices.third.empty())
        return 0;

    std::vector<std::pair<int, long>> combinations_first;
    std::vector<std::pair<int, long>> combinations_second;
    const auto& second = indices.second;
    const auto& third = indices.third;

    for (auto idx : indices.first)
    {
        combinations_first.emplace_back(idx, 0);
    }
    for (auto idx : indices.second)
    {
        combinations_second.emplace_back(idx, 0);
    }

    auto itemsGreaterInThird = [&](int value)
    {
        auto it = std::upper_bound(third.begin(), third.end(), value);
        return third.end() - it;
    };

    combinations_second.back().second = itemsGreaterInThird(combinations_second.back().first);

    for (int i = combinations_second.size() - 2; i >= 0; --i)
    {
        combinations_second[i].second =
            itemsGreaterInThird(combinations_second[i].first) + combinations_second[i + 1].second;
    }

    auto combinationsGreaterInSecond = [&](int value)
    {
        auto it = std::upper_bound(
                combinations_second.begin(),
                combinations_second.end(),
                std::make_pair(value, std::numeric_limits<long>::max()));

        return it == combinations_second.end() ? 0 : it->second;
    };

    combinations_first.back().second =
        combinationsGreaterInSecond(combinations_first.back().first);

    for (int i = combinations_first.size() - 2; i >= 0; --i)
    {
        combinations_first[i].second =
            combinationsGreaterInSecond(combinations_first[i].first) + combinations_first[i + 1].second;
    }

    return combinations_first.front().second;
}

long countTriplets(vector<long> arr, long r) {
    auto value_indices = collectAppearances(arr, r);

    long result = 0;
    for (const auto indices_pair : value_indices)
    {
        result += countPossibleTriplesFast(indices_pair.second);
    }

    return result;
}

int main()
{
    string nr_temp;
    getline(cin, nr_temp);

    vector<string> nr = split(rtrim(nr_temp));

    int n = stoi(nr[0]);

    long r = stol(nr[1]);

    string arr_temp_temp;
    getline(cin, arr_temp_temp);

    vector<string> arr_temp = split(rtrim(arr_temp_temp));

    vector<long> arr(n);

    for (int i = 0; i < n; i++) {
        long arr_item = stol(arr_temp[i]);

        arr[i] = arr_item;
    }

    long ans = countTriplets(arr, r);

    std::cout << ans << "\n";

    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}

vector<string> split(const string &str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}
