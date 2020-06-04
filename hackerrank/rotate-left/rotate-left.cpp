#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

template <typename Iterator>
void rotateLeft(Iterator first, Iterator last, int steps)
{
    while (true)
    {
        int len = distance(first, last);

        for (auto dst = first + steps; dst != last; ++first, ++dst)
        {
            swap(*first, *dst);
        }

        if (len % steps == 0)
        {
            return;
        }

        steps = steps - (len % steps);
    }
}

// 1 2 3 4 5 6 7 8
// 6 7 8 4 5 1 2 3   // steps = steps - (len % steps) = 5 - (8 % 5) = 2,  len = steps = 5
// 6 7 8 1 2 3 5 4   // steps = steps - (len % steps) = 2 - (5 % 2) = 1, len = steps = 2
// 6 7 8 1 2 3 4 5

int main()
{
    string nd_temp;
    getline(cin, nd_temp);

    vector<string> nd = split_string(nd_temp);

    int n = stoi(nd[0]);

    int d = stoi(nd[1]);

    string a_temp_temp;
    getline(cin, a_temp_temp);

    vector<string> a_temp = split_string(a_temp_temp);

    vector<int> a(n);

    for (int i = 0; i < n; i++) {
        int a_item = stoi(a_temp[i]);

        a[i] = a_item;
    }

    rotateLeft(a.begin(), a.end(), d);
    copy(a.begin(), a.end(), ostream_iterator<int>(cout, " "));
    cout << endl;

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
