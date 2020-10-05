#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

// Complete the surfaceArea function below.
int surfaceArea(vector<vector<int>> A) {
    int sum = std::accumulate(A[0].begin(), A[0].end(), 0);
    sum += std::accumulate(A.back().begin(), A.back().end(), 0);

    for (int row = 0; row < static_cast<int>(A.size()); ++row)
    {
        sum += A[row].front() + A[row].back();
    }

    for (int row = 0; row < static_cast<int>(A.size()) - 1; ++row)
    {
        for (int col = 0, limit = static_cast<int>(A[row].size()) - 1; col < limit; ++col)
        {
            sum += std::abs(A[row][col] - A[row + 1][col]);
            sum += std::abs(A[row][col] - A[row][col + 1]);
        }

        sum += std::abs(A[row].back() - A[row + 1].back());
    }

    for (int col = 0, limit = static_cast<int>(A[0].size()) - 1; col < limit; ++col)
    {
        sum += std::abs(A.back()[col] - A.back()[col + 1]);
    }

    sum += A.size() * A.front().size() * 2;

    return sum;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string HW_temp;
    getline(cin, HW_temp);

    vector<string> HW = split_string(HW_temp);

    int H = stoi(HW[0]);

    int W = stoi(HW[1]);

    vector<vector<int>> A(H);
    for (int i = 0; i < H; i++) {
        A[i].resize(W);

        for (int j = 0; j < W; j++) {
            cin >> A[i][j];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int result = surfaceArea(A);

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
