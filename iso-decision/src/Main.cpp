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
#include "CompareUtils.hpp"
#include "GraphUtils.hpp"
#include "Utils.hpp"
#include "WitnessUtils.hpp"
#include "MatchUtils.hpp"
#include "DetWitnessUtils.hpp"
#include "DetGraph.hpp"

#include "Logging.hpp"

//#include "program_options.cpp"

char* boolToWord(bool b) {

  if (b) return (char*)"Yes";
  return (char*)"No";

}


int main(int argc, char *argv[])
{
  // TODO use boost's program options library to allow options
  // like --silent and --wgPath <path to witness graph .dot>
  // this has a slight performance overhead, so it might not be worth it.
  // auto vm = getProgramOptionDescription(argc, argv);

  if (argc != 4) {
    std::cout << "Usage: " << argv[0] << " m1.dot m2.dot alignment.json" << std::endl;
    return 0;
  }

  // std::ofstream error("error.txt");

  // redirect cerr:
  ;// std::cerr.rdbuf(0);

  // read and parse the input automatons
  Graph_t g1 = Graph::parse(argv[1]);
  Graph_t g2 = Graph::parse(argv[2]);

  /*
  // list all vertices of g2
  boost::iterator_range<Graph::vIter> verticesIterator = boost::make_iterator_range(boost::vertices(g2));
  for (const Graph::vDesc &vertice : verticesIterator)
  {
    std::cout << g2[vertice].name << std::endl;
  }

  // list all edges of g2
  boost::iterator_range<Graph::eIter> edgesIterator = boost::make_iterator_range(boost::edges(g2));
  for (const Graph::eDesc &edge : edgesIterator)
  {
    std::cout << g2[ boost::source(edge,g2) ].name << g2[edge].label << g2[ boost::target(edge,g2) ].name << std::endl;
  }
  */

  // read and parse the alignment
  alignment alm = Alm::parse(argv[3]);

  // extract the sets of alignment groups for both the left and right side of the alignment.
  // for each label assign the groups it is contained in.
  labelGroupingMap lgm1 = Helper::LabelGroupingMap(g1, Alm::Lhs(alm));
  labelGroupingMap lgm2 = Helper::LabelGroupingMap(g2, Alm::Rhs(alm));

  // helper: get just the alignment groups without knowing to which label they belong.
  edgeLabelSet els1 = Helper::lgmFlatten(lgm1);
  edgeLabelSet els2 = Helper::lgmFlatten(lgm2);

  // in g1/g2 for each edge fill the "gp" property that contains
  // the set of alignment groups, that contain the edge label.
  // this will be used in DG::determinize to remove non-determinism
  // w.r.t. the alignment
  Helper::labelsToGroupings(g1, lgm1);
  Helper::labelsToGroupings(g2, lgm2);

  // Graph::print(g1);
  // Graph::print(g2);

  // remove non-determinism wrt. the alignment:
  // - merge edges (and vertices) that have the same set of alignment groups
  // - eliminate edges that have a label not contained in the alignment (epsilon-closure)
  DG_t dg1 = DG::determinize(g1, els1);
  DG_t dg2 = DG::determinize(g2, els2);

  /*
  std::ofstream d1file;
  d1file.open("det_m1.dot");
  DG::print(dg1,d1file);
  d1file.close();
  */

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

  bool leftEqual = Cmp::isEqual(dg1, dwg1, lgm1);
  bool rightEqual = Cmp::isEqual(dg2, dwg2, lgm2);
  bool bothEqual = leftEqual && rightEqual;

  std::cout << "Does the witness graph include all behavior of \"" << argv[1] << "\"? " << boolToWord(leftEqual) << std::endl;
  std::cout << "Does the witness graph include all behavior of \"" << argv[2] << "\"? " << boolToWord(rightEqual) << std::endl;


  return bothEqual ? 0 : 1;
}
