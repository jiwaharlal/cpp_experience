#include <type_traits>
#include <vector>

template <typename from_type, typename to_type, typename = void>
struct some_struct;

template <typename from_type, typename to_type>
struct some_struct <from_type, to_type, typename std::enable_if<
   std::is_same<from_type, to_type>::value>::type> 
{
    from_type val;
};

template <typename T, typename = void>
struct sfinae_make_signed;

template <typename T>
struct sfinae_make_signed<T, typename std::enable_if<std::is_integral<T>::value>::type>
{
    typedef typename std::make_signed<T>::type type;
};

template <typename from_type, typename to_type>
struct some_struct<from_type, to_type, typename std::enable_if<
   !std::is_same<from_type, to_type>::value && 
   std::is_same<typename sfinae_make_signed<from_type>::type, typename sfinae_make_signed<to_type>::type>::value>::type> 
{
    std::vector<from_type> v;
};

int main()
{
    //some_struct<double, long> a;
    //a.val = 2;

    some_struct<long, long> b;
    b.val = 2;

    some_struct<int, unsigned int> c;
    c.v.push_back(2);

    return 0;
}
