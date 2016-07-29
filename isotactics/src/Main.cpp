#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

#include "AlignmentUtils.hpp"
#include "GraphUtils.hpp"
#include "Utils.hpp"



void test(Graph_t g)
{
  const Range<eIter> edges = Util::makeRange(boost::edges(g));

  for (const eDesc &e : edges) {
    std::cout << g[e].label  << std::endl;

  }
}



/*
TODO:
  - startzustand parsen/speichern
  - endzustänge (plural!) parsen/speichern

  - witness graph algo
*/



int main()
{

  Graph_t g1 = Graph::parse("resources/m1.dot");
  Graph_t g2 = Graph::parse("resources/m2.dot");

  alignment alm = Alm::parse("resources/alignment.json");

  Graph::addAlm(g1, alm);
  Graph::addAlm(g2, alm);

  Graph::addAlmHalf(g1, Alm::getLhs(alm));
  Graph::addAlmHalf(g2, Alm::getRhs(alm));


  labelGroupingMap lgm1 = Graph::getLabelGroupingMap(g1);
  labelGroupingMap lgm2 = Graph::getLabelGroupingMap(g2);


  labelAlmSubMap lsm1 = Graph::getLabelAlmSubMap(g1);
  //Graph::printLsm(lsm1);

  labelAlmSubMap lsm2 = Graph::getLabelAlmSubMap(g2);
//  Graph::printLsm(lsm2);


  //std::cout << g1[(g1[boost::graph_bundle].start)].name << std::endl;


  test(g1);









  return 0;
}
