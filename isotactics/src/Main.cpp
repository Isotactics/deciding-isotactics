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


int main()
{

  Graph_t g1 = Graph::parse("resources/m1.dot");
  Graph_t g2 = Graph::parse("resources/m2.dot");

  alignment alm = Alm::parse("resources/alignment.json");

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





//  for (int i = 0; i < 1; i++) {
  while (!wgTodo.empty()) {

    wgv1 = wgTodo.front();
    wgTodo.pop_front();

    gv1 = Graph::getVertex(wg[wgv1].v1Name, g1);
    gv2 = Graph::getVertex(wg[wgv1].v2Name, g2);

    oe1 = Util::makeRange(boost::out_edges(gv1, g1));
    oe2 = Util::makeRange(boost::out_edges(gv2, g2));

    ms = wg[wgv1].ms;

    WG::Vertex currentV = WG::createVertex(wg[wgv1].v1Name, wg[wgv1].v2Name, ms);

    std::cout << "working on: " << currentV.name << std::endl;



    std::cout << "  checking rule 1:" << std::endl;

    for (const WG::eDesc &e1 : oe1) {
      l1 = g1[e1].label;

      for (const WG::eDesc &e2 : oe2) {
        l2 = g2[e2].label;

        msNew = Match::getMatchSet(alm, ms, l1, l2);

        if (msNew.empty())
          continue;

        std::cout << "    found new match set" << std::endl;
        std::cout << "    " << l1 << ", " << l2 << ", " << Match::setToString(msNew) << std::endl;

        dst1 = boost::target(e1, g1);
        dst2 = boost::target(e2, g2);

        WG::Vertex newV = WG::createVertex(g1[dst1].name, g2[dst2].name, msNew);
        std::cout << "    new Vertex: " << newV.name << std::endl;

        if (WG::hasVertex(newV, wg)) {
          std::cout << "    vertex already exists. adding edge, not todo" << std::endl << std::endl;
          wgv2 = WG::getVertex(newV, wg);
          WG::addEdge(wgv1, lgm1[l1], lgm2[l2], wgv2, wg);
          continue;
        }

        std::cout << "    new vertex doesn't exist. add vertex, add edge, add todo!" << std::endl << std::endl;

        wgv2 = WG::addVertex(newV, wg);
        WG::addEdge(wgv1, lgm1[l1], lgm2[l2], wgv2, wg);

        wgTodo.push_back(wgv2);
      }
    }









    std::cout << "  checking rule 2:" << std::endl;
    std::cout << "    g1 moves:" << std::endl;


    for (const WG::eDesc &e1 : oe1) {
          l1 = g1[e1].label;

          msNew = Match::getMatchSet2(ms, l1);

          if (msNew.empty())
            continue;

      std::cout << "    found new match set" << std::endl;
      std::cout << "    " << l1 << ", " << Match::setToString(msNew) << std::endl;

      dst1 = boost::target(e1, g1);
      dst2 = gv2;

      alignmentGrouping gp2;

      WG::Vertex newV = WG::createVertex(g1[dst1].name, g2[dst2].name, msNew);
      std::cout << "    new Vertex: " << newV.name << std::endl << std::endl;

      if (WG::vertexEqual(currentV, newV)) {
        std::cout << "   i,m the new vertex, only adding edge to myself " << std::endl;
        wgv2 = wgv1;
        WG::addEdge(wgv1, lgm1[l1], gp2, wgv2, wg);
        continue;
      }

      if (WG::hasVertex(newV, wg)) {
        std::cout << "   new vertex already exists. adding edge, not todo " << std::endl;
        wgv2 = WG::getVertex(newV, wg);
        WG::addEdge(wgv1, lgm1[l1], gp2, wgv2, wg);
        continue;
      }

      std::cout << "   new vertex doesn't exist. add node, add edge, add todo! " << std::endl;

      wgv2 = WG::addVertex(newV, wg);
      WG::addEdge(wgv1, lgm1[l1], gp2, wgv2, wg);
      wgTodo.push_back(wgv2);
    }















    std::cout << "  checking rule 2:" << std::endl;
    std::cout << "    g2 moves:" << std::endl;


    for (const WG::eDesc &e2 : oe2) {
          l2 = g2[e2].label;

          msNew = Match::getMatchSet2(ms, l2);

          if (msNew.empty())
            continue;

      std::cout << "    found new match set" << std::endl;
      std::cout << "    " << l2 << ", " << Match::setToString(msNew) << std::endl;

      dst1 = gv1;
      dst2 = boost::target(e2, g2);

      alignmentGrouping gp1;

      WG::Vertex newV = WG::createVertex(g1[dst1].name, g2[dst2].name, msNew);
      std::cout << "    new Vertex: " << newV.name << std::endl << std::endl;

      if (WG::vertexEqual(currentV, newV)) {
        std::cout << "   i,m the new vertex, only adding edge to myself " << std::endl;
        wgv2 = wgv1;
        WG::addEdge(wgv1, gp1, lgm2[l2], wgv2, wg);
        continue;
      }

      if (WG::hasVertex(newV, wg)) {
        std::cout << "   new vertex already exists. adding edge, not todo " << std::endl;
        wgv2 = WG::getVertex(newV, wg);
        WG::addEdge(wgv1, gp1, lgm2[l2], wgv2, wg);
        continue;
      }

      std::cout << "   new vertex doesn't exist. add node, add edge, add todo! " << std::endl;

      wgv2 = WG::addVertex(newV, wg);
      WG::addEdge(wgv1, gp1, lgm2[l2], wgv2, wg);
      wgTodo.push_back(wgv2);
    }



















    Util::printLine();
    WG::print(wg);
    Util::printLine();


    std::cout << "Todo:" << std::endl;

    for (const WG::vDesc &v : wgTodo)
      std::cout << "  " << wg[v].name << std::endl;

    Util::printLine();
    Util::printLine();
    Util::printLine();
    Util::printLine();
    Util::printLine();
    Util::printLine();
  }



  Alm::printGrouping(lgm2["x"]);

















/*
  WG_t wg;

  matchSet emptyMS;

  WG::vDesc wgv1, wgv2;
  Graph::vDesc gv1, gv2;
  Graph::vDesc dst1, dst2;

  Range<Graph::oeIter> oe1, oe2;

  gv1 = Graph::getStart(g1);
  gv2 = Graph::getStart(g2);

  wgv1 = WG::addVertex(g1[gv1].name, g2[gv2].name, emptyMS, wg);

  std::deque<WG::vDesc> wgTodo;
  wgTodo.push_back(wgv1);


  matchSet ms;

  label l1, l2;
  matchSet msTmp;


  while (!wgTodo.empty()) {

//  for (int i = 0; i < 9; i++) {

    wgv1 = wgTodo.front();
    wgTodo.pop_front();

    std::cout << "working on: " << wg[wgv1].name << std::endl;

    ms = wg[wgv1].ms;

    gv1 = Graph::getVertex(wg[wgv1].v1Name, g1);
    gv2 = Graph::getVertex(wg[wgv1].v2Name, g2);

    oe1 = Util::makeRange(boost::out_edges(gv1, g1));
    oe2 = Util::makeRange(boost::out_edges(gv2, g2));


    std::cout << "checking rule 1:" << std::endl;

    for (const WG::eDesc &e1 : oe1) {
      l1 = g1[e1].label;

      for (const WG::eDesc &e2 : oe2) {
        l2 = g2[e2].label;

        //std::cout << "getting match set for labels: " << l1 << " " << l2 << std::endl;

        msTmp = WG::getMatchSet(alm, ms, l1, l2);

        if (msTmp.empty())
          continue;

        std::cout << "match found for labels: " << l1 << " " << l2 << std::endl;

        dst1 = boost::target(e1, g1);
        dst2 = boost::target(e2, g2);

        std::cout << "checking vertex: " << g1[dst1].name << ", " << g2[dst2].name << std::endl;

        if (WG::hasVertex(g1[dst1].name, g2[dst2].name, ms, wg)) {
          std::cout << "vertex already exists" << std::endl;
          continue;
        }

        std::cout << "vertex new, adding vertex" << std::endl;

        wgv2 = WG::addVertex(g1[dst1].name, g2[dst2].name, msTmp, wg);
        WG::addEdge(wgv1, lgm1[l1], lgm2[l2], wgv2, wg);

        wgTodo.push_back(wgv2);
      }
    }

    std::cout << std::endl << "checking rule 2:" << std::endl;

    std::cout << "g1 moves:" << std::endl;

    for (const WG::eDesc &e1 : oe1) {
      l1 = g1[e1].label;

      msTmp = WG::getMatchSet2(ms, l1);

      if (msTmp.empty())
        continue;

      std::cout << "match found for label: " << l1 << std::endl;

      dst1 = boost::target(e1, g1);
      alignmentGrouping gp2;

      // Fall 1: Target Knoten is neu
      std::cout << "checking vertex: " << g1[dst1].name << ", " << g2[gv2].name << std::endl;

      if (!WG::hasVertex(g1[dst1].name, g2[gv2].name, ms, wg)) {
        std::cout << "vertex new, adding vertex" << std::endl;

        wgv2 = WG::addVertex(g1[dst1].name, wg[wgv1].v2Name, msTmp, wg);
        WG::addEdge(wgv1, lgm1[l1], gp2, wgv2, wg);

        wgTodo.push_back(wgv2);
        continue;
      }

      std::cout << "vertex already exists" << std::endl;

      // Fall 2: Target Knoten bin ich selbst (ich bin offensichtlich nicht neu)
      if ((wg[wgv1].v1Name == g1[dst1].name) && (wg[wgv1].v2Name == g2[gv2].name)) {

        std::cout << "target vertex is me" << std::endl;

        wgv2 = wgv1;
        WG::addEdge(wgv1, lgm1[l1], gp2, wgv2, wg);
        continue;
      }

      // Fall 3: target knoten istn anderer knoten, dens aber eben schon gibt
      std::cout << "target vertex not me but already exists" << std::endl;

      std::cout << "own ms: ";
      WG::printMatchSet(ms);


      wgv2 = WG::getVertex(g1[dst1].name, g2[gv2].name, ms, wg);

      std::cout << "found vertex with:" << std::endl;
      std::cout << "  v1Name: " << wg[wgv2].v1Name << std::endl;
      std::cout << "  v2Name: " << wg[wgv2].v2Name << std::endl;
      std::cout << "  ms: ";
      WG::printMatchSet(wg[wgv2].ms);


      WG::addEdge(wgv1, lgm1[l1], gp2, wgv2, wg);
    }





    std::cout << "g2 moves:" << std::endl;

    for (const WG::eDesc &e2 : oe2) {
      l2 = g2[e2].label;

      msTmp = WG::getMatchSet2(ms, l2);

      if (msTmp.empty())
        continue;

      std::cout << "match found for label: " << l2 << std::endl;

      dst2 = boost::target(e2, g2);
      alignmentGrouping gp1;

      // Fall 1: Target Knoten is neu
      std::cout << "checking vertex: " << g1[gv1].name << ", " << g2[dst2].name << std::endl;

      if (!WG::hasVertex(g1[gv1].name, g2[dst2].name, ms, wg)) {
        std::cout << "vertex new, adding vertex" << std::endl;

        wgv2 = WG::addVertex(g1[gv1].name, g2[dst2].name, msTmp, wg);
        WG::addEdge(wgv1, gp1, lgm2[l2], wgv2, wg);

        wgTodo.push_back(wgv2);
        continue;
      }

      std::cout << "vertex already exists" << std::endl;

      // Fall 2: Target Knoten bin ich selbst (ich bin offensichtlich nicht neu)
      if ((wg[wgv1].v1Name == g1[gv1].name) && (wg[wgv1].v2Name == g2[dst2].name)) {

        std::cout << "target vertex is me" << std::endl;

        wgv2 = wgv1;
        WG::addEdge(wgv1, gp1, lgm2[l2], wgv2, wg);
        continue;
      }

      // Fall 3: target knoten istn anderer knoten, dens aber eben schon gibt
      std::cout << "target vertex not me but already exists" << std::endl;

      wgv2 = WG::getVertex(g1[gv1].name, g2[dst2].name, ms, wg);
      WG::addEdge(wgv1, gp1, lgm2[l2], wgv2, wg);
    }



    WG::print(wg);
    Util::printLine();


    std::cout << "Todo:" << std::endl;

    for (const WG::vDesc &v : wgTodo)
      std::cout << "  " << wg[v].name << std::endl;

    Util::printLine();
  }

*/








  return 0;
}
