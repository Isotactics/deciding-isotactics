#include <sstream>
#include <string>

#include "MatchUtils.hpp"
#include "WitnessUtils.hpp"




WG::vDesc createStart(WG_t &wg, const DG_t &g1, const DG_t &g2)
{
  matchSet ms;
  WG::vDesc wgInit;

  DG::vDesc gs1 = DG::getStart(g1);
  DG::vDesc gs2 = DG::getStart(g2);

  WG::Vertex init = WG::createVertex(g1[gs1].name, g2[gs2].name, ms);
  init.role = "start";

  wgInit = WG::addVertex(init, wg);

  return wgInit;
}

WG_t WG::create(const DG_t &g1, const DG_t &g2, labelGroupingMap &lgm1,
                labelGroupingMap &lgm2, const alignment &alm)
{
  WG_t wg;
  matchSet ms, msNew;

  label l1, l2;

  WG::vDesc wgv1, wgv2;

  DG::vDesc gv1, gv2;
  DG::vDesc dst1, dst2;

  Range<DG::oeIter> oe1, oe2;

  WG::Vertex init, currentV;
  std::deque<WG::vDesc> wgTodo;



  wgv1 = createStart(wg, g1, g2);

  wgTodo.push_back(wgv1);


  while (!wgTodo.empty()) {

    wgv1 = wgTodo.front();
    wgTodo.pop_front();

    gv1 = DG::getVertexByName(g1, wg[wgv1].v1Name);
    gv2 = DG::getVertexByName(g2, wg[wgv1].v2Name);

    oe1 = Util::makeRange(boost::out_edges(gv1, g1));
    oe2 = Util::makeRange(boost::out_edges(gv2, g2));

    ms = wg[wgv1].ms;

    currentV = WG::createVertex(wg[wgv1].v1Name, wg[wgv1].v2Name, ms);
    std::cerr << "working on: " << currentV.name << std::endl;

    if ((g1[gv1].role == "end") && (g2[gv2].role == "end"))
      wg[wgv1].role = "end";


    std::cerr << "  checking rule 1:" << std::endl;

    for (const DG::eDesc &e1 : oe1) {

      l1 = g1[e1].label;

      for (const WG::eDesc &e2 : oe2) {
        l2 = g2[e2].label;

        msNew = Match::getMatchSet(alm, ms, l1, l2);

        if (msNew.empty())
          continue;

        std::cerr << "    found new match set" << std::endl;
        std::cerr << "    " << l1 << ", " << l2 << ", " << Match::setToString(msNew) << std::endl;

        dst1 = boost::target(e1, g1);
        dst2 = boost::target(e2, g2);

        WG::Vertex newV = WG::createVertex(g1[dst1].name, g2[dst2].name, msNew);
        std::cerr << "    new Vertex: " << newV.name << std::endl;

        if (WG::hasVertex(newV, wg)) {
          std::cerr << "    vertex already exists. adding edge, not todo" << std::endl << std::endl;
          wgv2 = WG::getVertex(newV, wg);
          WG::addEdge(wgv1, lgm1[l1], lgm2[l2], wgv2, wg);
          continue;
        }

        std::cerr << "    new vertex doesn't exist. add vertex, add edge, add todo" << std::endl << std::endl;

        wgv2 = WG::addVertex(newV, wg);
        WG::addEdge(wgv1, lgm1[l1], lgm2[l2], wgv2, wg);

        wgTodo.push_back(wgv2);
      }
    }




    std::cerr << "  checking rule 2:" << std::endl;
    std::cerr << "    g1 moves:" << std::endl;


    for (const WG::eDesc &e1 : oe1) {
          l1 = g1[e1].label;

          msNew = Match::getMatchSet2(ms, l1);

          if (msNew.empty())
            continue;

      std::cerr << "    found new match set" << std::endl;
      std::cerr << "    " << l1 << ", " << Match::setToString(msNew) << std::endl;

      dst1 = boost::target(e1, g1);
      dst2 = gv2;

      alignmentGrouping gp2;

      WG::Vertex newV = WG::createVertex(g1[dst1].name, g2[dst2].name, msNew);
      std::cerr << "    new Vertex: " << newV.name << std::endl << std::endl;

      if (WG::vertexEqual(currentV, newV)) {
        std::cerr << "   i,m the new vertex, only adding edge to myself " << std::endl;
        wgv2 = wgv1;
        WG::addEdge(wgv1, lgm1[l1], gp2, wgv2, wg);
        continue;
      }

      if (WG::hasVertex(newV, wg)) {
        std::cerr << "   new vertex already exists. adding edge, not todo " << std::endl;
        wgv2 = WG::getVertex(newV, wg);
        WG::addEdge(wgv1, lgm1[l1], gp2, wgv2, wg);
        continue;
      }

      std::cerr << "   new vertex doesn't exist. add node, add edge, add todo! " << std::endl;

      wgv2 = WG::addVertex(newV, wg);
      WG::addEdge(wgv1, lgm1[l1], gp2, wgv2, wg);
      wgTodo.push_back(wgv2);
    }




    std::cerr << "  checking rule 2:" << std::endl;
    std::cerr << "    g2 moves:" << std::endl;


    for (const WG::eDesc &e2 : oe2) {
          l2 = g2[e2].label;

          msNew = Match::getMatchSet2(ms, l2);

          if (msNew.empty())
            continue;

      std::cerr << "    found new match set" << std::endl;
      std::cerr << "    " << l2 << ", " << Match::setToString(msNew) << std::endl;

      dst1 = gv1;
      dst2 = boost::target(e2, g2);

      alignmentGrouping gp1;

      WG::Vertex newV = WG::createVertex(g1[dst1].name, g2[dst2].name, msNew);
      std::cerr << "    new Vertex: " << newV.name << std::endl << std::endl;

      if (WG::vertexEqual(currentV, newV)) {
        std::cerr << "   i,m the new vertex, only adding edge to myself " << std::endl;
        wgv2 = wgv1;
        WG::addEdge(wgv1, gp1, lgm2[l2], wgv2, wg);
        continue;
      }

      if (WG::hasVertex(newV, wg)) {
        std::cerr << "   new vertex already exists. adding edge, not todo " << std::endl;
        wgv2 = WG::getVertex(newV, wg);
        WG::addEdge(wgv1, gp1, lgm2[l2], wgv2, wg);
        continue;
      }

      std::cerr << "   new vertex doesn't exist. add node, add edge, add todo! " << std::endl;

      wgv2 = WG::addVertex(newV, wg);
      WG::addEdge(wgv1, gp1, lgm2[l2], wgv2, wg);
      wgTodo.push_back(wgv2);
    }


    Util::printLineDebug();
    WG::printDebug(wg);
    Util::printLineDebug();


    std::cerr << "Todo:" << std::endl;

    for (const WG::vDesc &v : wgTodo)
      std::cerr << "  " << wg[v].name << std::endl;

    Util::printLineDebug();
    Util::printLineDebug();
    Util::printLineDebug();
  }


  return wg;
}

