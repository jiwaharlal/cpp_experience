#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <typeinfo>
#include <typeindex>

template <typename T>
class is_container
{
   typedef char true_type;
   struct false_type{ true_type _[2]; };
   template <typename U>
   static true_type has_iterator_checker(typename U::iterator *);
   template <typename U>
   static false_type has_iterator_checker(...);
public:
   enum { value = (sizeof(has_iterator_checker<T>(0)) == sizeof(true_type)) };
};

template <typename T>
class hasToString
{
   typedef char true_type;
   struct false_type{ true_type _[2]; };
   template <typename U>
   static false_type check(...);
   //enum { valueSize = std::type_index(typeid(T::size())) };
   //typedef decltype(T::size()) sizeType;
};

template <bool Cond, class T = void>
struct enable_if
{
   typedef T type;
};

template <class T>
struct enable_if<false, T>
{};

template <typename T>
typename enable_if<!is_container<T>::value>::type
   super_print(T const &t)
{
   std::cout << t << std::endl;
}

template <typename T>
typename enable_if<is_container<T>::value>::type
   super_print(T const &t)
{
   typedef typename T::value_type value_type;
   std::copy(t.begin(),
             t.end(),
             std::ostream_iterator<value_type>(std::cout, ", "));
   std::cout << std::endl;
}

int main()
{
   super_print(10);
   std::vector<int> b;

   std::cout << hasToString<std::vector<int> >::valueSize << std::endl;

   std::cout << typeid(b.size()).name() << std::endl;
   b.push_back(1);
   b.push_back(2);
   b.push_back(3);
   super_print(b);

   return 0;
}
