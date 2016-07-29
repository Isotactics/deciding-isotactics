#ifndef __ALIGNMENTUTILS_HPP__
#define __ALIGNMENTUTILS_HPP__

#include <map>
#include <string>
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
using alignmentSub      = std::vector<alignmentPair>;

using almMap = std::map<alignmentGroup, alignmentGroup>;

namespace Alm {
  alignment parse(const std::string &path);

  alignmentGroup getGroup(json::array_t a);
  alignmentPair getPair(json::object_t o);

  alignmentGrouping getLhs(const alignment &alm);
  alignmentGrouping getRhs(const alignment &alm);

  almMap getAlmMap(const alignment &alm);

  bool hasLabel(const alignmentGroup &g, const label &l);
  alignmentGrouping getGrouping(const label &l, const alignmentHalf &alh);

  bool hasPair(const alignment &alm, const alignmentPair &p);

  void print(const alignment &a);
  void printPair(const alignmentPair &p);
  void printGrouping(const alignmentGrouping &gp);
  void printGroup(const alignmentGroup &g);

  void printAlmMap(const almMap &m);
}

#endif // __ALIGNMENTUTILS_HPP__
