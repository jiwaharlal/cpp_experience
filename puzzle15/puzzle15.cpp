#include <array>
#include <cinttypes>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <vector>

#include <boost/assign.hpp>
#include <boost/bimap.hpp>
#include <boost/graph/astar_search.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <boost/tuple/tuple_io.hpp>

// [row][column]
using CellGrid = std::array<std::array<std::int32_t, 4>, 4>;

struct Move
{
    bool isLegal() const
    {
        return std::labs(d_row) + std::labs(d_col) == 1;
    }

    bool operator <(const Move& other) const
    {
        return std::tie(d_row, d_col) < std::tie(other.d_row, other.d_col);
    }

    std::int32_t d_row;
    std::int32_t d_col;
};

struct Position
{
    std::int32_t row;
    std::int32_t col;
};

Position& operator +=(Position& lhs, const Move& rhs)
{
    lhs.col += rhs.d_col;
    lhs.row += rhs.d_row;
    return lhs;
}

Position operator +(const Position& lhs, const Move& rhs)
{
    Position result(lhs);
    result += rhs;

    return result;
}

Move operator -(const Position& lhs, const Position& rhs)
{
    return Move{lhs.row - rhs.row, lhs.col - rhs.col};
}

std::ostream& operator<< (std::ostream& out, const Move& m)
{
    return out << boost::tie(m.d_row, m.d_col);
}

// Defines options to move a chip
enum class EMove
{
    Up = 0x1,
    Down = 0x2,
    Left = 0x4,
    Right = 0x8
};

using MovesMap = boost::bimap<EMove, Move>;
static const MovesMap moves_map = boost::assign::list_of<MovesMap::relation>
    (EMove::Up, Move{1, 0})
    (EMove::Down, Move{-1, 0})
    (EMove::Left, Move{0, 1})
    (EMove::Right, Move{0, -1});

static const auto& move_enum_to_struct = moves_map.left;
static const auto& move_struct_to_enum = moves_map.right;

struct Field;
std::ostream& operator <<(std::ostream&, const Field&);

struct Field
{
    bool operator !=(const Field& other) const
    {
        return !operator ==(other);
    }

    bool operator ==(const Field& other) const
    {
        //std::cout << "Equal\n" << other;

        return boost::equal(cells, other.cells);
    }

    bool operator <(const Field& other) const
    {
        for (auto i = 0u; i < cells.size(); i++)
        {
            for (auto j = 0u; j < cells.size(); j++)
            {
                if (cells[i][j] < other.cells[i][j])
                {
                    return true;
                }
                else if (other.cells[i][j] < cells[i][j])
                {
                    return false;
                }
            }
        }

        return false;
    }

    CellGrid::value_type::reference operator[](const Position& p)
    {
        return cells[p.row][p.col];
    }

    CellGrid::value_type::const_reference operator[](const Position& p) const
    {
        return cells[p.row][p.col];
    }

    CellGrid cells;
    Position empty_cell;
};

std::ostream& operator <<(std::ostream& out, const EMove& m)
{
    switch (m)
    {
        case EMove::Up:
            return out << "up";
        case EMove::Down:
            return out << "down";
        case EMove::Left:
            return out << "left";
        case EMove::Right:
            return out << "right";
        default:
            throw std::runtime_error("Unknown move: ");
    }
}

bool isEndState(const Field& field)
{
    int counter = 1;
    for (auto i = 0u; i < field.cells.size(); i++)
    {
        const auto& row = field.cells[i];
        auto row_len = i == 3 ? 3 : 4;
        for (auto j = 0; j < row_len; j++, counter++)
        {
            if (counter != row[j])
            {
                return false;
            }
        }
    }

    return field.cells[field.empty_cell.row][field.empty_cell.col] == 0;
}

Field end_state = { {{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 0}}}, {3, 3} };

Field readField(std::istream& in)
{
    Field result;
    for (std::size_t i = 0u; i < 4; i++)
    {
        for (std::size_t j = 0u; j < 4; j++)
        {
            int32_t value;
            in >> value;
            result.cells[i][j] = value;
            if (value == 0)
            {
                result.empty_cell = {i, j};
            }
        }
    }

    return result;
}

std::ostream& operator <<(std::ostream& out, const Field& f)
{
    for (const auto& row : f.cells)
    {
        for (const auto& cell : row)
        {
            out << std::setw(3) << static_cast<std::int32_t>(cell);
        }
        out << "\n";
    }
    std::cout << "empty_cell: " << boost::tie(f.empty_cell.row, f.empty_cell.col) << std::endl;

    return out;
}

std::set<EMove> getAvailableMoves(const Field& field)
{
    std::set<EMove> moves;
    if (field.empty_cell.row != 0)
    {
        moves.insert(EMove::Down);
    }
    if (field.empty_cell.col != 0)
    {
        moves.insert(EMove::Right);
    }
    if (field.empty_cell.row != field.cells.size() - 1)
    {
        moves.insert(EMove::Up);
    }
    if (field.empty_cell.col != field.cells.size() - 1)
    {
        moves.insert(EMove::Left);
    }

    return moves;
}

