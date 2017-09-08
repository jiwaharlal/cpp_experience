#include <boost/phoenix/statement/sequence.hpp>
#include <boost/phoenix/core/reference.hpp>
#include <boost/phoenix/core/argument.hpp>
#include <boost/phoenix/operator/arithmetic.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

#define EXEC_METHOD(obj,  method) obj.method()

int main(int argc, char* argv[]) {
    unsigned long size = 0, trials = 0;

    double sum = 0.0;
    int factor = 1;

    std::vector<double> v(size);
    std::fill(v.begin(), v.end(), 1.0);

    for(unsigned long i = 0; i < trials; ++i) {
        using boost::phoenix::ref;
        using boost::phoenix::arg_names::_1;
        std::for_each(v.begin(), v.end(), ref(sum) += factor * _1);
    }

    return 0;
}
