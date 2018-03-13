#include <array>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <vector>

//#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/astar_search.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>

// [row][column]
using CellGrid = std::array<std::array<std::int32_t, 4>, 4>;

struct Position
{
    std::size_t row;
    std::size_t col;
};

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
        std::cout << "Equal\n" << other;

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

    CellGrid cells;
    Position empty_cell;
};

// Defines options to move a chip
enum class EMove
{
    Up = 0x1,
    Down = 0x2,
    Left = 0x4,
    Right = 0x8
};

EMove revertMove(EMove move)
{
    switch (move)
    {
        case EMove::Up:
            return EMove::Down;
        case EMove::Down:
            return EMove::Up;
        case EMove::Left:
            return EMove::Right;
        case EMove::Right:
            return EMove::Left;
        default:
            throw std::runtime_error("Unknown move: ");
    }
}

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

    //std::cout << "For field\n" << field << "available moves: ";
    //boost::copy(moves, std::ostream_iterator<EMove>(std::cout, ", "));
    //std::cout << std::endl;

    return moves;
}

void applyMoveInPlace(Field& field, EMove move)
{
    auto& empty_cell = field.empty_cell;
    auto& cells = field.cells;
    auto& row = cells[empty_cell.row];
    const auto& col_index = empty_cell.col;
    switch (move)
    {
        case EMove::Left:
            std::swap(row[empty_cell.col], row[empty_cell.col + 1]);
            field.empty_cell.col++;
            return;
        case EMove::Right:
            std::swap(row[empty_cell.col], row[empty_cell.col - 1]);
            field.empty_cell.col--;
            return;
        case EMove::Up:
            std::swap(cells[empty_cell.row][col_index], cells[empty_cell.row + 1][col_index]);
            field.empty_cell.row++;
            return;
        case EMove::Down:
            std::swap(cells[empty_cell.row][col_index], cells[empty_cell.row - 1][col_index]);
            field.empty_cell.row--;
            return;
        default:
            throw std::runtime_error("Unknown move: ");
    };
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

struct vertex_puz_state_t
{
    typedef boost::vertex_property_tag kind;
};

typedef boost::property<boost::vertex_color_t, boost::default_color_type,
    boost::property<boost::vertex_rank_t, unsigned int,
    boost::property<boost::vertex_distance_t, unsigned int,
    boost::property<boost::vertex_predecessor_t, unsigned int,
    boost::property<vertex_puz_state_t, Field> > > > > vert_prop;

typedef boost::property<boost::edge_weight_t, unsigned int> edge_prop;

//typedef adjacency_list<listS, vecS, undirectedS, vert_prop, edge_prop> mygraph_t;

//typedef mygraph_t::vertex_descriptor vertex_t;
//typedef mygraph_t::vertex_iterator vertex_iterator_t;
//typedef property_map<mygraph_t, vertex_puz_state_t>::type StateMap;
//typedef property_map<mygraph_t, edge_weight_t>::type WeightMap;
//typedef property_map<mygraph_t, vertex_predecessor_t>::type PredMap;

//using PuzzleGraph = boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS>;

// visitor that terminates when we find the goal
template <class Vertex>
class AstarGoalVisitor : public boost::default_astar_visitor
{
public:
    template <class Graph>
    void examine_vertex(Vertex u, Graph&)
    {
        //static int counter = 0;
        //counter++;
        //if (counter == 6)
        //{
            //throw FoundGoal();
        //}
        //std::cout << "Examining:\n" << u << std::endl;

        if (isEndState(static_cast<Vertex>(u)))
        {
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
            //std::cout << "sum: " << sum << std::endl;

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
using EdgeSet = std::set<Edge>;
using EdgeSetIteratorPtr = std::shared_ptr<EdgeSet::iterator>;
using EdgeSetSharedPtr = std::shared_ptr<EdgeSet>;

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
    typedef std::pair<Field, EMove> edge_descriptor;
    typedef boost::undirected_tag directed_category;
    typedef boost::disallow_parallel_edge_tag edge_parallel_category;
    typedef boost::incidence_graph_tag traversal_category;

    //typedef edge_prop edge_property_type;
    //typedef vert_prop vertex_property_type;
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

std::pair<PuzzleStateSpace::out_edge_iterator, PuzzleStateSpace::out_edge_iterator>
out_edges(const PuzzleStateSpace::vertex_descriptor& v, const PuzzleStateSpace&)
{
    EdgeSetSharedPtr edge_set = std::make_shared<EdgeSet>();
    for (const auto& move : getAvailableMoves(v))
    {
        edge_set->insert(std::make_pair(v, move));
    }

    //std::cout << "Set size: " << edge_set->size() << std::endl;

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
        //std::cout << "Default map, key:\n" << k << std::endl;

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

template <typename MapType>
typename MapType::value_type get(const MapType& map, const PuzzleStateSpace::edge_descriptor& desc)
{
    return map[desc];
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

    //std::cout << getAvailableMoves(state) << std::endl;
    typedef boost::associative_property_map<default_map<Field, unsigned>> DistanceMap;
    typedef default_map<Field, unsigned> WrappedDistanceMap;
    WrappedDistanceMap wrappedMap = WrappedDistanceMap(200);
    DistanceMap d = DistanceMap(wrappedMap);

    applyMoveInPlace(state, EMove::Down);
    wrappedMap[state] = distanceToGoal(state);
    std::cout << state << "distance to goal: " << distanceToGoal(state) << std::endl;
    auto prev_state = state;
    applyMoveInPlace(state, EMove::Right);
    std::cout << "State comparison: " << std::boolalpha << (prev_state < state) << " " << (state < prev_state) << std::endl;
    wrappedMap[state] = distanceToGoal(state);
    std::cout << state << "distance to goal: " << distanceToGoal(state) << std::endl;
    applyMoveInPlace(state, EMove::Down);
    wrappedMap[state] = distanceToGoal(state);
    std::cout << state << "distance to goal: " << distanceToGoal(state) << std::endl;
    applyMoveInPlace(state, EMove::Left);
    wrappedMap[state] = distanceToGoal(state);
    std::cout << state << "distance to goal: " << distanceToGoal(state) << std::endl;

    //PuzzleGraph g;

    PuzzleStateSpace space;
    auto initial_state = state;

    std::map<Field, Field> pred_map;
    boost::associative_property_map<std::map<Field, Field>> pred_property_map(pred_map);

    std::map<Field, unsigned> vertex_index_map;
    std::map<Field, unsigned> rank_map;
    std::map<Field, boost::default_color_type> color_map;

    wrappedMap[initial_state] = 0;

    try
    {
        boost::astar_search_no_init(
                space,
                initial_state,
                &distanceToGoal,
                boost::predecessor_map(pred_property_map)
                    .visitor(AstarGoalVisitor<Field>())
                    .weight_map(AlwaysOneMap())
                    .vertex_index_map(boost::associative_property_map<std::map<Field, unsigned>>(vertex_index_map))
                    .rank_map(boost::associative_property_map<std::map<Field, unsigned>>(rank_map))
                    .color_map(boost::associative_property_map<std::map<Field, boost::default_color_type>>(color_map))
                    .distance_map(d)
                    .distance_compare(std::less<unsigned>())
                    .distance_combine(std::plus<unsigned>())
                    );
    }
    catch (const FoundGoal& g)
    {
        //std::list<Field> moves;
        //for (auto state = end_state; !(state == initial_state); )
        //{
            //const auto& mv = pred_map[state];
            //moves.push_front(mv);
            //state = mv;
            ////applyMoveInPlace(state, revertMove(mv));
        //}
    }

    std::cout << "Distance map" << std::endl;
    for (const auto weight_pair : wrappedMap.m)
    {
        std::cout << weight_pair.first << "\n" << weight_pair.second << "\n";
    }

    return 0;
}
