#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include "DetGraph.hpp"
#include "HelperMaps.hpp"



DG_t DG::determinize(const Graph_t &g, const edgeLabelSet &els)
{
  DG_t dg;

  Graph::vDesc gStart = Graph::getStart(g);
  DG::Vertex dgStart = DG::createStart();

  DG::addVertexToSet(gStart, dgStart);
  DG::updateVertexName(dgStart, g);

  DG::vDesc dgStartvd = DG::addVertex(dg, dgStart);

  std::deque<DG::vDesc> dgTodo;
  dgTodo.push_back(dgStartvd);

  while (!dgTodo.empty()) {

    DG::vDesc curr = dgTodo.front();
    dgTodo.pop_front();

    std::vector<Graph::eDesc> outEdges = DG::getOutEdges(dg, curr, g);

    for (const alignmentGrouping &gp : els) {
      label l = Alm::getLabelFromGrouping(gp);

      if (!DG::hasEdgeForGrouping(g, outEdges, gp))
        continue;

      std::vector<Graph::eDesc> edges = DG::getEdgesForGrouping(g, outEdges, gp);
      std::vector<Graph::vDesc> dsts = Graph::getDestinations(g, edges);

      DG::Vertex nv = DG::createVertex(dsts, g);
      DG::updateVertexName(nv, g);

      if (DG::hasVertex(dg, nv)) {
        DG::vDesc existingDst = DG::getVertexByName(dg, nv.name);
        DG::addEdge(dg, curr, l, existingDst);

        continue;
      }

      DG::vDesc nvd = DG::addVertex(dg, nv);
      DG::addEdge(dg, curr, l, nvd);

      dgTodo.push_back(nvd);
    }
  }

  return dg;
}

DG::Vertex DG::createVertex()
{
  DG::Vertex nv;

  return nv;
}

DG::Vertex DG::createVertex(const std::vector<Graph::vDesc> &vertices, const Graph_t &g)
{
  DG::Vertex nv;

  for (const Graph::vDesc &v : vertices) {
    DG::addVertexToSet(v, nv);

    if (Graph::isFinalState(g, v))
      nv.role = "end";
  }

  return nv;
}

DG::Vertex DG::createStart()
{
  DG::Vertex nv;
  nv.role = "start";

  return nv;
}

DG::vDesc DG::getStart(const DG_t &dg)
{
  DG::vDesc res;

  const Range<DG::vIter> vertices = Util::makeRange(boost::vertices(dg));

  for (const DG::vDesc &vd : vertices) {
    if (dg[vd].role == "start")
      res =  vd;
  }

  return res;
}

bool DG::hasVertex(const DG_t &dg, const DG::Vertex &v)
{
  const Range<DG::vIter> vertices = Util::makeRange(boost::vertices(dg));

  for (const DG::vDesc &vd : vertices) {
    if (dg[vd].name == v.name)
      return true;
  }

  return false;
}

DG::vDesc DG::getVertexByName(const DG_t &dg, const std::string &name)
{
  DG::vDesc res;

  const Range<DG::vIter> vertices = Util::makeRange(boost::vertices(dg));

  for (const DG::vDesc &vd : vertices) {
    if (dg[vd].name == name)
      res =  vd;
  }

  return res;
}

void DG::addVertexToSet(const Graph::vDesc &gv, DG::Vertex &dgv)
{
  (dgv.vs).push_back(gv);
  std::sort((dgv.vs).begin(), (dgv.vs).end());

  return;
}

void DG::updateVertexName(DG::Vertex &dgv, const Graph_t &g)
{
  std::stringstream name;
  name << "{";

  std::vector<Graph::vDesc>::iterator it;

  for (it = (dgv.vs).begin(); it != ((dgv.vs).end() - 1); ++it)
    name << g[*it].name << ", ";

  name << g[*it].name << "}";

  dgv.name = name.str();

  return;
}

void DG::updateVertexName(DG_t &dg, const DG::vDesc &v, const Graph_t &g)
{
  std::stringstream name;
  name << "{";

  std::vector<Graph::vDesc>::iterator it;

  for (it = (dg[v].vs).begin(); it != ((dg[v].vs).end() - 1); ++it)
    name << g[*it].name << ", ";

  name << g[*it].name << "}";

  dg[v].name = name.str();

  return;
}

DG::vDesc DG::addEmptyVertex(DG_t &dg, const edgeLabelSet &els)
{
  DG::vDesc nv = boost::add_vertex(dg);

  dg[nv].name = "{}";
  dg[nv].role = "empty";

  DG::addSelfEdges(dg, nv, els);

  return nv;
}

void DG::addSelfEdges(DG_t &dg, const DG::vDesc &v, const edgeLabelSet &els)
{
  std::vector<label> labels = Helper::elsFlatten(els);

  for (const label &l : labels)
    addEdge(dg, v, l, v);

  return;
}

DG::eDesc DG::addEdge(DG_t &dg, const DG::vDesc &src, const label &l, const DG::vDesc &dst)
{
  DG::eDesc ne = boost::add_edge(src, dst, dg).first;

  dg[ne].label = l;
  dg[ne].lowlink = "";

  return ne;
}

DG::vDesc DG::addVertex(DG_t &dg, const DG::Vertex &v)
{
  DG::vDesc nv = boost::add_vertex(dg);

  dg[nv].name = v.name;
  dg[nv].role = v.role;
  dg[nv].vs   = v.vs;

  return nv;
}

