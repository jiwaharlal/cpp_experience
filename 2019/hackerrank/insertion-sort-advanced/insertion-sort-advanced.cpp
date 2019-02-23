#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

using namespace std;

vector<string> split_string(string);

class SmallObjectAllocator
{
public:
    SmallObjectAllocator()
    {
        m_chunks.emplace_back(m_chunk_size, 0);
    }

    template <typename T, typename ... ArgTypes>
    T* allocate(ArgTypes ... args)
    {
        //auto& last_chunk = m_chunks.back();
        if (sizeof(T) + m_chunk_pos >= m_chunk_size)
        {
            m_chunks.emplace_back(m_chunk_size, 0);
            m_chunk_pos = 0;
        }

        auto obj = new (m_chunks.back().data() + m_chunk_pos) T(args...);
        m_chunk_pos += sizeof(T);

        return obj;
    }

    void print()
    {
        std::cout << *(reinterpret_cast<std::string*>(m_chunks.back().data())) << std::endl;
    }

private:
    std::vector<std::vector<char>> m_chunks;
    int m_chunk_size = 0x10000;
    size_t m_chunk_pos = 0u;
};

class LGTreeLeaf
{
public:
    explicit LGTreeLeaf(SmallObjectAllocator* al)
        : LGTreeLeaf(1, 0, true, al)
    {}

    LGTreeLeaf(int level, int parent_border, bool is_greater_direction, SmallObjectAllocator* al)
        : m_level(level)
        , m_al(al)
    {
        int modifier = 1 << (sizeof(int) * 8 - level);
        m_border = is_greater_direction ? parent_border + modifier : parent_border - modifier;
    }

    int countGreater(int value)
    {
        if (value == m_border)
        {
            return m_g_count;
        }
        else if (value < m_border)
        {
            int count = m_g_count + m_equal_count;

            if (m_l_branch)
            {
                count += m_l_branch->countGreater(value);
            }

            return count;
        }
        else // value > m_border
        {
            if (m_g_branch)
            {
                return m_g_branch->countGreater(value);
            }
            else
            {
                return 0;
            }
        }
    }

    int countGreaterAndPut(int value)
    {
        if (value == m_border)
        {
            ++m_equal_count;
            return m_g_count;
        }
        else if (value < m_border)
        {
            int count = m_g_count + m_equal_count;

            if (!m_l_branch)
            {
                m_l_branch = m_al->allocate<LGTreeLeaf>(m_level + 1, m_border, false, m_al);
            }
            count += m_l_branch->countGreaterAndPut(value);

            return count;
        }
        else // value > m_border
        {
            ++m_g_count;
            if (!m_g_branch)
            {
                m_g_branch = m_al->allocate<LGTreeLeaf>(m_level + 1, m_border, true, m_al);
            }
            return m_g_branch->countGreaterAndPut(value);
        }
    }

    void put(int value)
    {
        if (value == m_border)
        {
            ++m_equal_count;
        }
        else if (value > m_border)
        {
            ++m_g_count;
            if (! m_g_branch)
            {
                m_g_branch = m_al->allocate<LGTreeLeaf>(m_level + 1, m_border, true, m_al);
            }
            m_g_branch->put(value);
        }
        else // value < m_border
        {
            if (!m_l_branch)
            {
                m_l_branch = m_al->allocate<LGTreeLeaf>(m_level + 1, m_border, false, m_al);
            }
            m_l_branch->put(value);
        }
    }

private:
    int m_level;
    int m_border;
    //std::unique_ptr<LGTreeLeaf> m_l_branch;
    //std::unique_ptr<LGTreeLeaf> m_g_branch;
    LGTreeLeaf* m_l_branch = nullptr;
    LGTreeLeaf* m_g_branch = nullptr;
    int m_g_count = 0;
    int m_equal_count = 0;
    SmallObjectAllocator* m_al;
};

// Complete the insertionSort function below.
long long insertionSort(vector<int> arr) {
    long long sum = 0;

    SmallObjectAllocator al;
    LGTreeLeaf lg_tree(8, 0, true, &al);

    for (const auto i : arr)
    {
        sum += lg_tree.countGreaterAndPut(i);
//        lg_tree.put(i);
    }

    return sum;
}

// Complete the insertionSort function below.
int naiveInsertionSort(vector<int> arr) {
    int sum = 0;
    if (arr.empty())
        return sum;

    for (int i = 1; i < arr.size(); ++i)
    {
        for (int j = i; j > 0; --j)
        {
            if (arr[j - 1] <= arr[j])
            {
                break;
            }
            swap(arr[j], arr[j - 1]);
            ++sum;
        }
    }

    return sum;
}

int main()
{
    //ofstream fout(getenv("OUTPUT_PATH"));

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

        long long result = insertionSort(arr);

        std::cout << result << "\n";
    }

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
