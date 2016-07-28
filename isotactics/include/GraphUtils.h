#include <string>
#include <utility>

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/range/iterator_range.hpp>

#include "AlignmentUtils.h"


struct VerticeProps {
  std::string name;
};

struct EdgeProps {
  std::string label;
};

struct GraphProps {
  std::string name;
  alignment al;
  alignmentGrouping alh;
};


using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                                    VerticeProps, EdgeProps, GraphProps>;

using vDesc = Graph::vertex_descriptor;
using eDesc = Graph::edge_descriptor;

using vIter = Graph::vertex_iterator;
using vIterPair = std::pair<vIter, vIter>;

using eIter = Graph::edge_iterator;
using eIterPair = std::pair<eIter, eIter>;

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

void addAlignment(Graph &g, const alignment &al);
void addAlignmentHalf(Graph &g, const alignmentGrouping &gp);

alignmentHalf getAlignmentHalf(const Graph &g);


void printAllEdges(const Graph &g);

void printOutEdge(const Graph &g, const eDesc &e);
void printOutEdges(const Graph &g, const vDesc &vd);
void printGraph(const Graph &g);
