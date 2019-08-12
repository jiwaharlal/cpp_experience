#include <iostream>

//enum class ConsoleColor
//{
    //Red,
    //Black,
    //Gray,
    //Green
//};

//void SetConsoleColor(std::basic_ostream<char>& out, ConsoleColor color)
//{
    //switch (color)
      //{
      //case ConsoleColor::Red:
        //out << "\033[1;31m";
        //break;
      //case ConsoleColor::Green:
        //out << "\033[1;32m";
        //break;
      //case ConsoleColor::Gray:
        //out << "\033[1;38m";
        //break;
      //case ConsoleColor::Black:
      //default:
      //case ConsoleColor::Gray:
        //out << "\033[1;38m";
      //}
//}

enum class ConsoleColor
{
    Red = 9,
    Green = 2, 
    Gray = 8,
    Normal = 0
};

template <typename CharType>
std::basic_ostream<CharType>& operator <<(std::basic_ostream<CharType>& out, ConsoleColor c)
{
    if (c == ConsoleColor::Normal)
    {
        return out << "\033[0m";
    }

    return out << "\033[38;5;" << static_cast<int>(c) << 'm';
}

int main()
{
    std::cout << "\033[1;31mbold red text\033[0m\n";

    std::cout << "\033[38;5;9m";
    std::cout << "Should be red\n";

    std::cout << "\033[0m";
    std::cout << "Should be normal again\n";

    std::cout << ConsoleColor::Gray << "Should be gray\n" << ConsoleColor::Normal;

    std::wcout << ConsoleColor::Green << L"Hello!\n";

    return 0;
}
