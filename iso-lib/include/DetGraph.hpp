#ifndef __DETGRAPHUTILS_HPP__
#define __DETGRAPHUTILS_HPP__

#include <string>
#include <vector>

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/range/iterator_range.hpp>

#include "HelperMaps.hpp"
#include "GraphUtils.hpp"

struct dgVerticeProps {
  std::string name;
  std::string role;

  std::vector<Graph::vDesc> vs;
};

struct dgEdgeProps {
  std::string label;
  std::string lowlink;
};

struct dgGraphProps {
  std::string name;
};

using DG_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                                    dgVerticeProps, dgEdgeProps, dgGraphProps>;

namespace DG {

  struct Vertex {
    std::string name;
    std::string role;

    std::vector<Graph::vDesc> vs;
  };

  using vDesc = DG_t::vertex_descriptor;
  using eDesc = DG_t::edge_descriptor;

  using vIter = DG_t::vertex_iterator;
  using vIterPair = std::pair<vIter, vIter>;

  using eIter = DG_t::edge_iterator;
  using eIterPair = std::pair<eIter, eIter>;

  using oeIter = DG_t::out_edge_iterator;
  using oeIterPair = std::pair<oeIter, oeIter>;

  DG_t determinize(const Graph_t &g, const edgeLabelSet &els);

  DG::Vertex createVertex();
  DG::Vertex createVertex(const std::vector<Graph::vDesc> &vertices, const Graph_t &g);

  DG::Vertex createStart();
  DG::vDesc getStart(const DG_t &dg);


  bool hasVertex(const DG_t &dg, const DG::Vertex &v);
  DG::vDesc getVertexByName(const DG_t &dg, const std::string &name);

  void addVertexToSet(const Graph::vDesc &gv, DG::Vertex &dg);

  void updateVertexName(DG::Vertex &dgv, const Graph_t &g);
  void updateVertexName(DG_t &dg, const DG::vDesc &v, const Graph_t &g);

  DG::vDesc addEmptyVertex(DG_t &dg, const edgeLabelSet &els);
  void addSelfEdges(DG_t &dg, const DG::vDesc &v, const edgeLabelSet &els);

  DG::eDesc addEdge(DG_t &dg, const DG::vDesc &src, const label &l, const DG::vDesc &dst);
  DG::vDesc addVertex(DG_t &dg, const DG::Vertex &v);

  std::vector<DG::eDesc> getOutEdges(const DG_t &dg, const DG::vDesc &v, const Graph_t &g);
  Range<DG::oeIter> getOutEdges(const DG_t &g, const DG::vDesc &v);

  bool hasEdgeForGrouping(const Graph_t &g, const std::vector<Graph::eDesc> &edges, const alignmentGrouping &gp);
  std::vector<Graph::eDesc> getEdgesForGrouping(const Graph_t &g, const std::vector<Graph::eDesc> &edges, const alignmentGrouping &gp);

  DG::vDesc getDst(const DG::vDesc &v, const std::string &l, const DG_t &g);




  void print(const DG_t &g);

  void printVertices(const DG_t &g);
  void printEdges(const DG_t &g);

  void printOutEdge(const DG_t &g, const Graph::eDesc &e);
  void printOutEdges(const DG_t &g, const Graph::vDesc &vd);
}

#endif // __DETGRAPHUTILS_HPP__
