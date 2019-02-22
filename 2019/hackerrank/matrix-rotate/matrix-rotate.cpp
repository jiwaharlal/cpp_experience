#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class CRing
{
public:
    explicit CRing(std::vector<std::vector<int>>& matrix, int idx)
        : m_matrix(matrix)
    {
        int row = idx;
        int col = idx;
        for (; col < static_cast<int>(matrix[0].size()) - idx - 1; ++col)
        {
            m_chain.emplace_back(row, col);
        }
        for (; row < static_cast<int>(matrix.size()) - idx - 1; ++row)
        {
            m_chain.emplace_back(row, col);
        }
        for (; col > idx; --col)
        {
            m_chain.emplace_back(row, col);
        }
        for (; row > idx; --row)
        {
            m_chain.emplace_back(row, col);
        }
    }

    void rotateCW(int steps)
    {
        auto size_signed = static_cast<int>(m_chain.size());
        steps %= size_signed;
        std::vector<int> values;
        values.reserve(m_chain.size());
        for (const auto& link : m_chain)
        {
            values.push_back(m_matrix[link.first][link.second]);
        }
        for (int i = 0; i < static_cast<int>(m_chain.size()); ++i)
        {
            int moved_pos = (i + steps + size_signed) % size_signed;
            auto link = m_chain[moved_pos];
            m_matrix[link.first][link.second] = values[i];
        }
    }

private:
    std::vector<std::vector<int>>& m_matrix;
    std::vector<std::pair<int, int>> m_chain;
};


string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

// Complete the matrixRotation function below.
void matrixRotation(vector<vector<int>> matrix, int r) {
    auto ring_count = std::min(matrix.size(), matrix[0].size()) / 2;
    std::vector<CRing> rings;
    rings.reserve(ring_count);
    for (size_t i = 0; i < ring_count; ++i)
    {
        rings.emplace_back(matrix, i);
    }

    for (auto& ring : rings)
    {
        ring.rotateCW(-r);
    }

    for (std::size_t i = 0u; i < matrix.size(); ++i)
    {
        for (size_t j = 0u; j < matrix[i].size(); ++j)
        {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << '\n';
    }
}

int main()
{
    string mnr_temp;
    getline(cin, mnr_temp);

    vector<string> mnr = split(rtrim(mnr_temp));

    int m = stoi(mnr[0]);

    int n = stoi(mnr[1]);

    int r = stoi(mnr[2]);

    vector<vector<int>> matrix(m);

    for (int i = 0; i < m; i++) {
        matrix[i].resize(n);

        string matrix_row_temp_temp;
        getline(cin, matrix_row_temp_temp);

        vector<string> matrix_row_temp = split(rtrim(matrix_row_temp_temp));

        for (int j = 0; j < n; j++) {
            int matrix_row_item = stoi(matrix_row_temp[j]);

            matrix[i][j] = matrix_row_item;
        }
    }

    matrixRotation(matrix, r);

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
