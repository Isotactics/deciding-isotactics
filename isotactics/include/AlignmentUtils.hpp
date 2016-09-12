#ifndef __ALIGNMENTUTILS_HPP__
#define __ALIGNMENTUTILS_HPP__

#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "json.hpp"
#include "GraphUtils.hpp"
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

using labelGroupingMap = std::unordered_map<label, alignmentGrouping>;
using labelAlmSubMap = std::unordered_map<label, alignmentSub>;

using edgeLabelSet = std::set<alignmentGrouping>;


namespace Alm {
  alignment parse(const std::string &path);

  alignmentGroup getGroup(json::array_t a);
  alignmentPair getPair(json::object_t o);

  alignmentGrouping Lhs(const alignment &alm);
  alignmentGrouping Rhs(const alignment &alm);

  almMap AlmMap(const alignment &alm);
  labelGroupingMap LabelGroupingMap(const Graph_t &g, const alignmentHalf &alh);
  labelAlmSubMap LabelAlmSubMap(const Graph_t &g, const alignment &alm);

  edgeLabelSet lgmFlatten(const labelGroupingMap &lgm);



  bool hasLabel(const alignmentGroup &g, const label &l);
  bool hasPair(const alignment &alm, const alignmentPair &p);

  alignmentGrouping getGrouping(const label &l, const alignmentHalf &alh);

  std::string groupToStr(const alignmentGroup &g);
  std::string groupingToStr(const alignmentGrouping &gp);

  void print(const alignment &a);
  void printPair(const alignmentPair &p);
  void printGrouping(const alignmentGrouping &gp);
  void printGroup(const alignmentGroup &g);

  void printAlmMap(const almMap &m);
  void printLgm(const labelGroupingMap &lgm);
  void printLsm(const labelAlmSubMap &lsm);
  void printEls(const edgeLabelSet &els);
}

#endif // __ALIGNMENTUTILS_HPP__
