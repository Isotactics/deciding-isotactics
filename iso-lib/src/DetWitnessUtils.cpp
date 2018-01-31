#include <algorithm>
#include <string>

#include "DetWitnessUtils.hpp"
#include "WitnessUtils.hpp"

#include "Logging.hpp"

DWG_t DWG::createLhs(const WG_t &wg, const edgeLabelSet &els)
{
  DWG_t dwg;
  DWG::vDesc dwgEmpty = DWG::addEmptyVertex(dwg, els);

  DWG::Vertex dwgStart = DWG::createVertex("", "start");
  WG::vDesc wgStart = WG::getStart(wg);

  DWG::addVertexToSet(dwgStart, wgStart, wg);
  DWG::addVertex(dwg, dwgStart, wg);

  std::deque<DWG::Vertex> dwgTodo;

  dwgTodo.push_back(dwgStart);

  while (!dwgTodo.empty()) {

    DWG::Vertex currV = dwgTodo.front();

    DWG::vDesc dwgv1 = DWG::getVertex(currV, dwg);
    dwgTodo.pop_front();

    DEBUG << "checking node: " << dwg[dwgv1].name << std::endl;

    std::vector<WG::eDesc> oedges = DWG::getOutEdges(dwg, wg, dwgv1);

    DEBUG << "  out edges:\n";

    for (const WG::eDesc &e : oedges)
      WG::printOutEdgeDebug(wg, e);

    Util::printLineDebug();


    for (const alignmentGrouping &gp : els) {

      DEBUG << "  grouping " << Alm::groupingToStr(gp) << ":\n";

      if (!WG::hasEdgeLhs(oedges, gp, wg)) {
        DEBUG << "    no edge for label, adding edge to empty state\n\n\n";

        DWG::addEdge(dwgv1, gp, dwgEmpty, dwg);
        continue;
      }
      DEBUG << "    has edge for label\n";

      std::vector<WG::eDesc> edges = WG::getEdgesWithLabelLhs(oedges, gp, wg);
      std::vector<WG::vDesc> dsts  = WG::getDestinations(edges, wg);

      DEBUG << "    label: " << Alm::groupingToStr(gp) << "\n";
      DEBUG << "    edges:\n";
      for (const WG::eDesc &e : edges)
        WG::printOutEdgeDebug(wg, e);

      IF_DEBUG(
        std::cerr << "    destinations:\n";

        for (const WG::vDesc &v : dsts)
          std::cerr << wg[v].name << "\n";

        std::cerr << "    dsts are sub: " << DWG::destinationsAreSubset(dsts, DWG::getVertex(dwgv1, dwg)) << "\n";
      )

      if (DWG::destinationsAreSubset(dsts, DWG::getVertex(dwgv1, dwg))) {
        DEBUG << "    destinations are subset of myself, adding self-edge\n\n\n";
        DWG::addEdge(dwgv1, gp, dwgv1, dwg);
        continue;
      }

      DWG::Vertex nv = DWG::createVertex();

      for (const WG::vDesc &dst : dsts)
        DWG::addVertexToSet(nv, dst, wg);

      DWG::findAllNodesLhs(nv, wg);
      DWG::setFinalState(nv, wg);

      DEBUG << "    current vertex: " << nv.name << " role: " << nv.role << "\n";

      if (DWG::hasVertex(nv, dwg)) {
        DEBUG << "    vertex already exists, only adding edge\n\n\n";
        DWG::vDesc dwgv2 = DWG::getVertex(nv, dwg);
        DWG::addEdge(dwgv1, gp, dwgv2, dwg);
        continue;
      }

      DEBUG << "    vertex is new, adding vertex, edge and todo\n\n\n";
      DWG::vDesc dwgv2 = DWG::addVertex(dwg, nv, wg);
      DWG::addEdge(dwgv1, gp, dwgv2, dwg);

      dwgTodo.push_back(DWG::getVertex(dwgv2, dwg));


    } // label loop

    DEBUG << "\nTodo:\n";

    for (const DWG::Vertex &v : dwgTodo)
      DEBUG << v.name << std::endl;

    Util::printLineDebug();
    Util::printLineDebug();
    Util::printLineDebug();
  } // todo loop

  return dwg;
}

