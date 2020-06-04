#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

vector<string> split_string(string);

template <typename Iterator>
long merge(Iterator begin, Iterator middle, Iterator end, Iterator tmp_out)
{
    auto left_it = begin;
    auto right_it = middle;
    auto out_it = tmp_out;
    long count = 0;

    for (; left_it != middle && right_it != end; )
    {
        if (*right_it < *left_it)
        {
            *out_it = *right_it;
            count += middle - left_it;
            ++right_it;
        }
        else
        {
            *out_it = *left_it;
            ++left_it;
        }
        ++out_it;
    }

    out_it = std::copy(left_it, middle, out_it);
    out_it = std::copy(right_it, end, out_it);

    std::copy(tmp_out, out_it, begin);

    return count;
}

template <typename Iterator>
long mergeSortCount(Iterator begin, Iterator end, Iterator tmp_out)
{
    if (end - begin <= 1)
    {
        return 0;
    }

    auto mid_offset = (end - begin) / 2;
    auto middle = begin + mid_offset;
    auto inv_count = mergeSortCount(begin, middle, tmp_out);
    inv_count += mergeSortCount(middle, end, tmp_out + mid_offset);

    inv_count += merge(begin, middle, end, tmp_out);

    return inv_count;
}

// Complete the countInversions function below.
long countInversions(vector<int> arr) {
    std::vector<int> tmp_arr(arr.size());
    return mergeSortCount(arr.begin(), arr.end(), tmp_arr.begin());
}

int main()
{
    int t;
    cin >> t;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int t_itr = 0; t_itr < t; t_itr++) {
        int n;
        cin >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string arr_temp_temp;
        getline(cin, arr_temp_temp);

        vector<string> arr_temp = split_string(arr_temp_temp);

        vector<int> arr(n);

        for (int i = 0; i < n; i++) {
            int arr_item = stoi(arr_temp[i]);

            arr[i] = arr_item;
        }

        long result = countInversions(arr);

        std::cout << result << "\n";
    }

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
