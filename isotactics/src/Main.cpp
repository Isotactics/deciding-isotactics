#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <sstream>


#include "AlignmentUtils.hpp"
#include "GraphUtils.hpp"
#include "Utils.hpp"
#include "WitnessUtils.hpp"



/* TODO:
  - im ausgabe dot knoten mit attributen

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



  WG_t wg;

  matchSet emptyMS;

  WG::vDesc wgv1, wgv2;
  Graph::vDesc gv1, gv2;

  Range<Graph::oeIter> oe1, oe2;

  gv1 = Graph::getStart(g1);
  gv2 = Graph::getStart(g2);

  wgv1 = WG::addVertex(g1[gv1].name, g2[gv2].name, emptyMS, wg);

  std::deque<WG::vDesc> wgTodo;
  wgTodo.push_back(wgv1);


  matchSet ms;

  label l1, l2;
  matchSet msTmp;

  Graph::vDesc dst1, dst2;

  while (!wgTodo.empty()) {

//  for (int i = 0; i < 4; i++) {

    wgv1 = wgTodo.front();
    wgTodo.pop_front();

    std::cout << "working on: " << wg[wgv1].name << std::endl;

    ms = wg[wgv1].ms;

    gv1 = Graph::getVertex(wg[wgv1].v1Name, g1);
    gv2 = Graph::getVertex(wg[wgv1].v2Name, g2);

    oe1 = Util::makeRange(boost::out_edges(gv1, g1));
    oe2 = Util::makeRange(boost::out_edges(gv2, g2));


    for (const WG::eDesc &e1 : oe1) {
      l1 = g1[e1].label;

      for (const WG::eDesc &e2 : oe2) {
        l2 = g2[e2].label;

        //std::cout << "getting match set for labels: " << l1 << " " << l2 << std::endl;

        msTmp = WG::getMatchSet(alm, ms, l1, l2);

        if (msTmp.empty())
          continue;

        //std::cout << "match found for labels: " << l1 << " " << l2 << std::endl;

        dst1 = boost::target(e1, g1);
        dst2 = boost::target(e2, g2);

        wgv2 = WG::addVertex(g1[dst1].name, g2[dst2].name, msTmp, wg);
        WG::addEdge(wgv1, lgm1[l1], lgm2[l2], wgv2, wg);

        wgTodo.push_back(wgv2);
      }
    }








    WG::print(wg);
    Util::printLine();
/*
    std::cout << "Todo:" << std::endl;

    for (const WG::vDesc &v : wgTodo)
      std::cout << "  " << wg[v].name << std::endl;
*/
    Util::printLine();
  }




/*
  Range<Graph::oeIter> oe1 = Util::makeRange(boost::out_edges(s1, g1));
  Range<Graph::oeIter> oe2 = Util::makeRange(boost::out_edges(s2, g2));






  for (const WG::eDesc &e1 : oe1) {
    l1 = g1[e1].label;


    for (const WG::eDesc &e2 : oe2) {
      l2 = g2[e2].label;

      msTmp = WG::getMatchSet(alm, ms, l1, l2);

      if (msTmp.empty())
        continue;

      dst1 = boost::target(e1, g1);
      dst2 = boost::target(e2, g2);

      wgv2 = WG::addVertex(g1[dst1].name, g2[dst2].name, msTmp[0], wg);

      WG::addEdge(wgv1, lgm1[l1], lgm2[l2], wgv2, wg);
    }



  }


  WG::print(wg);



  matchSet tmp = WG::getMatchSet(alm, ms, "a", "s");

  match m = tmp[0];

  WG::vDesc v1 = WG::addVertex(g1[s1].name, g2[s2].name, m, wg);
  WG::vDesc v2 = WG::addVertex(g1[s1].name, g2[s2].name, m, wg);

  WG::eDesc e = WG::addEdge(v1, lgm1["a"], lgm2["s"], v2, wg);

  WG::print(wg);
*/

  return 0;
}