WG::Vertex WG::createVertex(const vName &v1, const vName &v2, const matchSet &ms)
{
  WG::Vertex v;

  v.v1Name = v1;
  v.v2Name = v2;
  v.ms     = ms;

  v.name = WG::getVertexName(v);

  return v;
}

WG::vDesc WG::getVertex(const WG::Vertex &v, const WG_t &wg)
{
  WG::vDesc nv;

  Range<WG::vIter> vertices = Util::makeRange(boost::vertices(wg));

  for (const WG::vDesc &wgv : vertices) {
    if (wg[wgv].name == v.name)
      nv = wgv;
  }

  return nv;
}

WG::vDesc WG::addVertex(const WG::Vertex &v, WG_t &wg)
{
  WG::vDesc nv = boost::add_vertex(wg);

  wg[nv].name   = v.name;
  wg[nv].v1Name = v.v1Name;
  wg[nv].v2Name = v.v2Name;
  wg[nv].ms     = v.ms;
  wg[nv].role   = v.role;

  return nv;
}

WG::eDesc WG::addEdge(WG::vDesc &v1, const alignmentGrouping &gp1,
                    const alignmentGrouping &gp2, WG::vDesc &v2, WG_t &wg)
{
  WG::eDesc e = boost::add_edge(v1, v2, wg).first;

  std::stringstream name;
  name << Alm::groupingToStr(gp1) << " | " << Alm::groupingToStr(gp2);

  wg[e].name = name.str();
  wg[e].gp1  = gp1;
  wg[e].gp2  = gp2;

  return e;
}

