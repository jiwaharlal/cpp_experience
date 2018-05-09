#include <iostream>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/range/algorithm.hpp>
#include <sstream>
#include <vector>

inline void parseCoordList(const std::string& data_str, std::vector<double>& data_vector)
{
    double val;
    //std::vector<double> data_vector;
    //data_vector.reserve(16);

    //std::stringstream ss(data_str);

    auto source = boost::iostreams::basic_array_source<std::string::const_iterator>(data_str.begin(), data_str.end());
    boost::iostreams::stream<decltype(source)> ss;
    ss.open(source);

    while (ss >> val)
    {
        data_vector.push_back(val);
        if (ss.peek() == ',')
        {
            ss.ignore();
        }
    }

    //return data_vector;
}

int main()
{
    std::vector<double> doubles;
    doubles.reserve(10);

    std::string data_str = "23.4, 54.6, 21.93";

    for (int i = 0; i < 1000000; i++)
    {
        parseCoordList(data_str, doubles);
    }

    std::cout << "parsed:\n";
    boost::copy(doubles, std::ostream_iterator<double>(std::cout, ", "));
    std::cout << std::endl;

    return 0;
}
