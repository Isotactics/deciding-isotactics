#include <algorithm>
#include <fstream>
#include <iostream>

#include <map>
#include <unordered_map>

#include <vector>

#include "GraphUtils.h"
#include "AlignmentUtils.h"



void printNewLine()
{
  std::cout << std::endl;
}




using labelGroupingMap = std::unordered_map<label, alignmentGrouping>;


labelGroupingMap createLabelGroupingMap(Graph &g)
{
  labelGroupingMap lgm;

  const Range<eIter> edges = makeRange(boost::edges(g));
  alignmentHalf alh = getAlignmentHalf(g);

  label l;
  alignmentGrouping gp;

  for (const eDesc &e : edges) {
    l = g[e].label;
    gp = getLabelGrouping(l, alh);

    lgm[l] = gp;
  }

  return lgm;
}


void printLabelGroupingMap(const labelGroupingMap &lgm)
{
  for (const std::pair<label, alignmentGrouping> p : lgm) {
    std::cout << p.first << " ->  ";

    printAlignGrouping(p.second);
    printNewLine();
  }

  return;
}




int main()
{

  Graph g1 = parseGraph("resources/m1.dot");
  Graph g2 = parseGraph("resources/m2.dot");

  alignment al = parseAlignment("resources/alignment.json");

  addAlignment(g1, al);
  addAlignment(g2, al);

  alignmentGrouping lhs = getAlignmentLhs(al);
  alignmentGrouping rhs = getAlignmentRhs(al);

  addAlignmentHalf(g1, lhs);
  addAlignmentHalf(g2, rhs);

  printGraph(g1);
  printNewLine();
  printNewLine();

  printGraph(g2);
  printNewLine();
  printNewLine();


  labelGroupingMap lgm1 = createLabelGroupingMap(g1);
  labelGroupingMap lgm2 = createLabelGroupingMap(g2);

  printLabelGroupingMap(lgm1);
  printNewLine();
  printLabelGroupingMap(lgm2);







/*
  printGraph(g1);
  printNewLine();

  printGraph(g2);
  printNewLine();


  alignment al = parseAlignment("resources/alignment.json");

  alignmentGrouping lhs = getAlignmentLhs(al);
  alignmentGrouping rhs = getAlignmentRhs(al);

  alignmentGrouping l_s = getLabelGrouping("s", rhs);

  printAlignGrouping(l_s);
*/


  return 0;
}