void applyMoveInPlace(Field& field, EMove move)
{
    const auto& move_struct = move_enum_to_struct.at(move);
    const auto new_empty_cell = field.empty_cell + move_struct;
    std::swap(field[field.empty_cell], field[new_empty_cell]);
    field.empty_cell = new_empty_cell;
}

Field applyMove(const Field& field, EMove move)
{
    auto result = field;
    applyMoveInPlace(result, move);
    return result;
}

struct FoundGoal {};

std::vector<Field> getOneStepNeighbours(const Field& field)
{
    auto moves = getAvailableMoves(field);
    std::vector<Field> result(moves.size(), field);
    boost::for_each(result, moves, &applyMoveInPlace);

    return result;
}

// visitor that terminates when we find the goal
template <class Vertex>
class AstarGoalVisitor : public boost::default_astar_visitor
{
public:
    template <class Graph>
    void examine_vertex(Vertex u, Graph&)
    {
        if (isEndState(static_cast<Vertex>(u)))
        {
            std::cout << "Goal found" << std::endl;
            throw FoundGoal();
        }
    }
};

int distanceToGoal(const Field& field)
{
    std::int32_t sum = 0;
    std::int32_t side_size = field.cells.size();
    for (std::int32_t i = 0; i < side_size; i++)
    {
        for (std::int32_t j = 0; j < side_size; j++)
        {
            std::int32_t cell = field.cells[i][j];
            if (cell == 0)
            {
                continue;
            }
            cell--;
            sum += std::abs(cell / side_size - i) + std::abs(cell % side_size - j);
        }
    }

    return sum;
}

using Edge = std::pair<Field, EMove>;
using EdgeSet = std::vector<Edge>;
using EdgeSetIteratorPtr = std::shared_ptr<EdgeSet::iterator>;

class OutEdgeIterator
{
public:
    OutEdgeIterator() {};

    OutEdgeIterator(EdgeSetIteratorPtr iterator_ptr) : m_iterator(iterator_ptr) {}

    OutEdgeIterator& operator ++()
    {
        m_iterator->operator ++();

        return *this;
    }

    OutEdgeIterator operator ++(int)
    {
        OutEdgeIterator copy{*this};
        m_iterator->operator ++();

        return copy;
    }

    bool operator !=(const OutEdgeIterator& other) const
    {
        return *m_iterator != *(other.m_iterator);
    }

    bool operator ==(const OutEdgeIterator& other) const
    {
        return *m_iterator == *(other.m_iterator);
    }

    const Edge& operator *() const
    {
        return **m_iterator;
    }

private:
    EdgeSetIteratorPtr m_iterator;
};

namespace std
{
    template <>
    struct iterator_traits<OutEdgeIterator> : public iterator_traits<EdgeSet::iterator>
    {
    };
}

struct PuzzleStateSpace
{
    typedef Field vertex_descriptor;
    typedef Edge edge_descriptor;
    typedef boost::undirected_tag directed_category;
    typedef boost::disallow_parallel_edge_tag edge_parallel_category;
    typedef boost::incidence_graph_tag traversal_category;

    typedef boost::no_property edge_property_type;
    typedef boost::no_property vertex_property_type;

    typedef OutEdgeIterator out_edge_iterator;
    typedef std::size_t degree_size_type;
};

namespace boost
{
    template <> struct graph_traits<PuzzleStateSpace>
    {
        typedef PuzzleStateSpace G;

        typedef G::vertex_descriptor      vertex_descriptor;
        typedef G::edge_descriptor        edge_descriptor;
        typedef G::out_edge_iterator      out_edge_iterator;

        typedef G::directed_category      directed_category;
        typedef G::edge_parallel_category edge_parallel_category;
        typedef G::traversal_category     traversal_category;

        typedef G::degree_size_type       degree_size_type;

        typedef void in_edge_iterator;
        typedef void vertex_iterator;
        typedef void vertices_size_type;
        typedef void edge_iterator;
        typedef void edges_size_type;
    };
}

struct AlwaysOneMap
{
    typedef int value_type ;
    typedef Field key_type;
    typedef int& reference;
    typedef boost::readable_property_map_tag category;

    template <typename T>
    int operator[](const T& e) const
    {
        return 1;
    }
};

PuzzleStateSpace::vertex_descriptor source(const PuzzleStateSpace::edge_descriptor& e, const PuzzleStateSpace&)
{
    return e.first;
}

PuzzleStateSpace::vertex_descriptor target(const PuzzleStateSpace::edge_descriptor& e, const PuzzleStateSpace&)
{
    return applyMove(e.first, e.second);
}

PuzzleStateSpace::degree_size_type out_degree(const PuzzleStateSpace::vertex_descriptor& v, const PuzzleStateSpace&)
{
    return getAvailableMoves(v).size();
}

