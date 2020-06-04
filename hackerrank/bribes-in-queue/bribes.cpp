#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <queue>
#include <set>

using namespace std;

vector<string> split_string(string);

// Complete the minimumBribes function below.
using QueueState = std::vector<std::pair<int, int>>;

QueueState initialState(const std::vector<int>& v)
{
    QueueState result;
    result.reserve(v.size());
    std::transform(
            v.begin(),
            v.end(),
            std::back_inserter(result),
            [](int i){ return std::make_pair(i, 0); });

    return result;
}

bool isTransitionPossible(const QueueState& s, std::size_t i)
{
    return i < s.size() - 1u && s[i].second < 2 &&
        s[i].first - 1 > i && s[i].first > s[i + 1].first;
}

QueueState transit(const QueueState& s, std::size_t i)
{
    QueueState result(s);
    ++result[i].second;
    std::swap(result[i], result[i + 1u]);

    return result;
}

bool isCorrectQueue(const QueueState& s)
{
    for (std::size_t i = 0u; i < s.size(); ++i)
    {
        if (s[i].first - 1 != static_cast<int>(i))
            return false;
    }

    return true;
}

using StepsStatePair = std::pair<int, QueueState>;

void minimumBribes(vector<int> end_state) {
    auto s = initialState(end_state);

    std::queue<StepsStatePair> q;
    q.push(std::make_pair(0, s));

    std::set<QueueState> visited;
    visited.insert(s);

    bool is_found = false;

    while (!q.empty() && !is_found)
    {
        const auto cur = std::move(q.front());
        q.pop();

        for (std::size_t i = 0, limit = cur.second.size(); i != limit; ++i)
        {
            if (!isTransitionPossible(cur.second, i))
                continue;

            const auto new_queue = transit(cur.second, i);

            if (isCorrectQueue(new_queue))
            {
                std::cout << cur.first + 1 << std::endl;
                return;
            }

            if (visited.find(new_queue) == visited.end())
            {
                q.push(std::make_pair(cur.first + 1, new_queue));
                visited.insert(new_queue);
            }
        }
    }

    std::cout << "Too chaotic\n";
}

void calc(std::vector<int> q)
{
    int ans = 0;
    for (int i = q.size() - 1; i >= 0; i--) {
    if (q[i] - (i + 1) > 2) {
        cout << "Too chaotic" << endl;
        return;
    }
    for (int j = max(0, q[i] - 2); j < i; j++)
        if (q[j] > q[i])
            ans++;
    }
    cout << ans << endl;
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

        string q_temp_temp;
        getline(cin, q_temp_temp);

        vector<string> q_temp = split_string(q_temp_temp);

        vector<int> q(n);

        for (int i = 0; i < n; i++) {
            int q_item = stoi(q_temp[i]);

            q[i] = q_item;
        }

        calc(q);
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