DWG_t DWG::createRhs(const WG_t &wg, const edgeLabelSet &els)
{
  DWG_t dwg;
  DWG::vDesc dwgEmpty = DWG::addEmptyVertex(dwg, els);

  DWG::Vertex dwgStart = DWG::createVertex("", "start");
  WG::vDesc wgStart = WG::getStart(wg);

  DWG::addVertexToSet(dwgStart, wgStart, wg);
  DWG::addVertex(dwg, dwgStart, wg);

  std::deque<DWG::Vertex> dwgTodo;
  std::vector<DWG::vDesc> visited;

  dwgTodo.push_back(dwgStart);

  while (!dwgTodo.empty()) {

    DWG::Vertex currV = dwgTodo.front();

    DWG::vDesc dwgv1 = DWG::getVertex(currV, dwg);
    dwgTodo.pop_front();

    DEBUG << "checking node: " << dwg[dwgv1].name << std::endl;

    std::vector<WG::eDesc> oedges = DWG::getOutEdges(dwg, wg, dwgv1);
    DEBUG << "  out edges:\n";

    for (const WG::eDesc &e : oedges)
      WG::printOutEdgeDebug(wg, e);

    Util::printLineDebug();


    for (const alignmentGrouping &gp : els) {

      DEBUG << "  grouping " << Alm::groupingToStr(gp) << ":\n";

      if (!WG::hasEdgeRhs(oedges, gp, wg)) {
        DEBUG << "    no edge for label, adding edge to empty state\n\n\n";
        DWG::addEdge(dwgv1, gp, dwgEmpty, dwg);
        continue;
      }
      DEBUG << "    has edge for label\n";

      std::vector<WG::eDesc> edges = WG::getEdgesWithLabelRhs(oedges, gp, wg);
      std::vector<WG::vDesc> dsts  = WG::getDestinations(edges, wg);

      DEBUG << "    label: " << Alm::groupingToStr(gp) << "\n";
      DEBUG << "    edges:\n";
      for (const WG::eDesc &e : edges)
        WG::printOutEdgeDebug(wg, e);

      IF_DEBUG(
        DEBUG << "    destinations:\n";
        for (const WG::vDesc &v : dsts)
          DEBUG << wg[v].name << "\n";
        DEBUG << "    dsts are sub: " << DWG::destinationsAreSubset(dsts, DWG::getVertex(dwgv1, dwg)) << "\n";
      )

      if (DWG::destinationsAreSubset(dsts, DWG::getVertex(dwgv1, dwg))) {
        DEBUG << "    destinations are subset of myself, adding self-edge\n\n\n";
        DWG::addEdge(dwgv1, gp, dwgv1, dwg);
        continue;
      }

      DWG::Vertex nv = DWG::createVertex();

      for (const WG::vDesc &dst : dsts)
        DWG::addVertexToSet(nv, dst, wg);

      DWG::findAllNodesRhs(nv, wg);
      DWG::setFinalState(nv, wg);

      DEBUG << "    current vertex: " << nv.name << " role: " << nv.role << "\n";

      if (DWG::hasVertex(nv, dwg)) {
        DEBUG << "    vertex already exists, only adding edge\n\n\n";
        DWG::vDesc dwgv2 = DWG::getVertex(nv, dwg);
        DWG::addEdge(dwgv1, gp, dwgv2, dwg);
        continue;
      }

      DEBUG << "    vertex is new, adding vertex, edge and todo\n\n\n";
      DWG::vDesc dwgv2 = DWG::addVertex(dwg, nv, wg);
      DWG::addEdge(dwgv1, gp, dwgv2, dwg);

      dwgTodo.push_back(DWG::getVertex(dwgv2, dwg));


    } // label loop

    //DWG::print(dwg, wg);

    IF_DEBUG(
      DEBUG << "\nTodo:\n";
      for (const DWG::Vertex &v : dwgTodo)
        DEBUG << v.name << std::endl;
    )

    Util::printLineDebug();
    Util::printLineDebug();
    Util::printLineDebug();
  } // todo loop

  return dwg;
}

DWG::Vertex DWG::createVertex()
{
  DWG::Vertex nv;

  return nv;
}

DWG::Vertex DWG::createVertex(const std::string &name, const std::string &role)
{
  DWG::Vertex nv;

  nv.name = name;
  nv.role = role;

  return nv;
}

void DWG::addVertexToSet(DWG::Vertex &dwgv, const WG::vDesc wgv, const WG_t &wg)
{
  if (DWG::vertexHasVertex(dwgv, wgv))
    return;

  dwgv.vs.push_back(wgv);
  std::sort(dwgv.vs.begin(), dwgv.vs.end());
  DWG::updateVertexName(dwgv, wg);

  return;
}

void DWG::addVertexToSet(DWG_t &dwg, DWG::vDesc &dwgv, const WG::vDesc wgv, const WG_t & wg)
{
  (dwg[dwgv].vs).push_back(wgv);
  std::sort((dwg[dwgv].vs).begin(), (dwg[dwgv].vs).end());
  DWG::updateVertexName(dwgv, dwg, wg);

  return;
}

