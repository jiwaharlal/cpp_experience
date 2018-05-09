#include <iostream>
#include <string>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/function.hpp>

using namespace boost::lambda;

int main( int argc, char** argv ) {
   (void) argc;
   (void) argv;

    std::cout << "argument count: " << argc << std::endl;
    do {
        if ( argc == 1 ) {
            std::cout << "breaking" << std::endl;
            break;
        }

        std::cout << "not braked" << std::endl;
    } while ( false );

    std::string someString;

    std::vector< int > ints;
    for ( int i = 0; i < 10; i++ )
    {
       ints.push_back( i );
    }

    boost::function< void(int) > printInt = (std::cout << _1 << " ");
    //std::for_each( ints.begin(), ints.end(), std::cout << _1 << " " )( 42 );
    std::for_each( ints.begin(), ints.end(), printInt )( 42 );
    std::cout << std::endl;

    return 0;
}
