#include <algorithm>
#include <fstream>
#include <iostream>

#include <boost/algorithm/string/join.hpp>

#include "AlignmentUtils.hpp"
#include "Utils.hpp"



alignment Alm::parse(const std::string &path)
{
  std::ifstream alignmentFile(path, std::ifstream::binary);

  json j;
  alignmentFile >> j;

  alignment al;
  alignmentPair p;

  for (const json::object_t &o : j["alignment"]) {
    p = Alm::getPair(o);
    al.push_back(p);
  }

  return al;
}

alignmentGroup Alm::getGroup(json::array_t a)
{
  alignmentGroup g;

  for (const label &l : a)
    g.push_back(l);

  return g;
}

alignmentPair Alm::getPair(json::object_t o)
{
  alignmentPair p;
  p.first  = Alm::getGroup(o["lhs"]);
  p.second = Alm::getGroup(o["rhs"]);

  return p;
}

alignmentGrouping Alm::Lhs(const alignment &alm)
{
  alignmentGrouping gp;

  for (const alignmentPair &p : alm)
    gp.push_back(p.first);

  return gp;
}

alignmentGrouping Alm::Rhs(const alignment &alm)
{
  alignmentGrouping gp;

  for (const alignmentPair &p : alm)
    gp.push_back(p.second);

  return gp;
}

almMap Alm::AlmMap(const alignment &alm)
{
  almMap m;

  for (const alignmentPair &p : alm)
    m[p.first] = p.second;

  return m;
}

labelGroupingMap Alm::LabelGroupingMap(const Graph_t &g, const alignmentHalf &alh)
{
  labelGroupingMap lgm;

  const Range<Graph::eIter> edges = Util::makeRange(boost::edges(g));

  label l;
  alignmentGrouping gp;

  for (const Graph::eDesc &e : edges) {
    l = g[e].label;
    gp = Alm::getGrouping(l, alh);

    lgm[l] = gp;
  }

  return lgm;
}

labelAlmSubMap Alm::LabelAlmSubMap(const Graph_t &g, const alignment &alm)
{
  labelAlmSubMap lsm;

  const Range<Graph::eIter> edges = Util::makeRange(boost::edges(g));

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

bool Alm::hasLabel(const alignmentGroup &g, const label &l)
{
  alignmentGroup::const_iterator it;

  it = std::find(g.begin(), g.end(), l);

  if (it == g.end())
    return false;

  return true;
}

bool Alm::hasPair(const alignment &alm, const alignmentPair &p)
{
  for (const alignmentPair &ap : alm) {
    if (p == ap)
      return true;
  }

  return false;
}

alignmentGrouping Alm::getGrouping(const label &l, const alignmentHalf &alh)
{
  alignmentGrouping res;

  for (const alignmentGroup &g : alh) {
    if (Alm::hasLabel(g, l))
      res.push_back(g);
  }

  return res;
}

std::string Alm::groupToStr(const alignmentGroup &g)
{
  std::string res = boost::algorithm::join(g, ",");

  return res;
}

std::string Alm::groupingToStr(const alignmentGrouping &gp)
{
  std::vector<std::string> tmp;

  for (const alignmentGroup &g : gp)
    tmp.push_back(groupToStr(g));

  std::string res = boost::algorithm::join(tmp, " | ");

  return res;
}














void Alm::print(const alignment &alm)
{
  for (const alignmentPair &p : alm) {
    std::cout << "  ";
    Alm::printPair(p);
  }

  return;
}

void Alm::printPair(const alignmentPair &p)
{
  Alm::printGroup(p.first);
  std::cout << "\t<> ";
  Alm::printGroup(p.second);

  std::cout << std::endl;

  return;
}

void Alm::printGrouping(const alignmentGrouping &gp)
{
  alignmentGrouping::const_iterator it;

  for (it = gp.begin(); it != (gp.end() - 1); ++it) {
    Alm::printGroup(*it);
    std::cout << " | ";
  }

  Alm::printGroup(*it);

  return;
}

void Alm::printGroup(const alignmentGroup &g)
{
  alignmentGroup::const_iterator it;

  for (it = g.begin(); it != (g.end() - 1); ++it)
    std::cout << *it << ", ";

  std::cout << *it;


  return;
}

void Alm::printAlmMap(const almMap &m)
{
  for (const alignmentPair &p : m) {
    Alm::printGroup(p.first);
    std::cout << "\t-> ";
    Alm::printGroup(p.second);
    Util::printLine();
  }

  return;
}

void Alm::printLgm(const labelGroupingMap &lgm)
{
  for (const std::pair<label, alignmentGrouping> &p : lgm) {
    std::cout << p.first << " ->  ";
    Alm::printGrouping(p.second);
    Util::printLine();
  }

  return;
}

void Alm::printLsm(const labelAlmSubMap &lsm)
{
  for (const std::pair<label, alignmentSub> &p : lsm) {
    std::cout << p.first << " -> " << std::endl;
    Alm::print(p.second);
    Util::printLine();
  }

  return;
}
