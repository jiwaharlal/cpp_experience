#include <cstdint>
#include <iostream>

#include <boost/multi_array.hpp>
#include <boost/range/adaptors.hpp>

int main()
{
    boost::multi_array<std::int32_t, 2> mat{boost::extents[3][4]};
    //std::cout << "mat: " << boost::adaptors::format(mat) << std::endl;

    //for (const auto& row : mat)
    //{
        //for (const auto& i : row)
        //{
            //std::cout << i << '\n';
        //}
    //}

    std::cout << mat.size() << '\n';

    return 0;
}
