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

    std::for_each( ints.begin(), ints.end(), std::cout << _1 << " ");
    std::cout << std::endl;

    // std::remove_if( ints.begin(), ints.end(), _1 == 4 );
    std::for_each( ints.begin(), ints.end(), std::cout << _1 << " ");
    std::cout << std::endl;

    int i = 1;
    std::cout << (_1 = 2, _1 + boost::ref(i))(i) << std::endl;
    boost::function< int (int&) > func = ( _1 * 2 );

    std::cout << createMyltiplier( 5 )( 4 ) << std::endl;
}

int main( int argc, char** argv )
{
    using namespace boost::lambda;

    //sets();

    //return 0;

    (void) argc;
    (void) argv;

    std::vector< int > ints;
    ints.reserve( 20 );
    for ( int i = 0; i < 30; i++ )
    {
        ints.push_back( rnd(0, 100 ) );
    }


    std::cout << ints << std::endl;
    std::for_each( ints.begin(), ints.end(), std::cout << _1 << " " );
    std::cout << std::endl;
    //std::for_each(ints.begin(), ints.end(), boost::bind(&rnd, 5) );
    //std::find_if(ints.begin(), ints.end(), boost::bind(std::less<int>(), 5, _1));
    std::cout << "Min element: " <<  *std::min_element(ints.begin(), ints.end()) << std::endl;

    ints.erase( std::remove_if( ints.begin(), ints.end(), _1 < 50 ), ints.end() );
    // std::vector< int >::iterator newEnd = std::remove_if(ints.begin(), ints.end(), boost::bind(std::less<int>(), 50, _1));
    // ints.erase( newEnd, ints.end() );
    std::cout << ints << std::endl;

    //return 0;

    std::make_heap( ints.begin(), ints.end() );
    std::cout << ints << std::endl;

//    std::cout << "heap: " << ints << std::endl;
    for ( int i = 0; i < 10; i++ )
    {
        std::pop_heap( ints.begin(), ints.end() );
        ints.pop_back();
        std::cout << ints << std::endl;
    }

    sharedPtrs();

    return 0;
}