void DWG::updateVertexName(const DWG::vDesc &dwgv, DWG_t &dwg, const WG_t &wg)
{
  std::stringstream name;
  name << "{";

  std::vector<WG::vDesc>::iterator it;

  for (it = dwg[dwgv].vs.begin(); it != (dwg[dwgv].vs.end() - 1); ++it)
    name << "(" << wg[*it].name << "), ";

  name << "(" << wg[*it].name << ")";
  name << "}";

  dwg[dwgv].name = name.str();

  return;
}

void DWG::updateVertexName(DWG::Vertex &dwgv, const WG_t &wg)
{
  std::stringstream name;
  name << "{";

  std::vector<WG::vDesc>::iterator it;

  for (it = dwgv.vs.begin(); it != (dwgv.vs.end() - 1); ++it)
    name << "(" << wg[*it].name << "), ";

  name << "(" << wg[*it].name << ")";
  name << "}";

  dwgv.name = name.str();
}

DWG::vDesc DWG::addVertex(DWG_t &dwg, const DWG::Vertex &v, const WG_t &wg)
{
  DWG::vDesc nv = boost::add_vertex(dwg);

  dwg[nv].role = v.role;
  dwg[nv].vs   = v.vs;

  DWG::updateVertexName(nv, dwg, wg);

  return nv;
}

DWG::eDesc DWG::addEdge(DWG::vDesc &v1, const alignmentGrouping &gp, DWG::vDesc &v2, DWG_t &dwg)
{
  DWG::eDesc e = boost::add_edge(v1, v2, dwg).first;

  std::string name = Alm::groupingToStr(gp);

  dwg[e].name = name;
  dwg[e].gp = gp;

  return e;
}

DWG::Vertex DWG::getVertex(const DWG::vDesc &v, const DWG_t &dwg)
{
  DWG::Vertex nv;

  nv.name = dwg[v].name;
  nv.role = dwg[v].role;
  nv.vs   = dwg[v].vs;

  return nv;
}

DWG::vDesc DWG::getVertex(const DWG::Vertex &v, const DWG_t &dwg)
{
  DWG::vDesc nv;

  Range<DWG::vIter> vertices = Util::makeRange(boost::vertices(dwg));

  for (const DWG::vDesc &dwgv : vertices) {
    if (dwg[dwgv].name == v.name)
      nv = dwgv;
  }

  return nv;
}

DWG::vDesc DWG::getStart(const DWG_t &dwg)
{
  DWG::vDesc start;
  const Range<DWG::vIter> vertices = Util::makeRange(boost::vertices(dwg));

  for (const DWG::vDesc &v : vertices) {
    if (dwg[v].role == "start")
      start = v;
  }

  return start;
}


DWG::vDesc DWG::getDst(const DWG::vDesc &v, const alignmentGrouping &gp, const DWG_t &dwg)
{
  DWG::vDesc dst;

  Range<DWG::oeIter> oes = Util::makeRange(boost::out_edges(v, dwg));

  for (const DWG::eDesc &e : oes) {
    if (dwg[e].gp == gp) {
      dst = boost::target(e, dwg);
      break;
    }
  }

  return dst;
}

std::vector<WG::eDesc> DWG::getOutEdges(const DWG::Vertex &v, const WG_t &wg)
{
  std::vector<WG::eDesc> oedges;

  for (const WG::vDesc &wgv : v.vs) {
    Range<WG::oeIter> oes = Util::makeRange(boost::out_edges(wgv, wg));

    for (const WG::eDesc &e : oes)
      oedges.push_back(e);
  }

  return oedges;
}

std::vector<WG::eDesc> DWG::getOutEdges(const DWG_t & dwg, const WG_t &wg, const DWG::vDesc &v)
{
  std::vector<WG::eDesc> oedges;

  for (const WG::vDesc &wgv : dwg[v].vs) {
    Range<WG::oeIter> oes = Util::makeRange(boost::out_edges(wgv, wg));

    for (const WG::eDesc &e : oes)
      oedges.push_back(e);
  }

  return oedges;
}

bool DWG::hasVertex(const DWG::Vertex &v, const DWG_t &dwg)
{

  Range<DWG::vIter> vertices = Util::makeRange(boost::vertices(dwg));

  for (const DWG::vDesc &dwgv : vertices) {
    if (dwg[dwgv].name == v.name)
      return true;
  }

  return false;
}

