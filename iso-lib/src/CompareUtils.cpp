#include <string>
#include <boost/algorithm/string/join.hpp>

#include "CompareUtils.hpp"

#include "Logging.hpp"

bool Cmp::isExitCondition(const Cmp::VertexPair &v, const DG_t &g, const DWG_t &dwg)
{
  DG::vDesc gv = v.gv;
  DWG::vDesc dwgv = v.dwgv;

  if ((g[gv].role == "end") && (dwg[dwgv].role != "end"))
    return true;

  return false;
}

bool Cmp::alreadyVisited(const Cmp::Vertex &v, std::vector<Cmp::Vertex> visited)
{

  for (const Cmp::Vertex &vv : visited) {
    if (v.pair == vv.pair)
      return true;
  }

  return false;
}

bool Cmp::isEqual(const DG_t &g, const DWG_t &dwg, const labelGroupingMap &lgm) {

  DG::vDesc gStart = DG::getStart(g);
  DWG::vDesc dwgStart = DWG::getStart(dwg);

  Cmp::Vertex start(gStart, dwgStart);

  if (isExitCondition(start.pair, g, dwg))
    return false;

  std::deque<Cmp::Vertex> cmpTodo;
  cmpTodo.push_back(start);

  std::vector<Cmp::Vertex> visited;


  //for (int i = 0; i < 5; i++) {
  while (!cmpTodo.empty()) {

    Cmp::Vertex curr = cmpTodo.front();
    cmpTodo.pop_front();

    visited.push_back(curr);

    DG::vDesc gv = curr.pair.gv;
    DWG::vDesc dwgv = curr.pair.dwgv;

    DEBUG << "working on: " << Cmp::vpToString(curr.pair, g, dwg) << std::endl;

    Range<DG::oeIter> oes = DG::getOutEdges(g, gv);

    for (const DG::eDesc &e : oes) {

      label l = g[e].label;

      DEBUG << "  checking label " << l << std::endl;

      DG::vDesc gDst = DG::getDst(gv, l, g);
      DWG::vDesc dwgDst = DWG::getDst(dwgv, lgm.find(l)->second, dwg);

      Cmp::Vertex dst(gDst, dwgDst);
      Cmp::inheritPath(dst, curr);

      DEBUG << "  destination: " << Cmp::vpToString(dst.pair, g, dwg) << std::endl;

      if (isExitCondition(dst.pair, g, dwg)) {
        std::cout << "\nexit condition at: " << vertexToString(dst, g, dwg) << std::endl;
        return false;
      }

      if (alreadyVisited(dst, visited)) {
        DEBUG << "  destination already visited. continue.\n\n";

        continue;
      }

      DEBUG << "  adding new destination\n\n";

      cmpTodo.push_back(dst);
    }

    IF_DEBUG(
      std::cerr << "ToDo:\n";

      for (const Cmp::Vertex &v : cmpTodo)
        std::cerr << Cmp::vpToString(v.pair, g, dwg) << std::endl;

      std::cerr << "\nVisited:\n";
      for (const Cmp::Vertex &v : visited)
        std::cerr << Cmp::vpToString(v.pair, g, dwg) << std::endl;

      std::cerr << "\n\n\n\n\n";
    )
  }

  return true;
}

void Cmp::inheritPath(Cmp::Vertex &v, const Cmp::Vertex &vpath)
{
  for (const VertexPair &vp : vpath.path)
    v.path.push_back(vp);

  v.path.push_back(vpath.pair);
}

std::string Cmp::vpToString(const Cmp::VertexPair &vp, const DG_t &g, const DWG_t &dwg)
{
  DG::vDesc gv = vp.gv;
  DWG::vDesc dwgv = vp.dwgv;

  std::string res = "";
  res +=  g[gv].name;
  res += ", ";
  res += dwg[dwgv].name;

  return res;
}

std::string Cmp::vertexToString(const Cmp::Vertex &v, const DG_t &g, const DWG_t &dwg)
{
  std::string res = Cmp::vpToString(v.pair, g, dwg);


  std::vector<std::string> tmp;

  for (const Cmp::VertexPair &p : v.path)
    tmp.push_back(Cmp::vpToString(p, g, dwg));

  res += "  [" + boost::algorithm::join(tmp, " -> ") + "]";

  return res;
}
