#ifndef __WITNESSUTILS_HPP__
#define __WITNESSUTILS_HPP__

#include <string>

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/range/iterator_range.hpp>

#include "AlignmentUtils.hpp"
#include "Utils.hpp"


using match             = alignmentPair;
using matchSet          = alignment;


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
  using vDesc = WG_t::vertex_descriptor;
  using eDesc = WG_t::edge_descriptor;

  using vIter = WG_t::vertex_iterator;
  using vIterPair = std::pair<vIter, vIter>;

  using eIter = WG_t::edge_iterator;
  using eIterPair = std::pair<eIter, eIter>;

  using oeIter = WG_t::out_edge_iterator;
  using oeIterPair = std::pair<oeIter, oeIter>;


  WG::vDesc addVertex(const vName &v1, const vName &v2, const matchSet &ms, WG_t &wg);
  WG::eDesc addEdge(WG::vDesc &v1, const alignmentGrouping &gp1,
                    const alignmentGrouping &gp2, WG::vDesc &v2, WG_t &wg);


  bool hasMatch(const matchSet &ms, const match &m);

  bool matchEmpty(const match &m);
  bool matchSetEmpty(const matchSet &ms);

  match getMatch(const alignment &alm, const label &l1, const label &l2);
  matchSet getMatchSet(const alignment &alm, const matchSet &ms, const label &l1, const label &l2);


  std::string matchToStr(const match &m);
  std::string matchSetToStr(const matchSet &m);

  void printMatch(const match &m);
  void printMatchSet(const matchSet &ms);

  void print(const WG_t &wg);
  void printOutEdges(const WG_t &wg, const WG::vDesc &v);
  void printOutEdge(const WG_t &wg, const WG::eDesc &e);
}

#endif // __WITNESSUTILS_HPP__
