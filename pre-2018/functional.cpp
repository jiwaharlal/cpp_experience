#include <functional>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

struct A
{
    A(std::int32_t v) : val(v) {}
    ~A() { std::cout << "destructing A" << std::endl; }

    std::int32_t val;
};

std::function<std::int32_t(std::int32_t)> createQuadMapping()
{
    using IntToIntMap = std::unordered_map<std::int32_t, std::int32_t>;
    using MapSharedPtr = std::shared_ptr<IntToIntMap>;

    MapSharedPtr m{new IntToIntMap{{0, 0}, {1, 1}, {2, 4}, {3, 9}, {4, 16}, {5, 25}}};
    auto a = std::make_shared<A>(32);

    auto mapping = [m, a](std::int32_t val)
    {
        auto it = m->find(val);
        if (it == m->end())
        {
            return a->val;
        }
        return it->second;
    };

    return mapping;
}

void print()
{
    const auto mapping = createQuadMapping();

    std::cout << "Map 4: " << mapping(4) << std::endl;
}

int main()
{
    print();

    std::cout << "That's all" << std::endl;

    return 0;
}
