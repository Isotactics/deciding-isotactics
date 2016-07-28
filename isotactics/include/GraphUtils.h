#include <string>

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/range/iterator_range.hpp>


struct VerticeProps {
  std::string name;
};

struct EdgeProps {
  std::string label;
};


using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                                    VerticeProps, EdgeProps>;

using vDesc = Graph::vertex_descriptor;
using eDesc = Graph::edge_descriptor;

using vIter = Graph::vertex_iterator;
using vIterPair = std::pair<vIter, vIter>;

using oeIter = Graph::out_edge_iterator;
using oeIterPair = std::pair<oeIter, oeIter>;

template <typename T>
using Range = boost::iterator_range<T>;


template <typename T>
Range<T> makeRange(const std::pair<T, T> &p)
{
  return boost::make_iterator_range(p);
}

Graph parseGraph(const std::string &path);


void printOutEdge(const Graph &g, const eDesc &e);
void printOutEdges(const Graph &g, const vDesc &vd);
void printGraph(const Graph &g);
