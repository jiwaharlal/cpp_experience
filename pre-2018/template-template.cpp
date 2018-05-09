#include <iostream>
#include <vector>

template <typename T>
struct Storage
{
};

template <template<class> class StorageType, typename StorableType>
void f(const StorageType<StorableType>& s)
{
    //std::cout << s.size() << std::endl;
    std::cout << sizeof(StorageType) << std::endl;
}

int main()
{
    //std::vector<int> v;
    //v.push_back(10);
    Storage<int> v;

    f(v);

    return 0;
}