std::string WG::getVertexName(const WG::Vertex &v)
{
  std::stringstream name;

  name << "" << v.v1Name << ", " << v.v2Name << ", ";
  name << Match::setToString(v.ms) << "";

  return name.str();
}

WG::vDesc WG::getStart(const WG_t &wg)
{
  WG::vDesc start;
  const Range<WG::vIter> vertices = Util::makeRange(boost::vertices(wg));

  for (const WG::vDesc &v : vertices) {
    if (wg[v].role == "start")
      start = v;
  }

  return start;
}

bool WG::hasVertex(const WG::Vertex &v, const WG_t &wg)
{
  Range<WG::vIter> vertices = Util::makeRange(boost::vertices(wg));

  for (const WG::vDesc &wgv : vertices) {
    if (wg[wgv].name == v.name)
      return true;
  }

  return false;
}

bool WG::vertexEqual(const WG::Vertex &v1, const WG::Vertex &v2)
{
  if (v1.name == v2.name)
    return true;

  return false;
}

bool WG::hasEdgeLhs(const std::vector<WG::eDesc> &oes, const alignmentGrouping &gp, const WG_t &wg)
{
  for (const WG::eDesc &e : oes) {
    if (Alm::groupingEqual(wg[e].gp1, gp))
      return true;
  }

  return false;
}

bool WG::hasEdgeRhs(const std::vector<WG::eDesc> &oes, const alignmentGrouping &gp, const WG_t &wg)
{
  for (const WG::eDesc &e : oes) {
    if (Alm::groupingEqual(wg[e].gp2, gp))
      return true;
  }

  return false;
}

bool WG::hasEmptyTransitionLhs(const std::vector<WG::eDesc> &oes, const WG_t &wg)
{
  for (const WG::eDesc &e : oes) {
    if (wg[e].gp1.empty())
      return true;
  }

  return false;
}

bool WG::hasEmptyTransitionRhs(const std::vector<WG::eDesc> &oes, const WG_t &wg)
{
  for (const WG::eDesc &e : oes) {
    if (wg[e].gp2.empty())
      return true;
  }

  return false;
}

bool WG::hasEmptyTransitionLhs(const Range<WG::oeIter> &oes, const WG_t &wg)
{
  for (const WG::eDesc &e : oes) {
    if (wg[e].gp1.empty())
      return true;
  }

  return false;
}

bool WG::hasEmptyTransitionRhs(const Range<WG::oeIter> &oes, const WG_t &wg)
{
  for (const WG::eDesc &e : oes) {
    if (wg[e].gp2.empty())
      return true;
  }

  return false;
}

std::vector<WG::eDesc> WG::getEdgesWithLabelLhs(const std::vector<WG::eDesc> &oes,
                                                const alignmentGrouping &gp,
                                                const WG_t &wg)
{
  std::vector<WG::eDesc> edges;

  for (const WG::eDesc &e : oes) {
    if (Alm::groupingEqual(wg[e].gp1, gp))
      edges.push_back(e);
  }

  return edges;
}

std::vector<WG::eDesc> WG::getEdgesWithLabelRhs(const std::vector<WG::eDesc> &oes,
                                                const alignmentGrouping &gp,
                                                const WG_t &wg)
{
  std::vector<WG::eDesc> edges;

  for (const WG::eDesc &e : oes) {
    if (Alm::groupingEqual(wg[e].gp2, gp))
      edges.push_back(e);
  }

  return edges;
}


std::vector<WG::vDesc> WG::getDestinations(const std::vector<WG::eDesc> &es, const WG_t &wg)
{
  std::vector<WG::vDesc> dsts;

  for (const WG::eDesc &e : es)
    dsts.push_back(boost::target(e, wg));

  return dsts;
}

