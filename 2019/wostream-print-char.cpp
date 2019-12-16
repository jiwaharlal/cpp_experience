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

    std::cout << sizeof(wc) << std::endl;

    return 0;
}
