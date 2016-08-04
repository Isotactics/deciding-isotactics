#include <sstream>
#include <boost/algorithm/string/join.hpp>

#include "WitnessUtils.hpp"


WG::vDesc WG::addVertex(const vName &v1, const vName &v2, const matchSet &ms, WG_t &wg)
{
  WG::vDesc v = boost::add_vertex(wg);

  std::string mStr = WG::matchSetToStr(ms);

  std::stringstream name;
  name << v1 << "," << v2 << "," << mStr;

  wg[v].name   = name.str();
  wg[v].v1Name = v1;
  wg[v].v2Name = v2;
  wg[v].ms     = ms;

  return v;
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

bool WG::hasMatch(const matchSet &ms, const match &ma)
{
  for (const match &m : ms) {
    if (m == ma)
      return true;
  }

  return false;
}

bool WG::matchEmpty(const match &m)
{
  return (m.first).empty();
}

bool WG::matchSetEmpty(const matchSet &ms)
{
  return ms.empty();
}

match WG::getMatch(const alignment &alm, const label &l1, const label &l2)
{
  match m;
  matchSet res;

  for (const alignmentPair &p : alm) {
    if ((Alm::hasLabel(p.first, l1)) && (Alm::hasLabel(p.second, l2)))
      res.push_back(p);
  }

  if (res.empty())
    return m;

  return res[0];
}

matchSet WG::getMatchSet(const alignment &alm, const matchSet &ms,
                                          const label &l1, const label &l2)
{
  matchSet res;

  match m = WG::getMatch(alm, l1, l2);

  if (WG::matchEmpty(m))
    return res;

  if (!WG::hasMatch(ms, m))
    res.push_back(m);

  return res;
}

std::string WG::matchToStr(const match &m)
{
  std::stringstream res;

  res << "(";
  res << boost::algorithm::join(m.first, ",");
  res << " <> ";
  res << boost::algorithm::join(m.second, ",");
  res << ")";

  return res.str();
}

std::string WG::matchSetToStr(const matchSet &ms)
{
  std::stringstream res;
  matchSet::const_iterator it;

  if (ms.empty()) {
    res << "{}";
    return res.str();
  }

  res << "{";

  for (it = ms.begin(); it != ms.end() - 1; ++it) {
    res << WG::matchToStr(*it);
    res << ",";
  }

  res << WG::matchToStr(*it);
  res << "}";

  return res.str();
}

void WG::printMatch(const match &m)
{
  Alm::printGroup(m.first);
  std::cout << "\t<> ";
  Alm::printGroup(m.second);

  std::cout << std::endl;

  return;
}

void WG::printMatchSet(const matchSet &ms)
{
  for (const match &m : ms) {
    std::cout << "  ";
    WG::printMatch(m);
  }

  return;
}

void WG::printOutEdge(const WG_t &wg, const WG::eDesc &e)
{
  const WG::vDesc src = boost::source(e, wg);
  const WG::vDesc dst = boost::target(e, wg);

  std::string gp1 = Alm::groupingToStr(wg[e].gp1);
  std::string gp2 = Alm::groupingToStr(wg[e].gp2);


  std::cout << "  " << wg[src].name << " -> " << wg[dst].name;
  std::cout << " [gp1=\"" << gp1 << "\", gp2=\"" << gp2 <<"\"]" << std::endl;

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
