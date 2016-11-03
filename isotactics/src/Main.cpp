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
#include "GraphUtils.hpp"
#include "Utils.hpp"
#include "WitnessUtils.hpp"
#include "MatchUtils.hpp"
#include "DetWitnessUtils.hpp"


using VertexPair = std::pair<Graph::vDesc, DWG::vDesc>;

bool isExitCondition(const VertexPair &vp, const Graph_t &g, const DWG_t &dwg)
{
  Graph::vDesc gv = vp.first;
  DWG::vDesc dwgv = vp.second;

  if ((g[gv].role == "end") && (dwg[dwgv].role != "end"))
    return true;

  return false;
}

std::string vertexPairToString(const VertexPair &vp, const Graph_t &g, const DWG_t &dwg)
{
  Graph::vDesc gv = vp.first;
  DWG::vDesc dwgv = vp.second;

  std::string res = "";
  res +=  g[gv].name;
  res += ", ";
  res += dwg[dwgv].name;

  return res;
}

bool alreadyVisited(const VertexPair &p, std::vector<VertexPair> visited)
{
  for (const VertexPair &vp : visited) {
    if (vp == p)
      return true;
  }

  return false;
}

bool isEqual(const Graph_t &g, const DWG_t &dwg, const labelGroupingMap &lgm) {

  Graph::vDesc gStart = Graph::getStart(g);
  DWG::vDesc dwgStart = DWG::getStart(dwg);

  VertexPair start(gStart, dwgStart);

  if (isExitCondition(start, g, dwg))
    return false;

  std::deque<VertexPair> cmpTodo;
  cmpTodo.push_back(start);

  std::vector<VertexPair> visited;

  Graph::vDesc gDst;
  DWG::vDesc dwgDst;
  VertexPair dst;

  label l;


  //for (int i = 0; i < 5; i++) {
  while (!cmpTodo.empty()) {

    VertexPair vp = cmpTodo.front();
    cmpTodo.pop_front();

    visited.push_back(vp);

    Graph::vDesc gv = vp.first;
    DWG::vDesc dwgv = vp.second;

    std::cerr << "working on: " << vertexPairToString(vp, g, dwg) << std::endl;

    Range<Graph::oeIter> oes = Graph::getOutEdges(g, gv);

    for (const Graph::eDesc &e : oes) {

      l = g[e].label;
      std::cerr << "  checking label " << l << std::endl;

      gDst = Graph::getDst(gv, l, g);
      dwgDst = DWG::getDst(dwgv, lgm.find(l)->second, dwg);

      VertexPair dst(gDst, dwgDst);
      std::cerr << "  destination: " << vertexPairToString(dst, g, dwg) << std::endl;

      if (isExitCondition(dst, g, dwg)) {
        std::cerr << "  exit condition triggered. abort." << std::endl;
        return false;
      }

      if (alreadyVisited(dst, visited)) {
        std::cerr << "  destination already visited. continue." << std::endl;
        continue;
      }


      std::cerr << "  adding new destination\n";
      cmpTodo.push_back(dst);
      std::cerr << "\n";
    }

    std::cerr << "ToDo:\n";

    for (const VertexPair &vp : cmpTodo)
      std::cerr << "  " << vertexPairToString(vp, g, dwg) << std::endl;

    std::cerr << "\nVisited:\n";
    for (const VertexPair &vp : visited)
      std::cerr << "  " << vertexPairToString(vp, g, dwg) << std::endl;

    std::cerr << "\n\n\n\n\n";
  }





  return true;
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

  labelGroupingMap lgm1 = Alm::LabelGroupingMap(g1, Alm::Lhs(alm));
  labelGroupingMap lgm2 = Alm::LabelGroupingMap(g2, Alm::Rhs(alm));

  edgeLabelSet els1 = Alm::lgmFlatten(lgm1);
  edgeLabelSet els2 = Alm::lgmFlatten(lgm2);


  WG_t wg = WG::create(g1, g2, alm);
  //WG::print(wg);


  DWG_t dwg1 = DWG::createLhs(wg, els1);
//  DWG::print(dwg1, wg);


  DWG_t dwg2 = DWG::createRhs(wg, els2);
//  DWG::print(dwg2, wg);




//  g1[1].role = "end";

  std::cout << "g1 vs dwgLhs: " << isEqual(g1, dwg1, lgm1) << std::endl;
  std::cout << "g2 vs dwgRhs: " << isEqual(g2, dwg2, lgm2) << std::endl;










  return 0;
}
