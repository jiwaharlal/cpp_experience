#include <boost/lexical_cast.hpp>
#include <iostream>

enum EDirection
{
    Forward,
    Backward,
    Left,
    Right
};

std::string toText(const EDirection& dir)
{
    switch (dir)
    {
        case Forward:
            return "Forward";
        case Backward:
            return "Backward";
        case Left:
            return "Left";
        case Right:
            return "Right";
        default:
            throw std::runtime_error("Incorrect EDirection value");
    }
}

struct EnumAsText
{
    explicit EnumAsText(bool value) : is_as_text(value) {}

    std::ostream& operator ()(std::ostream& out) const
    {
        out.iword(flag_index) = is_as_text;

        return out;
    }

    bool is_as_text;
    const static int flag_index;
};

const int EnumAsText::flag_index = std::ios_base::xalloc();

std::ostream& operator <<(std::ostream& out, const EnumAsText& manipulator)
{
    return manipulator(out);
}

std::ostream& operator <<(std::ostream& out, const EDirection dir)
{
    if (out.iword(EnumAsText::flag_index))
    {
        return out << toText(dir);
    }

    return out << static_cast<int>(dir);
}

int main()
{
    std::cout << "in main" << std::endl;
    std::cout << Backward << " = " << EnumAsText(true) << Backward
        << " = " << EnumAsText(false) << Backward << std::endl;

    return 0;
}
