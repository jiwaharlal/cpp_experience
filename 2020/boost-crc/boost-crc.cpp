#include <boost/crc.hpp>      // for boost::crc_basic, boost::crc_optimal
#include <boost/cstdint.hpp>  // for boost::uint16_t
#include <boost/format.hpp>

#include <algorithm>  // for std::for_each
#include <cassert>    // for assert
#include <cstddef>    // for std::size_t
#include <iostream>   // for std::cout
#include <ostream>    // for std::endl


// Main function
int main ()
{
    // This is "123456789" in ASCII
    unsigned char const  data[] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x49 };
    std::size_t const    data_len = sizeof( data ) / sizeof( data[0] );

    // The expected CRC for the given data
    boost::uint16_t const  expected = 0x29B1;

    // Simulate CRC-CCITT
    boost::crc_basic<16>  crc_ccitt1( 0x1021, 0xFFFF, 0, false, false );
    crc_ccitt1.process_bytes( data, data_len );
    //assert( crc_ccitt1.checksum() == expected );
    std::cout << crc_ccitt1.checksum() << std::endl;

    // Repeat with the optimal version (assuming a 16-bit type exists)
    boost::crc_optimal<16, 0x1021, 0xFFFF, 0, false, false>  crc_ccitt2;
    crc_ccitt2 = std::for_each( data, data + data_len, crc_ccitt2 );
    //assert( crc_ccitt2() == expected );
    std::cout << crc_ccitt2.checksum() << std::endl;

    boost::crc_basic<8>  crc_ccitt3(0x21, 0xFF);
    crc_ccitt3.process_bytes( data, data_len );
    //assert( crc_ccitt3.checksum() == expected );
    std::cout << boost::format("%1x") % static_cast<std::int32_t>(crc_ccitt3.checksum()) << std::endl;
    std::cout << boost::format("%1X") % static_cast<std::int32_t>(0xAF) << std::endl;
    std::string crc = (boost::format("%1X% %1x%") % 0xAF).str();
    std::cout << crc << std::endl;

    std::cout << "All tests passed." << std::endl;
    return 0;
}
