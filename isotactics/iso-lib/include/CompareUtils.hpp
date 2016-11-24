#ifndef __COMPAREUTILS_HPP__
#define __COMPAREUTILS_HPP__

#include <string>
#include <vector>

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/range/iterator_range.hpp>

#include "AlignmentUtils.hpp"
#include "GraphUtils.hpp"
#include "DetWitnessUtils.hpp"

namespace Cmp {
  struct VertexPair {
    Graph::vDesc gv;
    DWG::vDesc dwgv;

    VertexPair() : gv{0}, dwgv{0} {}
    VertexPair(Graph::vDesc g, DWG::vDesc d) : gv{g}, dwgv{d} {}

    bool operator==(const VertexPair &other) const {
      return (gv == other.gv) && (dwgv == other.dwgv);
    }
  };

  struct Vertex {
    Cmp::VertexPair pair;
    std::vector<Cmp::VertexPair> path;

    Vertex() {}
    Vertex(Graph::vDesc g, DWG::vDesc d) : pair{Cmp::VertexPair(g, d)}{}

    bool operator==(const Cmp::Vertex &other) const {
      return (pair == other.pair) && (path == other.path);
    }
  };

  bool isExitCondition(const Cmp::VertexPair &vp, const Graph_t &g, const DWG_t &dwg);
  bool alreadyVisited(const Cmp::Vertex &p, std::vector<Cmp::Vertex> visited);

  bool isEqual(const Graph_t &g, const DWG_t &dwg, const labelGroupingMap &lgm);

  void inheritPath(Cmp::Vertex &v, const Cmp::Vertex &vpath);




  std::string vpToString(const Cmp::VertexPair &vp, const Graph_t &g, const DWG_t &dwg);
  std::string vertexToString(const Cmp::Vertex &v, const Graph_t &g, const DWG_t &dwg);



}



#endif // __COMPAREUTILS_HPP__
