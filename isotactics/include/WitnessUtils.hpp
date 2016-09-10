#ifndef __WITNESSUTILS_HPP__
#define __WITNESSUTILS_HPP__

#include <string>

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/range/iterator_range.hpp>

#include "AlignmentUtils.hpp"
#include "GraphUtils.hpp"
#include "MatchUtils.hpp"
#include "Utils.hpp"


struct wgVertexProps {
  std::string name;
  std::string v1Name;
  std::string v2Name;
  matchSet ms;

  std::string role;
};

struct wgEdgeProps {
  std::string name;

  alignmentGrouping gp1;
  alignmentGrouping gp2;
};

struct wgProps {
  std::string name;
};

using WG_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                                    wgVertexProps, wgEdgeProps, wgProps>;

using vName = std::string;

namespace WG {
  struct Vertex {
    std::string name;
    std::string v1Name;
    std::string v2Name;
    matchSet ms;

    std::string role;
  };

  using vDesc = WG_t::vertex_descriptor;
  using eDesc = WG_t::edge_descriptor;

  using vIter = WG_t::vertex_iterator;
  using vIterPair = std::pair<vIter, vIter>;

  using eIter = WG_t::edge_iterator;
  using eIterPair = std::pair<eIter, eIter>;

  using oeIter = WG_t::out_edge_iterator;
  using oeIterPair = std::pair<oeIter, oeIter>;

  WG_t create(const Graph_t &g1, const Graph_t &g2, const alignment &alm);

  WG::Vertex createVertex(const vName &v1, const vName &v2, const matchSet &ms);
  WG::vDesc getVertex(const WG::Vertex &v, const WG_t &wg);
  WG::vDesc addVertex(const WG::Vertex &v, WG_t &wg);

  std::string getVertexName(const WG::Vertex &v);

  WG::eDesc addEdge(WG::vDesc &v1, const alignmentGrouping &gp1,
                    const alignmentGrouping &gp2, WG::vDesc &v2, WG_t &wg);

  bool isFinalState(const WG::vDesc &wgv);

  bool hasVertex(const WG::Vertex &v, const WG_t &wg);
  bool vertexEqual(const WG::Vertex &v1, const WG::Vertex &v2);

  void print(const WG_t &wg);
  void printOutEdges(const WG_t &wg, const WG::vDesc &v);
  void printOutEdge(const WG_t &wg, const WG::eDesc &e);

  void printDebug(const WG_t &wg);
  void printOutEdgesDebug(const WG_t &wg, const WG::vDesc &v);
  void printOutEdgeDebug(const WG_t &wg, const WG::eDesc &e);
}

#endif // __WITNESSUTILS_HPP__