std::pair<OutEdgeIterator, OutEdgeIterator>
out_edges(const PuzzleStateSpace::vertex_descriptor& v, const PuzzleStateSpace&)
{
    auto edge_set = std::make_shared<EdgeSet>();
    for (const auto& move : getAvailableMoves(v))
    {
        edge_set->emplace_back(v, move);
    }

    return std::make_pair(
            OutEdgeIterator(EdgeSetIteratorPtr(edge_set, new EdgeSet::iterator(edge_set->begin()))),
            OutEdgeIterator(EdgeSetIteratorPtr(edge_set, new EdgeSet::iterator(edge_set->end()))));
}

template <typename K, typename V>
class default_map
{
public:
    typedef K key_type;
    typedef V data_type;
    typedef std::pair<K,V> value_type;

    default_map(V const& defaultValue)
        : m()
        , defaultValue(defaultValue)
    {}

    V & operator[](K const& k)
    {
        if (m.find(k) == m.end())
        {
            m[k] = defaultValue;
        }
        return m[k];
    }

//private:
    std::map<K,V> m;
    V const defaultValue;
};

//template <typename K, typename V>
//class debug_map
//{
//public:
    //typedef K key_type;
    //typedef V data_type;
    //typedef std::pair<K,V> value_type;

    //V & operator[](K const& k)
    //{
        //std::cout << "Accessing\n" << k << "found: " << m.count(k) << " dist to goal: " << distanceToGoal(k) << "\n\n";

        //return m[k];
    //}

    //const V& at(const K& k)
    //{
        //return m.at(k);
    //}

    //std::map<K,V> m;
//};

template <typename MapType>
typename MapType::value_type get(const MapType& map, const PuzzleStateSpace::edge_descriptor& desc)
{
    return map[desc];
}

EMove restoreMove(const Field& src, const Field& dst)
{
    auto move = dst.empty_cell - src.empty_cell;
    try
    {
        auto enum_move = move_struct_to_enum.at(move);

        if (!move.isLegal() || !(applyMove(src, enum_move) == dst))
        {
            std::cout << "Illegal move found\nSrc field:\n" << src << "Dst field\n" << dst
                << "Move: " << enum_move << std::endl;

            throw std::runtime_error("Illegal move");
        }

        return enum_move;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error while restoring move: " << move << " : " << e.what() << std::endl;
        throw;
    }
}

int main(int argc, char** argv)
{
    auto state = end_state;

    if (argc == 2)
    {
        std::ifstream fin(argv[1]);
        state = readField(fin);
    }

    std::cout << state;
    std::cout << std::boolalpha << isEndState(state) << std::endl;

    for (const auto m : { EMove::Down, EMove::Right, EMove::Down, EMove::Left })
    {
        auto prev_state = state;
        std::cout << "Applying move: " << m << std::endl;
        applyMoveInPlace(state, m);
        std::cout << state << "distance to goal: " << distanceToGoal(state) << std::endl;
    }

    auto initial_state = state;

    using PredecessorMap = std::map<Field, Field>;
    PredecessorMap pred_map;
    boost::associative_property_map<PredecessorMap> pred_property_map(pred_map);

    std::map<Field, unsigned> vertex_index_map;
    std::map<Field, unsigned> rank_map;
    std::map<Field, boost::default_color_type> color_map;

    typedef boost::associative_property_map<default_map<Field, unsigned>> DistanceMap;
    typedef default_map<Field, unsigned> WrappedDistanceMap;
    WrappedDistanceMap wrappedMap = WrappedDistanceMap(200);
    DistanceMap d = DistanceMap(wrappedMap);
    wrappedMap[initial_state] = 0;

    try
    {
        boost::astar_search_no_init(
                PuzzleStateSpace(),
                initial_state,
                &distanceToGoal,
                boost::predecessor_map(pred_property_map)
                    .visitor(AstarGoalVisitor<Field>())
                    .weight_map(AlwaysOneMap())
                    .vertex_index_map(boost::associative_property_map<std::map<Field, unsigned>>(vertex_index_map))
                    .rank_map(boost::associative_property_map<std::map<Field, unsigned>>(rank_map))
                    .color_map(boost::associative_property_map<std::map<Field, boost::default_color_type>>(color_map))
                    .distance_map(d)
                    //.distance_compare(std::less<unsigned>())
                    //.distance_combine(std::plus<unsigned>())
                    );
    }
    catch (const FoundGoal& g)
    {
        std::cout << "\nRestoring moves\n";
        std::list<EMove> moves;

        for (auto state = end_state; state != initial_state; )
        {
            const auto& prev_state = pred_map.at(state);
            auto m = restoreMove(prev_state, state);
            moves.push_front(m);
            state = prev_state;
        }

        std::cout << "Moves: ";
        boost::copy(moves, std::ostream_iterator<EMove>(std::cout, ", "));
        std::cout << std::endl;
    }

    return 0;
}
