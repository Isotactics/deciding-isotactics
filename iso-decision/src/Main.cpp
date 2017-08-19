#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <set>
//

#include "AlignmentUtils.hpp"
#include "CompareUtils.hpp"
#include "GraphUtils.hpp"
#include "Utils.hpp"
#include "WitnessUtils.hpp"
#include "MatchUtils.hpp"
#include "DetWitnessUtils.hpp"
#include "DetGraph.hpp"

char* boolToWord(bool b) {

  if (b) return "Yes";
  return "No";

}


int main(int argc, char *argv[])
{
  if (argc != 4) {
    std::cout << "Usage: " << argv[0] << " m1.dot m2.dot alignment.json" << std::endl;
    return 0;
  }

  std::ofstream error("error.txt");



 // redirect cerr:
 std::cerr.rdbuf(0);


  Graph_t g1 = Graph::parse(argv[1]);
  Graph_t g2 = Graph::parse(argv[2]);

  alignment alm = Alm::parse(argv[3]);

  labelGroupingMap lgm1 = Helper::LabelGroupingMap(g1, Alm::Lhs(alm));
  labelGroupingMap lgm2 = Helper::LabelGroupingMap(g2, Alm::Rhs(alm));

  edgeLabelSet els1 = Helper::lgmFlatten(lgm1);
  edgeLabelSet els2 = Helper::lgmFlatten(lgm2);

  Helper::labelsToGroupings(g1, lgm1);
  Helper::labelsToGroupings(g2, lgm2);

  DG_t dg1 = DG::determinize(g1, els1);
  DG_t dg2 = DG::determinize(g2, els2);


  WG_t wg = WG::create(dg1, dg2, lgm1, lgm2, alm);
  //WG::print(wg);

  std::cout << "Created witness graph for machines \"" << argv[1] << "\" and \""  << argv[2] << "\" w.r.t. alignment \""  << argv[3] << "\"" << std::endl;

  std::cout << "Writing witness graph to \"" << argv[1] << "_" << argv[2] << "_"  << argv[3] << "_witness_graph.dot\"" << std::endl;

  std::ofstream myfile;
  myfile.open((std::string) argv[1] + "_" + (std::string) argv[2] + "_" + (std::string) argv[3] +  "_witness_graph.dot");
  WG::write(wg,myfile);
  myfile.close();


  std::cout << "Comparing machine behavior with the witness graph... " << std::endl;

  DWG_t dwg1 = DWG::createLhs(wg, els1);
  DWG_t dwg2 = DWG::createRhs(wg, els2);

  std::cout << "Does the witness graph include all behavior of \"" << argv[1] << "\"? " << boolToWord(Cmp::isEqual(dg1, dwg1, lgm1)) << std::endl;
  std::cout << "Does the witness graph include all behavior of \"" << argv[2] << "\"? " << boolToWord(Cmp::isEqual(dg2, dwg2, lgm2)) << std::endl;


  return 0;
}
