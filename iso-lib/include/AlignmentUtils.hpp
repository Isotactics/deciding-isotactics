#ifndef __ALIGNMENTUTILS_HPP__
#define __ALIGNMENTUTILS_HPP__

#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "json.hpp"
#include "Utils.hpp"

using json = nlohmann::json;


using label             = std::string;
using alignmentGroup    = std::vector<label>;
using alignmentGrouping = std::vector<alignmentGroup>;
using alignmentPair     = std::pair<alignmentGroup, alignmentGroup>;
using alignmentHalf     = alignmentGrouping;
using alignment         = std::vector<alignmentPair>;
using alignmentSub      = alignment;

using almMap = std::map<alignmentGroup, alignmentGroup>;



namespace Alm {
  alignment parse(const std::string &path);

  alignmentGroup getGroup(json::array_t a);
  alignmentPair getPair(json::object_t o);

  alignmentGrouping Lhs(const alignment &alm);
  alignmentGrouping Rhs(const alignment &alm);

  almMap AlmMap(const alignment &alm);


  bool hasLabel(const alignmentGroup &g, const label &l);
  bool hasPair(const alignment &alm, const alignmentPair &p);
  bool groupEqual(const alignmentGroup &g1, const alignmentGroup &g2);
  bool groupingEqual(const alignmentGrouping &gp1, const alignmentGrouping &gp2);


  label getLabelFromGrouping(const alignmentGrouping &gp);
  alignmentGrouping getGrouping(const label &l, const alignmentHalf &alh);

  int getPermissiveness(const alignment &alm);




  std::string groupToStr(const alignmentGroup &g);
  std::string groupingToStr(const alignmentGrouping &gp);

  void print(const alignment &a);
  void printPair(const alignmentPair &p);
  void printGrouping(const alignmentGrouping &gp);
  void printGroup(const alignmentGroup &g);

  void printAlmMap(const almMap &m);

}

#endif // __ALIGNMENTUTILS_HPP__
