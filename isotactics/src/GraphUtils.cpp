#include <iostream>
#include "GraphUtils.h"


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



Graph parseGraph(const std::string &path)
{
  Graph g;

  boost::dynamic_properties dp(boost::ignore_other_properties);

  dp.property("node_id", boost::get(&VerticeProps::name, g));
  dp.property("label", boost::get(&EdgeProps::label, g));

  std::ifstream m(path);

  boost::read_graphviz(m, g, dp);

  return g;
}


void addAlignment(Graph &g, const alignment &al)
{
  g[boost::graph_bundle].al = al;;
}

void addAlignmentHalf(Graph &g, const alignmentHalf &alh)
{
  g[boost::graph_bundle].alh = alh;
}

alignmentHalf getAlignmentHalf(const Graph &g)
{
  return g[boost::graph_bundle].alh;
}






void printAllEdges(const Graph &g)
{
  const Range<eIter> edges = makeRange(boost::edges(g));

  for (const eDesc &e : edges)
    std::cout << g[e].label << std::endl;

}

void printOutEdge(const Graph &g, const eDesc &e)
{
  const vDesc src = boost::source(e, g);
  const vDesc dst = boost::target(e, g);

  std::cout << g[src].name << " -> " << g[dst].name;
  std::cout << " [label=\"" << g[e].label << "\"]" << std::endl;
}

void printOutEdges(const Graph &g, const vDesc &vd) {

  const Range<oeIter> outEdges = makeRange(boost::out_edges(vd, g));

  if (outEdges.empty())
    return;

  for (const eDesc &e : outEdges)
    printOutEdge(g, e);

}

void printGraph(const Graph &g)
{
  Range<vIter> vertices = makeRange(boost::vertices(g));

  for (const vDesc &v : vertices)
    printOutEdges(g, v);

  printAlignGrouping(g[boost::graph_bundle].alh);
}
