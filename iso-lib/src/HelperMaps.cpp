#include "HelperMaps.hpp"

labelGroupingMap Helper::LabelGroupingMap(const Graph_t &g, const alignmentHalf &alh)
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

labelAlmSubMap Helper::LabelAlmSubMap(const Graph_t &g, const alignment &alm)
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

edgeLabelSet Helper::lgmFlatten(const labelGroupingMap &lgm)
{
  edgeLabelSet els;

  for (const std::pair<label, alignmentGrouping> &p : lgm)
    els.insert(p.second);

  return els;
}

std::vector<label> Helper::elsFlatten(const edgeLabelSet &els)
{
  std::vector<label> labels;

  for (const alignmentGrouping &gp : els)
    for (const alignmentGroup &g : gp)
      for (const label &l : g)
        labels.push_back(l);

  return labels;
}

labelPermissivenessMap Helper::emptyLpm(const alignment &alm)
{
  labelPermissivenessMap lpm;

  for (const alignmentPair &p : alm) {

    for (const label &l : p.first)
      lpm[l] = 0;

    for (const label &l : p.second)
      lpm[l] = 0;

  }

  return lpm;
}

labelPermissivenessMap Helper::LabelPermissivenessMap(const alignment &alm)
{
  labelPermissivenessMap lpm = Helper::emptyLpm(alm);

  for (const alignmentPair &p : alm) {

    for (const label &l : p.first)
      lpm[l] += p.second.size();

    for (const label &l : p.second)
      lpm[l] += p.first.size();

  }

  return lpm;
}

int Helper::maxPermissiveness(const alignment &alm)
{
  int maxPermisseveness = 0;
  labelPermissivenessMap lpm = Helper::LabelPermissivenessMap(alm);

  for (const std::pair<label, int> &p : lpm) {
    if (p.second > maxPermisseveness)
      maxPermisseveness = p.second;
  }

  return maxPermisseveness;
}

int Helper::maxComplexity(const alignment &alm)
{
  int maxComplexity = 0;

  for (const alignmentPair &p : alm) {
    int complexity = p.first.size() * p.second.size();

    if (complexity > maxComplexity)
      maxComplexity = complexity;
  }

  return maxComplexity;
}

int Helper::alignmentNorm(const alignment &alm)
{
  return std::max(Helper::maxPermissiveness(alm), Helper::maxComplexity(alm));
}

void Helper::labelsToGroupings(Graph_t &g, labelGroupingMap &lgm)
{
  const Range<Graph::eIter> edges = Util::makeRange(boost::edges(g));

  for (const Graph::eDesc &e : edges) {
    alignmentGrouping gp = lgm[g[e].label];
    g[e].gp = gp;
  }

  return;
}






void Helper::printLgm(const labelGroupingMap &lgm)
{
  for (const std::pair<label, alignmentGrouping> &p : lgm) {
    std::cout << p.first << " ->  ";
    Alm::printGrouping(p.second);
    Util::printLine();
  }

  return;
}

void Helper::printLsm(const labelAlmSubMap &lsm)
{
  for (const std::pair<label, alignmentSub> &p : lsm) {
    std::cout << p.first << " -> " << std::endl;
    Alm::print(p.second);
    Util::printLine();
  }

  return;
}

void Helper::printEls(const edgeLabelSet &els)
{
  for (const alignmentGrouping &gp : els) {
    Alm::printGrouping(gp);
    Util::printLine();
  }

  return;
}

void Helper::printLpm(const labelPermissivenessMap &lpm)
{
  for (const std::pair<label, int> &p : lpm) {
    std::cout << p.first << " ->  " << p.second;
    Util::printLine();
  }

  return;
}
