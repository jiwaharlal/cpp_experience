#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <fstream>

using namespace std;

vector<string> split_string(string);

class Median
{
public:
    explicit Median(std::vector<int> initial)
    {
        int middle = initial.size() / 2;
        auto nth = initial.begin() + middle;
        std::nth_element(initial.begin(), nth, initial.end());

        m_bottom = std::multiset<int>(initial.begin(), nth);
        m_top = std::multiset<int>(nth, initial.end());
    }

    int getDoubled()
    {
        if (m_top.size() > m_bottom.size())
        {
            return *m_top.begin() * 2;
        }

        return *std::prev(m_bottom.end()) + *m_top.begin();
    }

    void update(int to_remove, int to_add)
    {
        auto initial_top_size = m_top.size();
        auto initial_bottom_size = m_bottom.size();

        if (to_remove >= *m_top.begin())
        {
            auto it = m_top.find(to_remove);
            m_top.erase(it);
        }
        else
        {
            auto it = m_bottom.find(to_remove);
            m_bottom.erase(it);
        }

        if (to_add >= *m_top.begin())
        {
            m_top.insert(to_add);
        }
        else
        {
            m_bottom.insert(to_add);
        }

        while (m_top.size() > initial_top_size)
        {
            auto tmp = *m_top.begin();
            m_top.erase(m_top.begin());
            m_bottom.insert(tmp);
        }

        while (m_bottom.size() > initial_bottom_size)
        {
            auto tmp = *std::prev(m_bottom.end());
            m_bottom.erase(std::prev(m_bottom.end()));
            m_top.insert(tmp);
        }
    }
private:
    std::multiset<int> m_bottom;
    std::multiset<int> m_top;
};

// Complete the activityNotifications function below.
int activityNotifications(vector<int> expenditure, int d) {
    Median m(std::vector<int>{expenditure.begin(), expenditure.begin() + d});
    int notice_count = 0;

    for (int i = d; i < expenditure.size(); ++i)
    {
        auto doubled_median = m.getDoubled();
        if (expenditure[i] >= doubled_median)
        {
            ++notice_count;
        }
        m.update(expenditure[i - d], expenditure[i]);
    }

    return notice_count;
}

int main()
{
    //ofstream fout(getenv("OUTPUT_PATH"));

    string nd_temp;
    getline(cin, nd_temp);

    vector<string> nd = split_string(nd_temp);

    int n = stoi(nd[0]);

    int d = stoi(nd[1]);

    string expenditure_temp_temp;
    getline(cin, expenditure_temp_temp);

    vector<string> expenditure_temp = split_string(expenditure_temp_temp);

    vector<int> expenditure(n);

    for (int i = 0; i < n; i++) {
        int expenditure_item = stoi(expenditure_temp[i]);

        expenditure[i] = expenditure_item;
    }

    int result = activityNotifications(expenditure, d);

    std::cout << result << "\n";

    //fout.close();

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