std::vector<Graph::eDesc> DG::getOutEdges(const DG_t &dg, const DG::vDesc &v, const Graph_t &g)
{
  std::vector<Graph::eDesc> allOutEdges;

  for (const DG::vDesc &gvd : dg[v].vs) {
    Range<Graph::oeIter> outEdges = Graph::getOutEdges(g, gvd);

    for (const Graph::eDesc &oe : outEdges)
      allOutEdges.push_back(oe);

  }

  return allOutEdges;
}

Range<DG::oeIter> DG::getOutEdges(const DG_t &g, const DG::vDesc &v)
{
  return Util::makeRange(boost::out_edges(v, g));
}

bool DG::hasEdgeForGrouping(const Graph_t &g, const std::vector<Graph::eDesc> &edges, const alignmentGrouping &gp)
{
  for (const Graph::eDesc &e : edges) {
    if (g[e].gp == gp)
      return true;
  }

  return false;
}

std::vector<Graph::eDesc> DG::getEdgesForGrouping(const Graph_t &g, const std::vector<Graph::eDesc> &edges, const alignmentGrouping &gp)
{
  std::vector<Graph::eDesc> res;

  for (const Graph::eDesc &e : edges) {
    if (g[e].gp == gp)
      res.push_back(e);
  }

  return res;
}

DG::vDesc DG::getDst(const DG::vDesc &v, const std::string &l, const DG_t &g)
{
  DG::vDesc dst;

  Range<DG::oeIter> oes = DG::getOutEdges(g, v);

  for (const DG::eDesc &e : oes) {
    if (g[e].label == l) {
      dst = boost::target(e, g);
      break;
    }
  }

  return dst;
}













void DG::print(const DG_t &g)
{
  return DG::print(g, std::cout);
}
void DG::print(const DG_t &g, std::ostream& target)
{
  target << "digraph {" << std::endl;

  Range<DG::vIter> vertices = Util::makeRange(boost::vertices(g));

  for (const DG::vDesc &v : vertices) {
    if (g[v].role == "start") {
      target << " \"" << g[v].name << "\" [role=\"start\"]" << std::endl;
      continue;
    }

    if (g[v].role == "end") {
      target << " \"" << g[v].name << "\" [role=\"end\"]" << std::endl;
      continue;
    }

    if (g[v].role == "empty") {
      target << " \"" << g[v].name << "\" [role=\"empty\"]" << std::endl;
      continue;
    }
  }

  target << std::endl;

  for (const DG::vDesc &v : vertices)
    DG::printOutEdges(g, v, target);

  target << "}\n\n" << std::endl;

  return;
}

void DG::printVertices(const DG_t &g)
{
  const Range<DG::vIter> vertices = Util::makeRange(boost::vertices(g));

  for (const DG::vDesc &v : vertices)
    std::cout << g[v].name << std::endl;

  return;
}

void DG::printEdges(const DG_t &g)
{
  const Range<DG::eIter> edges = Util::makeRange(boost::edges(g));

  for (const DG::eDesc &e : edges)
    std::cout << g[e].label << std::endl;

  return;
}

void DG::printOutEdge(const DG_t &g, const eDesc &e)
{
  return DG::printOutEdge(g, e, std::cout);
}
void DG::printOutEdge(const DG_t &g, const eDesc &e, std::ostream& target)
{
  const DG::vDesc src = boost::source(e, g);
  const DG::vDesc dst = boost::target(e, g);

  target << "  " << g[src].name << " -> " << g[dst].name;

  dgEdgeProps edge = g[e];

  target << " [label=\"" << g[e].label << "\"]" << std::endl;
  // target << " [label=\"" << Alm::groupingToStr(g[e].gp) << "\"]" << std::endl;
}

void DG::printOutEdges(const DG_t &g, const DG::vDesc &vd)
{
  return DG::printOutEdges(g, vd, std::cout);
}
void DG::printOutEdges(const DG_t &g, const DG::vDesc &vd, std::ostream& target)
{
  const Range<DG::oeIter> outEdges = Util::makeRange(boost::out_edges(vd, g));

  if (outEdges.empty())
    return;

  for (const DG::eDesc &e : outEdges)
    DG::printOutEdge(g, e, target);

  return;
}




/*

Graph::Vertex Graph::createVertex()
{
  Graph::Vertex nv;
  return nv;
}


Graph::Vertex Graph::createVertex(const Graph_t &g, const Graph::vDesc &v)
{
  Graph::Vertex nv;
  nv.name = g[v].name;
  nv.role = g[v].role;

  return nv;
}





Graph::vDesc Graph::addVertex(Graph_t &g, const Graph::Vertex &v)
{
  Graph::vDesc nv = boost::add_vertex(g);

  g[nv].name = v.name;
  g[nv].role = v.role;
  //g[nv].vs = v.vs;

  return nv;
}


void Graph::addVertexToSet(const Graph::vDesc &vd, Graph::Vertex &v)
{
  (v.vs).push_back(vd);

  return;
}


void Graph::addVertexToSet(const Graph_t &g, const Graph::vDesc &vd, Graph::Vertex &v)
{
  std::cout << "vd: " << vd << "\n";

  (v.vs).push_back(vd);
  std::sort(v.vs.begin(), v.vs.end());

  Graph::updateVertexName(g, v);

  return;
}







std::vector<Graph::eDesc> Graph::getOutEdges(const Graph_t &g, const Graph::Vertex &v)
{
  std::vector<Graph::eDesc> oedges;

  for (const Graph::vDesc &gv : v.vs) {
    Range<Graph::oeIter> oes = Util::makeRange(boost::out_edges(gv, g));

    for (const Graph::eDesc &e : oes)
      oedges.push_back(e);
  }

  return oedges;
}

*/
