#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<string> split_string(string);

// Complete the acmTeam function below.
vector<int> acmTeam(vector<string> topic) {
    std::vector<int> score_counts(topic[0].size() + 1, 0);

    for (int i = 0, limit = topic.size() - 1; i < limit; ++i)
    {
        for (int j = i + 1; j < topic.size(); ++j)
        {
            int total = 0;
            for (int k = 0; k < topic[i].size(); ++k)
            {
                if (topic[i][k] == '1' || topic[j][k] == '1')
                    ++total;
            }
            ++score_counts[total];
        }
    }

    for (int i = score_counts.size() - 1; i >= 0; --i)
    {
        if (score_counts[i])
        {
            return {i, score_counts[i]};
        }
    }

    return {0, 0};
}

int main()
{
    string nm_temp;
    getline(cin, nm_temp);

    vector<string> nm = split_string(nm_temp);

    int n = stoi(nm[0]);

    int m = stoi(nm[1]);

    vector<string> topic(n);

    for (int i = 0; i < n; i++) {
        string topic_item;
        getline(cin, topic_item);

        topic[i] = topic_item;
    }

    vector<int> result = acmTeam(topic);

    for (int i = 0; i < result.size(); i++) {
        std::cout << result[i];

        if (i != result.size() - 1) {
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
