#include <sstream>
#include <string>

#include "WitnessUtils.hpp"



WG::Vertex WG::createVertex(const vName &v1, const vName &v2, const matchSet &ms)
{
  WG::Vertex v;

  v.v1Name = v1;
  v.v2Name = v2;
  v.ms     = ms;

  v.name = WG::getVertexName(v);

  return v;
}

std::string WG::getVertexName(const WG::Vertex &v)
{
  std::stringstream name;

  name << "" << v.v1Name << ", " << v.v2Name << ", ";
  name << Match::setToString(v.ms) << "";

  return name.str();
}

WG::vDesc WG::addVertex(const WG::Vertex &v, WG_t &wg)
{
  WG::vDesc nv = boost::add_vertex(wg);

  wg[nv].name   = v.name;
  wg[nv].v1Name = v.v1Name;
  wg[nv].v2Name = v.v2Name;
  wg[nv].ms     = v.ms;

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

/*
  for (const WG::vDesc &v : vertices) {
    attributes...

  }
*/

  for (const WG::vDesc &v : vertices)
    printOutEdges(wg, v);


  std::cout << "}" << std::endl;

  return;
}
