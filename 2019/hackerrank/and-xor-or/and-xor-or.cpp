#include <bits/stdc++.h>

#include "helper.hpp"

using namespace std;

vector<string> split_string(string);

/*
 * Complete the andXorOr function below.
 */
Result andXorOr(vector<int> a) {
    /*
     * Write your code here.
     */
    Result best = {0, {0, 0}};
    for (auto it = std::next(a.begin()); it != a.end(); ++it)
    {
        Result val{*it ^ *std::prev(it), {it - a.begin() - 1, it - a.begin()}};
        best = std::max(val, best, cmpByValue(std::less<int>()));
    }

    auto summits = findSummits(a);

    if (summits.empty())
    {
        return best;
    }

    auto max_on_hills = exploreHills(a, summits, std::bit_xor<int>(), cmpByValue(std::greater<int>()));

    return std::max(best, max_on_hills, cmpByValue(std::less<int>()));
}

Result andXorOr2(vector<int> a) {
    /*
     * Write your code here.
     */
    Result best = {0, {0, 0}};
    for (auto it = std::next(a.begin()); it != a.end(); ++it)
    {
        Result val{*it ^ *std::prev(it), {it - a.begin() - 1, it - a.begin()}};
        best = std::max(val, best, cmpByValue(std::less<int>()));
    }

    auto tipping_points = findTippingPoints(a);

    if (tipping_points.second.empty())
    {
        return best;
    }

    auto max_on_hills = exploreHills(a, tipping_points, std::bit_xor<int>(), cmpByValue(std::greater<int>()));

    return std::max(best, max_on_hills, cmpByValue(std::less<int>()));
}

int main()
{
    int a_count;
    cin >> a_count;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string a_temp_temp;
    getline(cin, a_temp_temp);

    vector<string> a_temp = split_string(a_temp_temp);

    vector<int> a(a_count);

    for (int a_itr = 0; a_itr < a_count; a_itr++) {
        int a_item = stoi(a_temp[a_itr]);

        a[a_itr] = a_item;
    }

    auto fullResult = andXorOr2(a);
    int result = fullResult.value;

    std::cout << result << "\n";

    // sanity check
    auto borders = std::minmax(fullResult.idxs.first, fullResult.idxs.second);
    std::cout << "Borders: " << borders.first << ", " << borders.second << std::endl;
    std::vector<int> sub_a(a.begin() + borders.first, a.begin() + borders.second + 1);
    std::sort(sub_a.begin(), sub_a.end());
    std::cout << "Min elements of subrange: " << sub_a[0] << ", " << sub_a[1] << std::endl;

    return 0;
}

vector<string> split_string(string input_string) {
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
