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

    long result = 0l;
    for (auto i1 : indices.first)
    {
        const auto& second = indices.second;
        auto it2 = std::upper_bound(second.begin(), second.end(), i1);

        for (; it2 != second.end(); ++it2)
        {
            auto i2 = *it2;
            const auto& third = indices.third;
            auto it3 = std::upper_bound(third.begin(), third.end(), i2);

            result += third.end() - it3;
        }
    }

    return result;
}

long countPossibleTriplesFast(const TripleIndices& indices)
{
    std::unordered_map<int, long> combinations_first;
    std::unordered_map<int, long> combinations_second;
}

long countTriplets(vector<long> arr, long r) {
    auto value_indices = collectAppearances(arr, r);

    long result = 0;
    for (const auto indices_pair : value_indices)
    {
        result += countPossibleTriples(indices_pair.second);
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
