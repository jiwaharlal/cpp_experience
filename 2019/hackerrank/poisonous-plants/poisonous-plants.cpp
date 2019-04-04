#include <iostream>
#include <vector>
#include <list>
#include <set>

using namespace std;

vector<string> split_string(string);

struct Links
{
    int prev;
    int next;
};

// Complete the poisonousPlants function below.
int poisonousPlants(vector<int> p)
{
    std::vector<Links> still_alive;
    for (int i = 0; i < p.size(); ++i)
    {
        still_alive.push_back(Links{i - 1, i + 1});
    }

    std::vector<int> to_check;
    to_check.reserve(p.size() - 1);
    for (int i = 1; i < p.size(); ++i)
    {
        to_check.push_back(i);
    }

    std::vector<int> to_die;
    while (true)
    {
        to_die.clear();
        std::vector<int> next_check;

        for (int i = 0; i < to_check.size(); ++i)
        {
            int elt_idx = to_check[i];
            int prev_idx = still_alive[elt_idx].prev;
            int next_idx = still_alive[elt_idx].next;

            if (p[elt_idx] > p[prev_idx])
            {
                to_die.push_back(elt_idx);
                next_check.push_back(next_idx);
            }
        }
    }
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string p_temp_temp;
    getline(cin, p_temp_temp);

    vector<string> p_temp = split_string(p_temp_temp);

    vector<int> p(n);

    for (int i = 0; i < n; i++) {
        int p_item = stoi(p_temp[i]);

        p[i] = p_item;
    }

    int result = poisonousPlants(p);

    fout << result << "\n";

    fout.close();

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
