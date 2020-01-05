#include <iostream>

int main()
{
    wchar_t wc = L'w';
    char c = 'c';
    std::uint8_t u8 = 55;
    std::uint16_t u16 = 55;
    std::wstring ws = L"Hello";

    std::cout << "ostream char: " << c << " wchar: " << wc << " uint8: " << u8 << " uint16: " << u16 << std::endl;
    std::wcout << "wostream char: " << c << " wchar: " << wc << " uint8: " << u8 << " uint16: " << u16 << std::endl;

    std::cout << std::to_string(u8) << " " << std::to_string(wc) << std::endl;
    std::cout << "sizeof(wc) " << sizeof(wc) << std::endl;
    std::wcout << std::string("Hello") << std::endl;

    return 0;
}
