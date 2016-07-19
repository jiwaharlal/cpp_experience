#include <iostream>
#include <iterator>
#include <utility>
#include <algorithm>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/foreach.hpp>
#include <boost/graph/vertex_and_edge_range.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/container/set.hpp>

template <typename T1, typename T2>
std::ostream& operator <<(std::ostream& out, const std::pair<T1, T2>& p)
{
   out << "<" << p.first << ", " << p.second << ">";

   return out;
}

template <typename T>
std::ostream& operator <<(std::ostream& out, const std::vector<T>& v)
{
   out << "{";
   if (!v.empty())
   {
      typename std::vector<T>::const_iterator it = v.begin();
      out << *it;
      for (++it; it != v.end(); ++it)
      {
         out << ", " << *it;
      }
   }
   out << "}";

   return out;
}

struct BfsPrinter: public boost::default_bfs_visitor
{
   template <class GraphType, class VertexDescType>
   void discover_vertex(const VertexDescType& v, const GraphType& g)
   {
      std::cout << "Vertex " << v << " discovered" << std::endl;
   }

   template <class GraphType, class VertexDescType>
   void finish_vertex(const VertexDescType& v, const GraphType& g)
   {
      std::cout << "Vertex " << v << " finished" << std::endl;
   }
};

void minCut()
{
   boost::container::set<std::pair<int, int> > vertices;
   vertices.random();
}

int main(int, char**)
{
   typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> tGraph;
   typedef boost::graph_traits<tGraph>::vertex_descriptor tVertexDescriptor;

   typedef std::pair<int, int> tEdge;
   std::vector<tEdge> edges = {{0, 1}, {1, 2}, {0, 2}, {2, 3}, {3, 9}};
   std::vector<int> ints = {1, 2, 3, 4};

   tGraph graph(edges.begin(), edges.end(), 15);
   //graph.add_vertex(9);
   //graph.remove_vertex(3);

   //std::copy(ints.begin(), ints.end(), std::ostream_iterator<int>(std::cout, ", "));
   std::cout << edges << std::endl;

   boost::graph_traits<tGraph>::edge_iterator ei, eiEnd;
   boost::graph_traits<tGraph>::vertex_iterator vi, viEnd;

   //std::cout << std::endl << eiEnd - ei << " total vertices" << std::endl;

   std::cout << std::endl << "Edges:" << std::endl;
   for (boost::tie(ei, eiEnd) = boost::edges(graph); ei != eiEnd; ++ei)
   {
      std::cout << *ei << ", ";
   }
   std::cout << std::endl;

   boost::tie(vi, viEnd);
   auto it = std::find(vi, viEnd, 0);
   if (it != viEnd)
   {
      std::cout << "Removing vertex" << *it << std::endl;
      boost::remove_vertex(*it, graph);
   }
   else
   {
      std::cout << "Vertex not found" << std::endl;
   }

   std::cout << std::endl << "Vertices" << std::endl;
   for (boost::tie(vi, viEnd) = boost::vertices(graph); vi != viEnd; ++vi)
   {
      std::cout << *vi << ", ";
   }

   std::cout << std::endl << "Edges:" << std::endl;
   for (boost::tie(ei, eiEnd) = boost::edges(graph); ei != eiEnd; ++ei)
   {
      std::cout << *ei << ", ";
   }

   std::cout << std::endl;

   BfsPrinter bfsp;
   boost::breadth_first_search(graph, boost::vertex(1, graph), boost::visitor(bfsp));

   //std::cout << edges[0] << std::endl;

   return 0;
}
