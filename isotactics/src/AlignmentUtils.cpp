#include <algorithm>
#include <fstream>
#include <iostream>

#include "AlignmentUtils.h"


using json = nlohmann::json;

using label             = std::string;
using alignmentGroup    = std::vector<label>;
using alignmentGrouping = std::vector<alignmentGroup>;
using alignmentPair     = std::pair<alignmentGroup, alignmentGroup>;
using alignment         = std::vector<alignmentPair>;




alignment parseAlignment(const std::string &path)
{
  std::ifstream alignmentFile(path, std::ifstream::binary);

  json j;
  alignmentFile >> j;

  alignment al;
  alignmentPair p;

  for (const json::object_t &o : j["alignment"]) {
    p = objectToAlignPair(o);
    al.push_back(p);
  }

  return al;
}

alignmentPair objectToAlignPair(json::object_t o)
{
  alignmentPair p;
  p.first  = arrayToAlignGroup(o["lhs"]);
  p.second = arrayToAlignGroup(o["rhs"]);

  return p;
}

alignmentGroup arrayToAlignGroup(json::array_t a)
{
  alignmentGroup g;

  for (const auto &e : a)
    g.push_back(e);

  return g;
}




bool groupContainsLabel(const label &l, const alignmentGroup &g)
{
  alignmentGroup::const_iterator it;

  it = std::find(g.begin(), g.end(), l);

  if (it == g.end())
    return false;

  return true;
}

alignmentGrouping getLabelGrouping(const label &l, const alignmentGrouping &gp)
{
  alignmentGrouping res;

  for (const alignmentGroup &g : gp) {
    if (groupContainsLabel(l, g))
      res.push_back(g);
  }

  return res;
}




alignmentGrouping getAlignmentLhs(const alignment &al)
{
  alignmentGrouping gp;

  for (const alignmentPair &p : al)
    gp.push_back(p.first);

  return gp;
}

alignmentGrouping getAlignmentRhs(const alignment &al)
{
  alignmentGrouping gp;

  for (const alignmentPair &p : al)
    gp.push_back(p.second);

  return gp;
}




void printAlignGroup(const alignmentGroup &g)
{
  for (const label &l : g)
    std::cout << l << " ";

  return;
}

void printAlignGrouping(const alignmentGrouping &gp)
{
  for (const alignmentGroup &g : gp) {
    printAlignGroup(g);
    std::cout << std::endl;
  }

  return;
}

void printAlignPair(const alignmentPair &p)
{
  printAlignGroup(p.first);
  std::cout << "\t<> ";
  printAlignGroup(p.second);

  std::cout << std::endl;

  return;
}

void printAlignment(const alignment &a)
{
  for (const alignmentPair &p : a)
    printAlignPair(p);

  return;
}
