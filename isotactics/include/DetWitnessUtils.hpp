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


  DWG::Vertex createVertex(const std::string &name, const std::string &role);
  void addWGVertex(const WG::vDesc &wgv, DWG::Vertex &dwgv, DWG_t &dwg);


  void addWGVertex(const WG::vDesc &wgv, const DWG::vDesc &dwgv, DWG_t &dwg);

  DWG::vDesc addVertex(const DWG::Vertex &v, DWG_t &dwg);


  //DWG::vDesc addEmptyVertex(DWG_t &dwg);


  void print(const DWG_t &dwg);

}

#endif // __DETWITNESSUTILS_HPP__
