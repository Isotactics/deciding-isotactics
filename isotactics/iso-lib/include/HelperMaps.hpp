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

using labelPermissivenessMap = std::unordered_map<label, int>;



namespace Helper {
  labelGroupingMap LabelGroupingMap(const Graph_t &g, const alignmentHalf &alh);
  labelAlmSubMap LabelAlmSubMap(const Graph_t &g, const alignment &alm);

  edgeLabelSet lgmFlatten(const labelGroupingMap &lgm);
  std::vector<label> elsFlatten(const edgeLabelSet &els);

  void labelsToGroupings(Graph_t &g, labelGroupingMap &lgm);

  labelPermissivenessMap emptyLpm(const alignment &alm);
  labelPermissivenessMap LabelPermissivenessMap(const alignment &alm);

  int maxPermissiveness(const alignment &alm);
  int maxComplexity(const alignment &alm);
  int alignmentNorm(const alignment &alm);




  void printLgm(const labelGroupingMap &lgm);
  void printLsm(const labelAlmSubMap &lsm);
  void printEls(const edgeLabelSet &els);
  void printLpm(const labelPermissivenessMap &lpm);
}



#endif // __HELPERMAPS_HPP__
