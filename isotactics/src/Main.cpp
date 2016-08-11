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
*/


int main(int argc, char *argv[])
{

  Graph_t g1 = Graph::parse(argv[1]);
  Graph_t g2 = Graph::parse(argv[2]);

  alignment alm = Alm::parse(argv[3]);

  labelGroupingMap lgm1 = Alm::LabelGroupingMap(g1, Alm::Lhs(alm));
  labelGroupingMap lgm2 = Alm::LabelGroupingMap(g2, Alm::Rhs(alm));

  Graph::vDesc gs1 = Graph::getStart(g1);
  Graph::vDesc gs2 = Graph::getStart(g2);


  WG_t wg;
  matchSet ms, msNew;

  label l1, l2;

  WG::vDesc wgv1, wgv2;

  Graph::vDesc gv1, gv2;
  Graph::vDesc dst1, dst2;

  Range<Graph::oeIter> oe1, oe2;


  // add init node to wg
  WG::Vertex v = WG::createVertex(g1[gs1].name, g2[gs2].name, ms);
  wgv1 = WG::addVertex(v, wg);

  // add init node to todo list
  std::deque<WG::vDesc> wgTodo;
  wgTodo.push_back(wgv1);


  while (!wgTodo.empty()) {

    wgv1 = wgTodo.front();
    wgTodo.pop_front();

    gv1 = Graph::getVertex(wg[wgv1].v1Name, g1);
    gv2 = Graph::getVertex(wg[wgv1].v2Name, g2);

    oe1 = Util::makeRange(boost::out_edges(gv1, g1));
    oe2 = Util::makeRange(boost::out_edges(gv2, g2));

    ms = wg[wgv1].ms;

    WG::Vertex currentV = WG::createVertex(wg[wgv1].v1Name, wg[wgv1].v2Name, ms);

    std::cerr << "working on: " << currentV.name << std::endl;

    std::cerr << "  checking rule 1:" << std::endl;

    for (const WG::eDesc &e1 : oe1) {
      l1 = g1[e1].label;

      for (const WG::eDesc &e2 : oe2) {
        l2 = g2[e2].label;

        msNew = Match::getMatchSet(alm, ms, l1, l2);

        if (msNew.empty())
          continue;

        std::cerr << "    found new match set" << std::endl;
        std::cerr << "    " << l1 << ", " << l2 << ", " << Match::setToString(msNew) << std::endl;

        dst1 = boost::target(e1, g1);
        dst2 = boost::target(e2, g2);

        WG::Vertex newV = WG::createVertex(g1[dst1].name, g2[dst2].name, msNew);
        std::cerr << "    new Vertex: " << newV.name << std::endl;

        if (WG::hasVertex(newV, wg)) {
          std::cerr << "    vertex already exists. adding edge, not todo" << std::endl << std::endl;
          wgv2 = WG::getVertex(newV, wg);
          WG::addEdge(wgv1, lgm1[l1], lgm2[l2], wgv2, wg);
          continue;
        }

        std::cerr << "    new vertex doesn't exist. add vertex, add edge, add todo!" << std::endl << std::endl;

        wgv2 = WG::addVertex(newV, wg);
        WG::addEdge(wgv1, lgm1[l1], lgm2[l2], wgv2, wg);

        wgTodo.push_back(wgv2);
      }
    }









    std::cerr << "  checking rule 2:" << std::endl;
    std::cerr << "    g1 moves:" << std::endl;


    for (const WG::eDesc &e1 : oe1) {
          l1 = g1[e1].label;

          msNew = Match::getMatchSet2(ms, l1);

          if (msNew.empty())
            continue;

      std::cerr << "    found new match set" << std::endl;
      std::cerr << "    " << l1 << ", " << Match::setToString(msNew) << std::endl;

      dst1 = boost::target(e1, g1);
      dst2 = gv2;

      alignmentGrouping gp2;

      WG::Vertex newV = WG::createVertex(g1[dst1].name, g2[dst2].name, msNew);
      std::cerr << "    new Vertex: " << newV.name << std::endl << std::endl;

      if (WG::vertexEqual(currentV, newV)) {
        std::cerr << "   i,m the new vertex, only adding edge to myself " << std::endl;
        wgv2 = wgv1;
        WG::addEdge(wgv1, lgm1[l1], gp2, wgv2, wg);
        continue;
      }

      if (WG::hasVertex(newV, wg)) {
        std::cerr << "   new vertex already exists. adding edge, not todo " << std::endl;
        wgv2 = WG::getVertex(newV, wg);
        WG::addEdge(wgv1, lgm1[l1], gp2, wgv2, wg);
        continue;
      }

      std::cerr << "   new vertex doesn't exist. add node, add edge, add todo! " << std::endl;

      wgv2 = WG::addVertex(newV, wg);
      WG::addEdge(wgv1, lgm1[l1], gp2, wgv2, wg);
      wgTodo.push_back(wgv2);
    }















    std::cerr << "  checking rule 2:" << std::endl;
    std::cerr << "    g2 moves:" << std::endl;


    for (const WG::eDesc &e2 : oe2) {
          l2 = g2[e2].label;

          msNew = Match::getMatchSet2(ms, l2);

          if (msNew.empty())
            continue;

      std::cerr << "    found new match set" << std::endl;
      std::cerr << "    " << l2 << ", " << Match::setToString(msNew) << std::endl;

      dst1 = gv1;
      dst2 = boost::target(e2, g2);

      alignmentGrouping gp1;

      WG::Vertex newV = WG::createVertex(g1[dst1].name, g2[dst2].name, msNew);
      std::cerr << "    new Vertex: " << newV.name << std::endl << std::endl;

      if (WG::vertexEqual(currentV, newV)) {
        std::cerr << "   i,m the new vertex, only adding edge to myself " << std::endl;
        wgv2 = wgv1;
        WG::addEdge(wgv1, gp1, lgm2[l2], wgv2, wg);
        continue;
      }

      if (WG::hasVertex(newV, wg)) {
        std::cerr << "   new vertex already exists. adding edge, not todo " << std::endl;
        wgv2 = WG::getVertex(newV, wg);
        WG::addEdge(wgv1, gp1, lgm2[l2], wgv2, wg);
        continue;
      }

      std::cerr << "   new vertex doesn't exist. add node, add edge, add todo! " << std::endl;

      wgv2 = WG::addVertex(newV, wg);
      WG::addEdge(wgv1, gp1, lgm2[l2], wgv2, wg);
      wgTodo.push_back(wgv2);
    }




    Util::printLineDebug();
    WG::printDebug(wg);
    Util::printLineDebug();


    std::cerr << "Todo:" << std::endl;

    for (const WG::vDesc &v : wgTodo)
      std::cerr << "  " << wg[v].name << std::endl;

    Util::printLineDebug();
    Util::printLineDebug();
    Util::printLineDebug();
  }



  WG::print(wg);







  return 0;
}
