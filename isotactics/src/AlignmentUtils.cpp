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

alignmentPair Alm::getPair(json::object_t o)
{
  alignmentPair p;
  p.first  = Alm::getGroup(o["lhs"]);
  p.second = Alm::getGroup(o["rhs"]);

  return p;
}

alignmentGroup Alm::getGroup(json::array_t a)
{
  alignmentGroup g;

  for (const label &l : a)
    g.push_back(l);

  return g;
}

bool Alm::hasLabel(const alignmentGroup &g, const label &l)
{
  alignmentGroup::const_iterator it;

  it = std::find(g.begin(), g.end(), l);

  if (it == g.end())
    return false;

  return true;
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


bool Alm::hasPair(const alignment &alm, const alignmentPair &p)
{
  for (const alignmentPair &ap : alm) {
    if (p == ap)
      return true;
  }

  return false;
}

alignmentGrouping Alm::getLhs(const alignment &alm)
{
  alignmentGrouping gp;

  for (const alignmentPair &p : alm)
    gp.push_back(p.first);

  return gp;
}

alignmentGrouping Alm::getRhs(const alignment &alm)
{
  alignmentGrouping gp;

  for (const alignmentPair &p : alm)
    gp.push_back(p.second);

  return gp;
}

almMap Alm::getAlmMap(const alignment &alm)
{
  almMap m;

  for (const alignmentPair &p : alm)
    m[p.first] = p.second;

  return m;
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


