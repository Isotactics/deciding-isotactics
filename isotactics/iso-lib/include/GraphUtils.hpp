#ifndef __GRAPHUTILS_HPP__
#define __GRAPHUTILS_HPP__

#include <string>
#include <unordered_map>
#include <utility>

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/range/iterator_range.hpp>

#include "Utils.hpp"


struct VerticeProps {
  std::string name;
  std::string role;
};

struct EdgeProps {
  std::string label;
  std::string lowlink;
};

struct GraphProps {
  std::string name;
};

using Graph_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                                    VerticeProps, EdgeProps, GraphProps>;


namespace Graph {
  using vDesc = Graph_t::vertex_descriptor;
  using eDesc = Graph_t::edge_descriptor;

  using vIter = Graph_t::vertex_iterator;
  using vIterPair = std::pair<vIter, vIter>;

  using eIter = Graph_t::edge_iterator;
  using eIterPair = std::pair<eIter, eIter>;

  using oeIter = Graph_t::out_edge_iterator;
  using oeIterPair = std::pair<oeIter, oeIter>;


  Graph_t parse(const std::string &path);

  Graph::vDesc getStart(const Graph_t &g);
  std::vector<Graph::vDesc> getEnds(const Graph_t &g);

  Range<Graph::oeIter> getOutEdges(const Graph_t &g, const Graph::vDesc &v);
  Graph::vDesc getVertex(const std::string &vName, const Graph_t &g);

  Graph::vDesc getDst(const Graph::vDesc &v, const std::string &l, const Graph_t &g);



  void print(const Graph_t &g);

  void printVertices(const Graph_t &g);
  void printEdges(const Graph_t &g);

  void printOutEdge(const Graph_t &g, const Graph::eDesc &e);
  void printOutEdges(const Graph_t &g, const Graph::vDesc &vd);


}

#endif // __GRAPHUTILS_HPP__
