#include <string>
#include <utility>
#include <vector>

#include "json.hpp"


using json = nlohmann::json;

using label             = std::string;
using alignmentGroup    = std::vector<label>;
using alignmentGrouping = std::vector<alignmentGroup>;
using alignmentPair     = std::pair<alignmentGroup, alignmentGroup>;
using alignment         = std::vector<alignmentPair>;



alignment parseAlignment(const std::string &path);

alignmentGrouping getAlignmentLhs(const alignment &al);
alignmentGrouping getAlignmentRhs(const alignment &al);

alignmentGroup arrayToAlignGroup(json::array_t a);
alignmentPair objectToAlignPair(json::object_t o);

bool groupContainsLabel(const label &l, const alignmentGroup &g);
alignmentGrouping getLabelGrouping(const label &l, const alignmentGrouping &gp);

void printAlignGroup(const alignmentGroup &g);
void printAlignGrouping(const alignmentGrouping &gp);
void printAlignPair(const alignmentPair &p);
void printAlignment(const alignment &a);
