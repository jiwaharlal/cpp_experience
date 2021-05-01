#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

struct void_tag {};

template<class Container>
class back_emplace_iterator : public std::iterator< std::output_iterator_tag,
                                                   void, void, void, void >
{
protected:
    Container* container;
public:
    typedef Container container_type;

    explicit back_emplace_iterator(Container& x) : container(&x) {}

    template<class T>
    back_emplace_iterator<Container>&
    operator=(T&& t)
    {
        container->emplace_back(std::forward<T>(t));
        return *this;
    }

    back_emplace_iterator<Container>&
    operator=(void_tag)
    {
        container->emplace_back();
        return *this;
    }

    back_emplace_iterator& operator*() { return *this; }
    back_emplace_iterator& operator++() { return *this; }
    back_emplace_iterator& operator++(int) { return *this; }
};

template< class Container >
inline back_emplace_iterator<Container>
back_emplacer( Container& c )
{
    return back_emplace_iterator<Container>(c);
}
struct A {
    A() { cout << "A::A()\n"; }
    ~A() { cout << "A::~A()\n"; }
    A(const A&) { cout << "A::A(const A&)\n"; }
    A& operator =(const A&) { cout << "operator =(const A&)\n"; return *this; }
    A(A&&) noexcept { cout << "A::A(A&&)\n"; }
    A& operator =(A&&) noexcept { cout << "operator =(A&&)\n"; return *this; }
};

A getA()
{
    return {};
}

vector<A> getAs(int count)
{
    vector<A> result;
    result.reserve(count);
    std::generate_n(back_emplacer(result), count, [](){ return void_tag(); });

    return result;
}

int main()
{
    {
        std::vector<A> as;

        //as.reserve(5);
        for (int i = 0; i != 5; ++i)
        {
            as.push_back(A());
            //as.emplace_back();
        }
        cout << "generation done\n";

        A a = getA();
    }
    std::fill_n(std::ostream_iterator<char>(std::cout), 20, '-');
    cout << endl;
    {
        auto as = getAs(3);
    }

    return 0;
}
