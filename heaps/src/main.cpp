// #include "BinaryHeap.h"

#define BOOST_BIND_NO_PLACEHOLDERS

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <functional>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm.hpp>
#include <cstdio>
#include <boost/function.hpp>
#include <boost/lambda/control_structures.hpp>

using namespace boost::lambda;

int rnd( int min, int max )
{
    int diff = max - min;
    return std::rand() % diff + min;
}

template< class T >
bool less(const T& left, const T& right)
{
    return left < right;
}

template < class T >
inline std::ostream& operator <<( std::ostream& out, const std::vector< T >& v )
{
    std::copy( v.begin(), v.end(), std::ostream_iterator< T >( out, " " ) );

    return out;
}

template <class T>
inline std::ostream& operator <<( std::ostream& out, const boost::shared_ptr< T >& v )
{
    out << *v;
    return out;
}

template <class T, class PtrT>
inline T& deref(PtrT ptr)
{
    return *ptr;
}

template <class T>
inline std::ostream& operator <<( std::ostream& out, const std::vector< boost::shared_ptr< T > >& v )
{
    //std::transform( v.begin(), v.end(), std::ostream_iterator< T >( out, " " ), &deref<T, boost::shared_ptr<T> > );
    std::transform( v.begin(), v.end(), std::ostream_iterator< T >( out, " " ), *_1 );
    return out;
}

void sharedPtrs()
{
    std::vector< boost::shared_ptr< int > > ints;
    for ( int i = 0; i < 10; i++ )
    {
        ints.push_back( boost::shared_ptr< int >( new int( i ) ) );
    }

    // std::remove_if(ints.begin(), ints.end(), boost::bind

    std::cout << ints << std::endl;
}

template <class T>
boost::function<T (const T&) > createMyltiplier( const T& val )
{
    return (_1 * val);
}

bool isEven(int i)
{
   return i % 2 == 0;
}

void sets()
{
    std::set< int > ints;

    for ( int i = 0; i < 10; i++ )
    {
        ints.insert( i );
    }

    std::for_each(ints.begin(), ints.end(), std::cout << _1 << " ");
    std::cout << std::endl;

    // std::remove_if( ints.begin(), ints.end(), _1 == 4 );
    std::for_each( ints.begin(), ints.end(), std::cout << _1 << " ");
    std::cout << std::endl;

    int i = 1;
    std::cout << (_1 = 2, _1 + boost::ref(i))(i) << std::endl;
    boost::function<int(int&)> func = (_1 * 2);

    std::cout << createMyltiplier( 5 )( 4 ) << std::endl;
}

int main()
{
    std::vector<int> ints(20);
    boost::generate(ints, boost::bind(&rnd, 0, 100));

    std::cout << ints << std::endl;
    std::cout << "Min element: " << *boost::min_element(ints) << std::endl;

    boost::make_heap(ints);

    std::cout << "heap:\n" << ints << std::endl;

    for (int i = 0; i < 10; i++)
    {
        boost::pop_heap(ints);
        std::cout << "Popped element: " << ints.back() << std::endl;
        ints.pop_back();
        std::cout << ints << std::endl;
    }

    return 0;
}
