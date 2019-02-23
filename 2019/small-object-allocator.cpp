#include <iostream>
#include <vector>
#include <string>

class SmallObjectAllocator
{
public:
    SmallObjectAllocator()
    {
        m_chunks.emplace_back(m_chunk_size, 0);
    }

    template <typename T, typename ... ArgTypes>
    T* allocate(ArgTypes ... args)
    {
        //auto& last_chunk = m_chunks.back();
        if (sizeof(T) + m_chunk_pos >= m_chunk_size)
        {
            m_chunks.emplace_back(0, m_chunk_size);
            m_chunk_pos = 0;
        }

        auto obj = new (m_chunks.back().data() + m_chunk_pos) T(args...);
        m_chunk_pos += sizeof(T);

        return obj;
    }

    void print()
    {
        std::cout << *(reinterpret_cast<std::string*>(m_chunks.back().data())) << std::endl;
    }

private:
    std::vector<std::vector<char>> m_chunks;
    int m_chunk_size = 0x10000;
    size_t m_chunk_pos = 0u;
};

int main()
{
    SmallObjectAllocator al;
    auto s = al.allocate<std::string>("hello");

    std::cout << *s << std::endl;
    al.print();

    return 0;
}
