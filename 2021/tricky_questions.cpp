#include <iostream>
#include <vector>
#include <type_traits>

using namespace std;

void typesHierarchy()
{
    cout << sizeof(int) << ' ' << sizeof(long) << ' ' << sizeof(long long) << endl;
    cout << 25u - 50l << endl;

    long long i = -50;
    unsigned long j = 26u;

    cout << (i < j) << endl;
}

void iteratorIncrement()
{
    vector vec{1, 2, 3, 4, 5, 6};
    for (auto itr = vec.begin(); itr != vec.end(); itr++) {
        cout << *itr << ' ';
    }
    cout << '\n';
}


template <typename T, typename U>
constexpr bool isDerivedFrom()
{
    struct s
    {
        static constexpr bool isDerived(T*) { return true; }
        static constexpr bool isDerived( ... ) { return false; }
    };

    return s::isDerived(static_cast<U*>(nullptr));
}

class A {};
class B : public A {};

template<typename D, typename B>
class IsDerivedFromHelper
{
    class No { };
    class Yes { No no[3]; };

    static Yes Test( B* );
    static No Test( ... );
public:
    enum { Is = sizeof(Test(static_cast<D*>(0))) == sizeof(Yes) };

};


template <class C, class P>
constexpr bool IsDerivedFrom() {
    return IsDerivedFromHelper<C, P>::Is;
}

template <typename T, typename U>
void testIsDerived()
{
    if constexpr (IsDerivedFrom<T, U>())
        cout << "Yes\n";
    else
        cout << "No\n";
}

class Base {
    virtual void method(std::string s) {std::cout << "from Base " << s << std::endl;}
public:
    Base() {method("constructor");}
    virtual ~Base() {method("destructor");}
    void baseMethod() {method("baseMethod");}
};

class AA : public Base {
    void method(string s) {std::cout << "from AA " << s << std::endl;}
public:
    AA() {method("constructor");}
    ~AA() {method("destructor");}
};

int testInheritance(void) {
    Base* base = new AA;
    base->baseMethod();
    delete base;
    return 0;
}

//template <typename T>
//void only_int(T, T);

//template <>
//void only_int<int>(int, int)
//{}

//template <>
//void only_int(long, long)
//{}

//void only_int(int, int) {}

//template <typename T, typename U> void only_int(T, U) = delete;

template <typename T>
typename std::enable_if<std::is_same<T, int>::value, void>::type only_int(T, T) {}

int main()
{
    typesHierarchy();
    iteratorIncrement();
    testIsDerived<A, B>();
    testIsDerived<B, A>();

    only_int(3, 4);
    //only_int(3l, 4l);

    testInheritance();

    return 0;
}
