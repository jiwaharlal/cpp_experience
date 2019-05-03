#include <iostream>
#include <functional>

struct Note
{
    std::string title;
};

int main()
{
    Note n{"hello"};

    std::function<const std::string&(const Note*)> f = &Note::title;
    auto accessor = &Note::title;

    std::cout << f(&n) << std::endl;
    std::cout << accessor(n) << std::endl;

    return 0;
}
