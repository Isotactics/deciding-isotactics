#include <algorithm>
#include <deque>
#include <fstream>
#include <deque>
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



bool isSuperset(std::vector<Graph::vDesc> a, std::vector<Graph::vDesc> b)
{
  std::sort(a.begin(), a.end());
  std::sort(b.begin(), b.end());

  return std::includes(a.begin(), a.end(), b.begin(), b.end());
}

bool isProperSuperset(std::vector<Graph::vDesc> a, std::vector<Graph::vDesc> b)
{
  return (a.size() > b.size());
}

DG_t det(const Graph_t &g, const edgeLabelSet &els)
{
  DG_t dg;

  //DG::vDesc dgEmpty = DG::addEmptyVertex(dg, els);

  Graph::vDesc gStart = Graph::getStart(g);
  DG::Vertex dgStart = DG::createStart();

  DG::addVertexToSet(gStart, dgStart);
  DG::updateVertexName(dgStart, g);

  DG::vDesc dgStartvd = DG::addVertex(dg, dgStart);

  std::deque<DG::vDesc> dgTodo;
  dgTodo.push_back(dgStartvd);

  //while (!dgTodo.empty()) {
  for (int i = 0; i < 2; i++) {

    DG::vDesc curr = dgTodo.front();
    dgTodo.pop_front();

    std::cout << "curr: " << dg[curr].name << "\n";

    std::vector<Graph::eDesc> outEdges = DG::getOutEdges(dg, curr, g);

    for (const Graph::eDesc &oe : outEdges)
      Graph::printOutEdge(g, oe);

    Util::printLine();

    for (const alignmentGrouping &gp : els) {
      label l = Alm::getLabelFromGrouping(gp);

      if (!DG::hasEdgeForGrouping(g, outEdges, gp)) {
        //DG::addEdge(dg, curr, l, dgEmpty);
        continue;
      }

      std::vector<Graph::eDesc> edges = DG::getEdgesForGrouping(g, outEdges, gp);
      std::vector<Graph::vDesc> dsts = Graph::getDestinations(g, edges);

      if (isSuperset(dsts, dg[curr].vs) && isProperSuperset(dsts, dg[curr].vs)) {
        dg[curr].vs = dsts;
        dgTodo.push_back(curr);
        DG::updateVertexName(dg, curr, g);
        continue;
      }

      if (isSuperset(dsts, dg[curr].vs) && !isProperSuperset(dsts, dg[curr].vs)) {
        DG::addEdge(dg, curr, l, curr);
        continue;
      }

      DG::Vertex nv = DG::




    } // els loop

    std::cout << "\n\n";

  } // todo lopp






  std::cout << "---------------------------------------------\n";
  DG::print(dg);












/*

  Graph::vDesc empty = Graph::addEmptyVertex(dg);

  for (alignmentGrouping gp : els)
    Graph::addEdge(empty, gp, empty, dg);

  Graph::vDesc gStart = Graph::getStart(g);
  Graph::Vertex dgStart = Graph::createVertex(g, gStart);

  Graph::vDesc dStart = Graph::addVertex(dg, dgStart);

  Graph::addVertexToSet(g, dStart, dgStart);



  Graph::print(dg);




//  addVertex(res, dgStart);



  std::deque<Graph::Vertex> todo;
  todo.push_back(dgStart);

  //while(!todo.empty()) {
  for (int i = 0; i < 1; i++) {

    Graph::Vertex curr = todo.front();
    todo.pop_front();
    std::cout << "working on: " << curr.name << "\n";

    std::vector<Graph::eDesc> oedges = Graph::getOutEdges(g, curr);

    for (const Graph::eDesc &oe : oedges)
      Graph::printOutEdge(g, oe);



    for (const alignmentGrouping &gp : els) {

    }

  }
*/

  return dg;
}




int main(int argc, char *argv[])
{
  if (argc != 4) {
    std::cout << "Usage: " << argv[0] << " m1.dot m2.dot alignment.json" << std::endl;
    return 0;
  }

  Graph_t g1 = Graph::parse(argv[1]);
  Graph_t g2 = Graph::parse(argv[2]);

  alignment alm = Alm::parse(argv[3]);

  labelGroupingMap lgm1 = Helper::LabelGroupingMap(g1, Alm::Lhs(alm));
  labelGroupingMap lgm2 = Helper::LabelGroupingMap(g2, Alm::Rhs(alm));

  edgeLabelSet els1 = Helper::lgmFlatten(lgm1);
  edgeLabelSet els2 = Helper::lgmFlatten(lgm2);

  Helper::labelsToGroupings(g1, lgm1);
  Helper::labelsToGroupings(g2, lgm2);

  Graph::print(g1);
  Util::printLine();


  det(g1, els1);





/*
  Alm::print(alm);
  Util::printLine();


  Graph::print(g1);
  Util::printLine();


  Graph::print(g2);
  Util::printLine();

*/


//  Graph::print(g2);
//  Util::printLine();




//  WG_t wg = WG::create(g1, g2, alm);






  return 0;
}
