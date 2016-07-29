#ifndef __GRAPHUTILS_HPP__
#define __GRAPHUTILS_HPP__

#include <string>
#include <unordered_map>
#include <utility>

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/range/iterator_range.hpp>

#include "AlignmentUtils.hpp"
#include "Utils.hpp"


using startV   = boost::adjacency_list<>::vertex_descriptor;
using endVList = std::vector<boost::adjacency_list<>::vertex_descriptor>;


struct VerticeProps {
  std::string name;
  bool isStart  = false;
  bool isFinish = false;
};

struct EdgeProps {
  std::string label;
  std::string lowlink;
};

struct GraphProps {
  std::string name;
  startV start;
  endVList ends;

  alignment alm;
  alignmentGrouping alh;
};

using Graph_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                                    VerticeProps, EdgeProps, GraphProps>;

using vDesc = Graph_t::vertex_descriptor;
using eDesc = Graph_t::edge_descriptor;

using vIter = Graph_t::vertex_iterator;
using vIterPair = std::pair<vIter, vIter>;

using eIter = Graph_t::edge_iterator;
using eIterPair = std::pair<eIter, eIter>;

using oeIter = Graph_t::out_edge_iterator;
using oeIterPair = std::pair<oeIter, oeIter>;

using labelGroupingMap = std::unordered_map<label, alignmentGrouping>;
using labelAlmSubMap = std::unordered_map<label, alignmentSub>;


namespace Graph {
  Graph_t parse(const std::string &path);

  void setStartVertex(Graph_t &g);

  void addAlm(Graph_t &g, const alignment &al);
  void addAlmHalf(Graph_t &g, const alignmentGrouping &gp);

  alignment getAlm(const Graph_t &g);
  alignmentHalf getAlmHalf(const Graph_t &g);
  labelGroupingMap getLabelGroupingMap(const Graph_t &g);
  labelAlmSubMap getLabelAlmSubMap(const Graph_t &g);

  void print(const Graph_t &g);

  void printAlm(const Graph_t &g);
  void printAlmHalf(const Graph_t &g);

  void printVertices(const Graph_t &g);
  void printEdges(const Graph_t &g);

  void printOutEdge(const Graph_t &g, const eDesc &e);
  void printOutEdges(const Graph_t &g, const vDesc &vd);

  void printLgm(const labelGroupingMap &lgm);
  void printLsm(const labelAlmSubMap &lsm);
}

#endif // __GRAPHUTILS_HPP__
