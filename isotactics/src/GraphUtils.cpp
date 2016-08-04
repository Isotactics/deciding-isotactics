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
  boost::read_graphviz(m, g, dp);

  return g;
}


void Graph::addAlm(Graph_t &g, const alignment &alm)
{
  g[boost::graph_bundle].alm = alm;
}

void Graph::addAlmHalf(Graph_t &g, const alignmentHalf &alh)
{
  g[boost::graph_bundle].alh = alh;
}

alignment Graph::getAlm(const Graph_t &g)
{
  return g[boost::graph_bundle].alm;
}

alignmentHalf Graph::getAlmHalf(const Graph_t &g)
{
  return g[boost::graph_bundle].alh;
}

labelGroupingMap Graph::getLabelGroupingMap(const Graph_t &g)
{
  labelGroupingMap lgm;

  const Range<Graph::eIter> edges = Util::makeRange(boost::edges(g));
  alignmentHalf alh = Graph::getAlmHalf(g);

  label l;
  alignmentGrouping gp;

  for (const Graph::eDesc &e : edges) {
    l = g[e].label;
    gp = Alm::getGrouping(l, alh);

    lgm[l] = gp;
  }

  return lgm;
}

labelAlmSubMap Graph::getLabelAlmSubMap(const Graph_t &g)
{
  labelAlmSubMap lsm;

  const Range<Graph::eIter> edges = Util::makeRange(boost::edges(g));
  alignment alm = Graph::getAlm(g);

  label l;
  alignmentSub almSub;

  for (const Graph::eDesc &e : edges) {
    l = g[e].label;

    for (const alignmentPair &p : alm) {

      if ((Alm::hasLabel(p.first, l)) || (Alm::hasLabel(p.second, l)))
        almSub.push_back(p);

    }

    lsm[l] = almSub;
    almSub.clear();
  }

  return lsm;

}

Graph::vDesc Graph::getStart(const Graph_t &g)
{
  vDesc start;
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
  vDesc res;
  const Range<Graph::vIter> vertices = Util::makeRange(boost::vertices(g));

  for (const Graph::vDesc &v : vertices) {
    if (g[v].name == vName)
      res = v;
  }

  return res;
}



void Graph::print(const Graph_t &g)
{
  Range<Graph::vIter> vertices = Util::makeRange(boost::vertices(g));

  for (const Graph::vDesc &v : vertices)
    Graph::printOutEdges(g, v);

  return;
}

void Graph::printAlm(const Graph_t &g)
{
  Alm::print(g[boost::graph_bundle].alm);
}

void Graph::printAlmHalf(const Graph_t &g)
{
  Alm::printGrouping(g[boost::graph_bundle].alh);
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

void Graph::printOutEdges(const Graph_t &g, const vDesc &vd) {

  const Range<Graph::oeIter> outEdges = Util::makeRange(boost::out_edges(vd, g));

  if (outEdges.empty())
    return;

  for (const Graph::eDesc &e : outEdges)
    Graph::printOutEdge(g, e);

  return;
}

void Graph::printLgm(const labelGroupingMap &lgm)
{
  for (const std::pair<label, alignmentGrouping> &p : lgm) {
    std::cout << p.first << " ->  ";
    Alm::printGrouping(p.second);
    Util::printLine();
  }

  return;
}

void Graph::printLsm(const labelAlmSubMap &lsm)
{
  for (const std::pair<label, alignmentSub> &p : lsm) {
    std::cout << p.first << " -> " << std::endl;
    Alm::print(p.second);
    Util::printLine();
  }

  return;
}
