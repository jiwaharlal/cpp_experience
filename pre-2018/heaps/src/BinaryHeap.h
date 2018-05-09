#ifndef __BINARY_HEAP_H__
#define __BINARY_HEAP_H__

#include <vector>

template < class ElementType, class ComparatorType >
class BinaryHeap
{
public:
    explicit BinaryHeap( const ComparatorType& comparator );

    void push( const ElementType& value );
    ElementType pop();

private:
    ComparatorType m_comparator;
    std::vector< ElementType > m_data;
};

template < class ElementType, class ComparatorType >
void BinaryHeap< ElementType, ComparatorType >::push( const ElementType& value )
{
    m_data.push_back( value );

}

#endif // __BINARY_HEAP_H__
