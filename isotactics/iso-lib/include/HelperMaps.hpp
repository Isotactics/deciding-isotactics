#ifndef __HELPERMAPS_HPP__
#define __HELPERMAPS_HPP__

#include <set>
#include <unordered_map>
#include <vector>

#include "AlignmentUtils.hpp"
#include "GraphUtils.hpp"
#include "Utils.hpp"


using labelGroupingMap = std::unordered_map<label, alignmentGrouping>;
using labelAlmSubMap = std::unordered_map<label, alignmentSub>;

using edgeLabelSet = std::set<alignmentGrouping>;

namespace Helper {
  labelGroupingMap LabelGroupingMap(const Graph_t &g, const alignmentHalf &alh);
  labelAlmSubMap LabelAlmSubMap(const Graph_t &g, const alignment &alm);

  edgeLabelSet lgmFlatten(const labelGroupingMap &lgm);
  std::vector<label> elsFlatten(const edgeLabelSet &els);

  void labelsToGroupings(Graph_t &g, labelGroupingMap &lgm);




  void printLgm(const labelGroupingMap &lgm);
  void printLsm(const labelAlmSubMap &lsm);
  void printEls(const edgeLabelSet &els);
}



#endif // __HELPERMAPS_HPP__
