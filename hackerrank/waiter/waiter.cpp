#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

/*
 * Complete the waiter function below.
 */

std::vector<int> generatePrimes(int count)
{
    std::vector<int> result;
    result.reserve(count);
    result.push_back(1);
    result.push_back(2);

    for (int value = 3; result.size() < count; ++value)
    {
        const bool is_divisible = std::any_of(
                result.begin() + 1,
                result.end(),
                [&](int divisor){ return value % divisor == 0; });

        if (is_divisible)
        {
            continue;
        }

        result.push_back(value);
    }

    return result;
}

vector<int> waiter(vector<int> number, int q) {
    /*
     * Write your code here.
     */
    std::vector<std::vector<int>> bs(q + 1, std::vector<int>());

    auto primes = generatePrimes(q + 1);

    for (int i = 1; i <= q; ++i)
    {
        std::vector<int> tmp;
        std::partition_copy(
                number.rbegin(),
                number.rend(),
                std::back_inserter(bs[i]),
                std::back_inserter(tmp),
                [&](int val){ return val % primes[i] == 0; });

        number.swap(tmp);
    }

    std::vector<int> result;
    result.reserve(number.size());

    for (const auto& b : bs)
    {
        std::copy(b.rbegin(), b.rend(), std::back_inserter(result));
    }
    std::copy(number.rbegin(), number.rend(), std::back_inserter(result));

    return result;
}

int main()
{
    string nq_temp;
    getline(cin, nq_temp);

    vector<string> nq = split_string(nq_temp);

    int n = stoi(nq[0]);

    int q = stoi(nq[1]);

    string number_temp_temp;
    getline(cin, number_temp_temp);

    vector<string> number_temp = split_string(number_temp_temp);

    vector<int> number(n);

    for (int number_itr = 0; number_itr < n; number_itr++) {
        int number_item = stoi(number_temp[number_itr]);

        number[number_itr] = number_item;
    }

    vector<int> result = waiter(number, q);

    for (int result_itr = 0; result_itr < result.size(); result_itr++) {
        std::cout << result[result_itr];

        if (result_itr != result.size() - 1) {
            std::cout << "\n";
        }
    }

    std::cout << "\n";

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