std::vector<WG::eDesc> WG::getEmptyEdgesLhs(const Range<WG::oeIter> &oes, const WG_t &wg)
{
  std::vector<WG::eDesc> eedges;

  for (const WG::eDesc &e : oes) {
    if (wg[e].gp1.empty())
      eedges.push_back(e);
  }

  return eedges;
}

std::vector<WG::eDesc> WG::getEmptyEdgesRhs(const Range<WG::oeIter> &oes, const WG_t &wg)
{
  std::vector<WG::eDesc> eedges;

  for (const WG::eDesc &e : oes) {
    if (wg[e].gp2.empty())
      eedges.push_back(e);
  }

  return eedges;
}












void WG::printOutEdge(const WG_t &wg, const WG::eDesc &e)
{
  const WG::vDesc src = boost::source(e, wg);
  const WG::vDesc dst = boost::target(e, wg);

  std::string gp1 = Alm::groupingToStr(wg[e].gp1);
  std::string gp2 = Alm::groupingToStr(wg[e].gp2);

  std::stringstream label;

  label << "{";

  if (gp1 == "")
    label << "-";
  else
    label << gp1;

  label << "}, {";

  if (gp2 == "")
    label << "-";
  else
    label << gp2;

  label << "}";

  std::cout << "  \"" << wg[src].name << "\" ->  \"" << wg[dst].name << "\"";
  std::cout << " [label=\"" << label.str() << "\", gp1=\"" << gp1 << "\", gp2=\"" << gp2 <<"\"]" << std::endl;

  return;
}

void WG::printOutEdges(const WG_t &wg, const WG::vDesc &v)
{
  Range<WG::oeIter> oedges = Util::makeRange(boost::out_edges(v, wg));

  if (oedges.empty())
    return;

  for (const WG::eDesc &e : oedges)
    printOutEdge(wg, e);

  Util::printLine();

  return;
}

void WG::print(const WG_t &wg)
{
  std::cout << "digraph {" << std::endl;

  Range<WG::vIter> vertices = Util::makeRange(boost::vertices(wg));

  for (const WG::vDesc &v : vertices) {

    if (wg[v].role == "start") {
      std::cout << "  \"" << wg[v].name << "\" [role=\"start\"]" << std::endl;
      continue;
    }

    if (wg[v].role == "end") {
      std::cout << "  \"" << wg[v].name << "\" [role=\"end\"]" << std::endl;
      continue;
    }
  }

  Util::printLine();

  for (const WG::vDesc &v : vertices)
    WG::printOutEdges(wg, v);


  std::cout << "}" << std::endl;

  return;
}


void WG::printOutEdgeDebug(const WG_t &wg, const WG::eDesc &e)
{
  const WG::vDesc src = boost::source(e, wg);
  const WG::vDesc dst = boost::target(e, wg);

  std::string gp1 = Alm::groupingToStr(wg[e].gp1);
  std::string gp2 = Alm::groupingToStr(wg[e].gp2);

  std::stringstream label;

  label << "{";

  if (gp1 == "")
    label << "-";
  else
    label << gp1;

  label << "}, {";

  if (gp2 == "")
    label << "-";
  else
    label << gp2;

  label << "}";

  std::cerr << "  \"" << wg[src].name << "\" ->  \"" << wg[dst].name << "\"";
  std::cerr << " [label=\"" << label.str() << "\", gp1=\"" << gp1 << "\", gp2=\"" << gp2 <<"\"]" << std::endl;

  return;
}

void WG::printOutEdgesDebug(const WG_t &wg, const WG::vDesc &v)
{
  Range<WG::oeIter> oedges = Util::makeRange(boost::out_edges(v, wg));

  if (oedges.empty())
    return;

  for (const WG::eDesc &e : oedges)
    printOutEdgeDebug(wg, e);

  Util::printLineDebug();

  return;
}

void WG::printDebug(const WG_t &wg)
{
  std::cerr << "digraph {" << std::endl;

  Range<WG::vIter> vertices = Util::makeRange(boost::vertices(wg));

/*
  for (const WG::vDesc &v : vertices) {
    attributes...

  }
*/

  for (const WG::vDesc &v : vertices)
    printOutEdgesDebug(wg, v);


  std::cerr << "}" << std::endl;

  return;
}