bool DWG::vertexHasVertex(const DWG::Vertex &dwgv, const WG::vDesc &wgv)
{
  return (std::find(dwgv.vs.begin(), dwgv.vs.end(), wgv) != dwgv.vs.end());
}

DWG::vDesc DWG::addEmptyVertex(DWG_t &dwg, const edgeLabelSet &els)
{
  DWG::vDesc nv = boost::add_vertex(dwg);

  dwg[nv].name = "{}";
  dwg[nv].role = "empty";

  for (const alignmentGrouping &gp : els)
    DWG::addEdge(nv, gp, nv, dwg);

  return nv;
}


bool DWG::hasVertexThatContains(const WG::vDesc &v, const DWG_t &dwg)
{
  Range<DWG::vIter> vertices = Util::makeRange(boost::vertices(dwg));

  for (const DWG::vDesc &dwgv : vertices) {
    if (DWG::vertexHasVertex(DWG::getVertex(dwgv, dwg), v))
      return true;
  }

  return false;
}


DWG::vDesc DWG::getVertexThatContains(const WG::vDesc &wgv, const DWG_t &dwg)
{
  DWG::vDesc res;

  Range<DWG::vIter> vertices = Util::makeRange(boost::vertices(dwg));

  for (const DWG::vDesc &dwgv : vertices) {
    if (DWG::vertexHasVertex(DWG::getVertex(dwgv, dwg), wgv)) {
      res = dwgv;
      break;
    }
  }

  return res;
}

bool DWG::alreadyVisited(const std::vector<WG::vDesc> &vs, const WG::vDesc &t)
{
  return (std::find(vs.begin(), vs.end(), t) != vs.end());
}

bool DWG::alreadyContained(const DWG::Vertex &v, const WG::vDesc &nv)
{
  return (std::find(v.vs.begin(), v.vs.end(), nv) != v.vs.end());
}

bool DWG::destinationsAreSubset(const std::vector<WG::vDesc> dsts, const DWG::Vertex &v)
{
  for (const WG::vDesc &dst : dsts) {
    if (std::find(v.vs.begin(), v.vs.end(), dst) == v.vs.end())
      return false;
  }

  return true;
}

void DWG::findAllNodesLhs(DWG::Vertex &v, const WG_t &wg)
{
  // fill a queue with the vectors in v.vs and then iterate over them
  // we need the queue because inside the loop we may append new vectors
  // to v.vs, thereby invalidating any iterator.
  std::deque<WG::vDesc> vectors;
  for (const auto &start : v.vs) {
    vectors.push_back(start);
  }

  while(!vectors.empty())
  {
    auto start = vectors.front();
    vectors.pop_front();

    DEBUG << "[findAllNodes] start at " << wg[start].name << "\n";

    std::deque<WG::vDesc> todo;
    std::vector<WG::vDesc> visited;

    todo.push_back(start);

    while (!todo.empty()) {

      WG::vDesc curr = todo.front();
      todo.pop_front();
      visited.push_back(curr);

      DEBUG << "  [findAllNodes] checking node " << wg[curr].name << " (" << curr << ")\n";

      Range<WG::oeIter> oedges = Util::makeRange(boost::out_edges(curr, wg));

      if (oedges.empty()) {
        DEBUG << "  [findAllNodes] no out edges, done.\n";
        continue;
      }

      DEBUG << "  [findAllNodes] out edges:\n";
      for (const WG::eDesc &e : oedges)
        WG::printOutEdgeDebug(wg, e);

      if (!WG::hasEmptyTransitionLhs(oedges, wg)) {
        DEBUG << "  [findAllNodes] no empty edge\n";
        continue;
      }

      std::vector<WG::eDesc> empties = WG::getEmptyEdgesLhs(oedges, wg);
      DEBUG << "  [findAllNodes] empty edges:\n";
      for (const WG::eDesc &e : empties)
        WG::printOutEdgeDebug(wg, e);

      for (const WG::eDesc &e : empties) {
        WG::vDesc nv = boost::target(e, wg);
        DEBUG << "    [findAllNodes] checking dst " << wg[nv].name << "\n";

        if (DWG::alreadyVisited(visited, nv)) {
          DEBUG << "    [findAllNodes] already visited\n";
          continue;
        }

        if (DWG::vertexHasVertex(v, nv)) {
          DEBUG << "    [findAllNodes] already contained\n";
          continue;
        }

        DEBUG << "    [findAllNodes] not visited, not contained -> adding node " << wg[nv].name << "\n";

        DWG::addVertexToSet(v, nv, wg);
        todo.push_back(nv);
      }
    }
  }

  DEBUG << "[findAllNodes] done.\n\n";

  return;
}

