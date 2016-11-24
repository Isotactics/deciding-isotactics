#include <iostream>

#include "GraphUtils.hpp"
#include "Utils.hpp"


Graph_t Graph::parse(const std::string &path)
{
  Graph_t g;

  boost::dynamic_properties dp(boost::ignore_other_properties);

  dp.property("node_id", boost::get(&VerticeProps::name, g));
  dp.property("role", boost::get(&VerticeProps::role, g));

  dp.property("label", boost::get(&EdgeProps::label, g));
  dp.property("lowlink", boost::get(&EdgeProps::lowlink, g));

  std::ifstream m(path);

  if (!m) {
    std::cout << "[Err] Graph::parse // ifstream open" << std::endl;
    return g;
  }

  boost::read_graphviz(m, g, dp);

  return g;
}


Graph::vDesc Graph::getStart(const Graph_t &g)
{
  Graph::vDesc start;
  const Range<Graph::vIter> vertices = Util::makeRange(boost::vertices(g));

  for (const Graph::vDesc &v : vertices) {
    if (g[v].role == "start")
      start = v;
  }

  return start;
}

std::vector<Graph::vDesc> Graph::getEnds(const Graph_t &g)
{
  std::vector<Graph::vDesc> res;

  const Range<Graph::vIter> vertices = Util::makeRange(boost::vertices(g));

  for (const Graph::vDesc &v : vertices) {
    if (g[v].role == "end")
      res.push_back(v);
  }

  return res;
}

Range<Graph::oeIter> Graph::getOutEdges(const Graph_t &g, const Graph::vDesc &v)
{
  return Util::makeRange(boost::out_edges(v, g));
}

Graph::vDesc Graph::getVertex(const std::string &vName, const Graph_t &g)
{
  Graph::vDesc res;
  const Range<Graph::vIter> vertices = Util::makeRange(boost::vertices(g));

  for (const Graph::vDesc &v : vertices) {
    if (g[v].name == vName)
      res = v;
  }

  return res;
}


Graph::vDesc Graph::getDst(const Graph::vDesc &v, const std::string &l, const Graph_t &g)
{
  Graph::vDesc dst;

  Range<Graph::oeIter> oes = Graph::getOutEdges(g, v);

  for (const Graph::eDesc &e : oes) {
    if (g[e].label == l) {
      dst = boost::target(e, g);
      break;
    }
  }

  return dst;
}








void Graph::print(const Graph_t &g)
{
  Range<Graph::vIter> vertices = Util::makeRange(boost::vertices(g));

  for (const Graph::vDesc &v : vertices) {
    if (g[v].role == "start") {
      std::cout << "  \"" << g[v].name << "\" [role=\"start\"]" << std::endl;
      continue;
    }

    if (g[v].role == "end") {
      std::cout << "  \"" << g[v].name << "\" [role=\"end\"]" << std::endl;
      continue;
    }
  }

  Util::printLine();

  for (const Graph::vDesc &v : vertices)
    Graph::printOutEdges(g, v);

  return;
}

void Graph::printVertices(const Graph_t &g)
{
  const Range<Graph::vIter> vertices = Util::makeRange(boost::vertices(g));

  for (const Graph::vDesc &v : vertices)
    std::cout << g[v].name << std::endl;

  return;
}

void Graph::printEdges(const Graph_t &g)
{
  const Range<Graph::eIter> edges = Util::makeRange(boost::edges(g));

  for (const Graph::eDesc &e : edges)
    std::cout << g[e].label << std::endl;

  return;
}

void Graph::printOutEdge(const Graph_t &g, const eDesc &e)
{
  const Graph::vDesc src = boost::source(e, g);
  const Graph::vDesc dst = boost::target(e, g);

  std::cout << g[src].name << " -> " << g[dst].name;
  std::cout << " [label=\"" << g[e].label << "\"]" << std::endl;
}

void Graph::printOutEdges(const Graph_t &g, const Graph::vDesc &vd) {

  const Range<Graph::oeIter> outEdges = Util::makeRange(boost::out_edges(vd, g));

  if (outEdges.empty())
    return;

  for (const Graph::eDesc &e : outEdges)
    Graph::printOutEdge(g, e);

  return;
}


