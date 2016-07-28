#include <iostream>
#include "GraphUtils.h"


using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                                    VerticeProps, EdgeProps>;

using vDesc = Graph::vertex_descriptor;
using eDesc = Graph::edge_descriptor;

using vIter = Graph::vertex_iterator;
using vIterPair = std::pair<vIter, vIter>;

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
}
