#ifndef __DETWITNESSUTILS_HPP__
#define __DETWITNESSUTILS_HPP__

#include <string>
#include <vector>

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/range/iterator_range.hpp>

#include "WitnessUtils.hpp"
#include "Utils.hpp"


struct dwgVertexProps {
  std::string name;
  std::string role;

  std::vector<WG::vDesc> vs;
};

struct dwgEdgeProps {
  std::string name;
  alignmentGrouping gp;
};

struct dwgProps {
  std::string name;
};

using DWG_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                                    dwgVertexProps, dwgEdgeProps, dwgProps>;


namespace DWG {
  struct Vertex {
  std::string name;
  std::string role;

  std::vector<WG::vDesc> vs;
  };

  using vDesc = DWG_t::vertex_descriptor;
  using eDesc = DWG_t::edge_descriptor;

  using vIter = DWG_t::vertex_iterator;
  using vIterPair = std::pair<vIter, vIter>;

  using eIter = DWG_t::edge_iterator;
  using eIterPair = std::pair<eIter, eIter>;

  using oeIter = DWG_t::out_edge_iterator;
  using oeIterPair = std::pair<oeIter, oeIter>;

  DWG_t createLhs(const WG_t &wg, const edgeLabelSet &els);
  DWG_t createRhs(const WG_t &wg, const edgeLabelSet &els);

  DWG::Vertex createVertex();
  DWG::Vertex createVertex(const std::string &name, const std::string &role);

  void addVertexToSet(DWG::Vertex &dwgv, const WG::vDesc wgv, const WG_t &wg);
  void addVertexToSet(DWG_t &dwg, DWG::vDesc &dwgv, const WG::vDesc wgv, const WG_t &wg);

  void updateVertexName(DWG::Vertex &dwgv, const WG_t &wg);
  void updateVertexName(const DWG::vDesc &dwgv, DWG_t &dwg, const WG_t &wg);

  DWG::vDesc addVertex(DWG_t &dwg, const DWG::Vertex &v, const WG_t &wg);
  DWG::vDesc addEmptyVertex(DWG_t &dwg, const edgeLabelSet &els);

  DWG::eDesc addEdge(DWG::vDesc &v1, const alignmentGrouping &gp, DWG::vDesc &v2, DWG_t &dwg);

  DWG::Vertex getVertex(const DWG::vDesc &v, const DWG_t &dwg);
  DWG::vDesc  getVertex(const DWG::Vertex &v, const DWG_t &dwg);

  DWG::vDesc getStart(const DWG_t &dwg);
  DWG::vDesc getDst(const DWG::vDesc &v, const alignmentGrouping &gp, const DWG_t &dwg);

  std::vector<WG::eDesc> getOutEdges(const DWG::Vertex &v, const WG_t &wg);
  std::vector<WG::eDesc> getOutEdges(const DWG_t & dwg, const WG_t &wg, const DWG::vDesc &v);

  bool hasVertex(const DWG::Vertex &v, const DWG_t &dwg);
  bool vertexHasVertex(const DWG::Vertex &dwgv, const WG::vDesc &wgv);

  bool hasVertexThatContains(const WG::vDesc &v, const DWG_t &dwg);

  DWG::vDesc getVertexThatContains(const WG::vDesc &wgv, const DWG_t &dwg);

  bool alreadyVisited(const std::vector<WG::vDesc> &vs, const WG::vDesc &t);
  bool alreadyContained(const DWG::Vertex &v, const WG::vDesc &nv);
  bool destinationsAreSubset(const std::vector<WG::vDesc> dsts, const DWG::Vertex &v);

  void findAllNodesLhs(DWG::Vertex &v, const WG_t &wg);
  void findAllNodesRhs(DWG::Vertex &v, const WG_t &wg);


  void setFinalState(DWG::Vertex &v, const WG_t &wg);






  void print(const DWG_t &dwg);
  void printOutEdges(const DWG::vDesc &v, const DWG_t &dwg);
  void printOutEdge(const DWG::eDesc &e, const DWG_t &dwg);


}

#endif // __DETWITNESSUTILS_HPP__
