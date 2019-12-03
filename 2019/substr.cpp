#include <algorithm>
#include <string>
#include <iostream>

class Str
{
public:
    using size_type = std::string::size_type;

    Str(std::string data)
        : m_data(std::move(data))
    {}

    Str substr(size_type pos = 0, size_type count = std::string::npos) const &
    {
        return Str{m_data.substr(pos, count)};
    }

    Str substr(size_type pos = 0, size_type count = std::string::npos) &&
    {
        m_data.erase(m_data.begin() + pos + count, m_data.end());
        m_data.erase(m_data.begin(), m_data.begin() + pos);
        return Str(std::move(m_data));
    }

    std::string m_data;
};

std::ostream& operator<<(std::ostream& out, const Str& s)
{
    return out << s.m_data;
}

int main()
{
    Str s1{"hello there guys"};

    auto s2 = s1.substr(3, 5);
    auto s3 = std::move(s1).substr(3, 5);

    std::cout << s1 << '\n' << s2 << '\n' << s3 << std::endl;

    return 0;
}
