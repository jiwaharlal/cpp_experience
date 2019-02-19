#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

int diagonalDifference(vector<vector<int>> arr) {
  std::int32_t width = arr.size();
  std::int32_t diff = 0;

  for (std::int32_t i = 0; i < width; ++i) {
    // std::cout << "on prim: " << arr[i][i] << " on sec: " << arr[i][width -
    // i];
    std::int32_t add = arr[i][i] - arr[width - i - 1][i];
    // std::cout << "add: " << add << '\n';
    diff += add;
  }

  return std::abs(diff);
}

int main()
{
    std::vector<std::vector<int>> v = {{11, 2, 4}, {4, 5, 6}, {10, 8, -12}};
    auto diff = diagonalDifference(v);

    std::cout << diff << std::endl;

    return 0;
}