void DWG::findAllNodesRhs(DWG::Vertex &v, const WG_t &wg)
{
  // fill a queue with the vectors in v.vs and then iterate over them
  // we need the queue because inside the loop we may append new vectors
  // to v.vs, thereby invalidating any iterator.
  std::deque<WG::vDesc> vectors;
  for (const auto &start : v.vs) {
    vectors.push_back(start);
  }

  while(!vectors.empty())
  {
    auto start = vectors.front();
    vectors.pop_front();

    DEBUG << "[findAllNodes] start at " << wg[start].name << "\n";

    std::deque<WG::vDesc> todo;
    std::vector<WG::vDesc> visited;

    todo.push_back(start);

    while (!todo.empty()) {

      WG::vDesc curr = todo.front();
      todo.pop_front();
      visited.push_back(curr);

      DEBUG << "  [findAllNodes] checking node " << wg[curr].name << " (" << curr << ")\n";

      Range<WG::oeIter> oedges = Util::makeRange(boost::out_edges(curr, wg));

      if (oedges.empty()) {
        DEBUG << "  [findAllNodes] no out edges, done.\n";
        continue;
      }

      IF_DEBUG(
        DEBUG << "  [findAllNodes] out edges:\n";
        for (const WG::eDesc &e : oedges)
          WG::printOutEdgeDebug(wg, e);
      );

      if (!WG::hasEmptyTransitionRhs(oedges, wg)) {
        DEBUG << "  [findAllNodes] no empty edge\n";
        continue;
      }

      std::vector<WG::eDesc> empties = WG::getEmptyEdgesRhs(oedges, wg);
      DEBUG << "  [findAllNodes] empty edges:\n";
      for (const WG::eDesc &e : empties)
        WG::printOutEdgeDebug(wg, e);

      for (const WG::eDesc &e : empties) {
        WG::vDesc nv = boost::target(e, wg);
        DEBUG << "    [findAllNodes] checking dst " << wg[nv].name << "\n";

        if (DWG::alreadyVisited(visited, nv)) {
          DEBUG << "    [findAllNodes] already visited\n";
          continue;
        }

        if (DWG::vertexHasVertex(v, nv)) {
          DEBUG << "    [findAllNodes] already contained\n";
          continue;
        }

        DEBUG << "    [findAllNodes] not visited, not contained -> adding node " << wg[nv].name << "\n";

        DWG::addVertexToSet(v, nv, wg);
        todo.push_back(nv);
      }
    }
  }

  DEBUG << "[findAllNodes] done.\n\n";

  return;
}

void DWG::setFinalState(DWG::Vertex &v, const WG_t &wg)
{
  for (const WG::vDesc &vd : v.vs) {
    if (wg[vd].role == "end") {
      v.role = "end";
      return;
    }
  }

  return;
}






























void DWG::printOutEdge(const DWG::eDesc &e, const DWG_t &dwg)
{
  const DWG::vDesc src = boost::source(e, dwg);
  const DWG::vDesc dst = boost::target(e, dwg);

  std::string gp = Alm::groupingToStr(dwg[e].gp);

  std::stringstream label;

  label << "{";

  if (gp == "")
    label << "-";
  else
    label << gp;

  label << "}";

  std::cout << "  \"" << dwg[src].name << "\" ->  \"" << dwg[dst].name << "\"";
  std::cout << " [label=\"" << label.str() << "\", gp=\"" << gp << "\"]" << std::endl;

  return;
}


void DWG::printOutEdges(const DWG::vDesc &v, const DWG_t &dwg)
{
  Range<DWG::oeIter> oedges = Util::makeRange(boost::out_edges(v, dwg));

  if (oedges.empty())
    return;

  for (const DWG::eDesc &e : oedges)
    printOutEdge(e, dwg);

  Util::printLine();

  return;

}

void DWG::print(const DWG_t &dwg)
{

  std::cout << "digraph {" << std::endl;

  Range<WG::vIter> vertices = Util::makeRange(boost::vertices(dwg));

  for (const DWG::vDesc &v : vertices) {

    if (dwg[v].role == "start") {
      std::cout << "  \"" << dwg[v].name << "\" [role=\"start\"]" << std::endl;
      continue;
    }

    if (dwg[v].role == "end") {
      std::cout << "  \"" << dwg[v].name << "\" [role=\"end\"]" << std::endl;
      continue;
    }
  }

  Util::printLine();

  for (const WG::vDesc &v : vertices)
    DWG::printOutEdges(v, dwg);

  std::cout << "}" << std::endl;

  return;
}
