// requirenments
// write magic.hpp
// result is not dependent on function names
// any code in RUN_CODE should run before code in main
// main should not be changed or relinked in any way
// expected result (g++):
// hello:foo
// hello:bar
// main

#include "magic.hpp"

#include <iostream>
using namespace std;

//#define CONCAT_IMPL( x, y ) x##y
//#define MACRO_CONCAT( x, y ) CONCAT_IMPL( x, y )

//#define RUN_CODE(code) \
    //} \
    //static int MACRO_CONCAT(x, __COUNTER__) = []{ code; return 0; }(); \
    //void MACRO_CONCAT(fun, __COUNTER__) () {

void foolishfrl() {
	cout << "begin: " << "foo" << endl;

	RUN_CODE(cout << "hello:" << "foo" << endl);

	cout << "end: " << "foo" << endl;
}

void barjhfsldls() {
    cout << "begin: " << "bar" << endl;

    RUN_CODE(cout << "hello:" << "bar" << endl);

    cout << "end: " << "bar" << endl;
}


int main(int argc, char const *argv[])
{
	cout << "main" << endl;
	cin.get();
	return 0;
}
