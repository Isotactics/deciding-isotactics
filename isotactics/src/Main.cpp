#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <set>


#include "AlignmentUtils.hpp"
#include "GraphUtils.hpp"
#include "Utils.hpp"
#include "WitnessUtils.hpp"
#include "MatchUtils.hpp"
#include "DetWitnessUtils.hpp"



/* TODO:
*/



int main(int argc, char *argv[])
{
  if (argc != 4) {
    std::cout << "Usage: " << argv[0] << " m1.dot m2.dot alignment.json" << std::endl;
    return 0;
  }

  Graph_t g1 = Graph::parse(argv[1]);
  Graph_t g2 = Graph::parse(argv[2]);

  alignment alm = Alm::parse(argv[3]);

  labelGroupingMap lgm1 = Alm::LabelGroupingMap(g1, Alm::Lhs(alm));
  labelGroupingMap lgm2 = Alm::LabelGroupingMap(g2, Alm::Rhs(alm));

  edgeLabelSet els1 = Alm::lgmFlatten(lgm1);
  edgeLabelSet els2 = Alm::lgmFlatten(lgm2);


  WG_t wg = WG::create(g1, g2, alm);
//  WG::print(wg);


//  DWG_t dwg1 = DWG::createLhs(wg, els1);
//  DWG::print(dwg1, wg);


  DWG_t dwg2 = DWG::createRhs(wg, els2);
  DWG::print(dwg2, wg);



  return 0;
}
