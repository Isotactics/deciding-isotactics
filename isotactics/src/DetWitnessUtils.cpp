#include <string>

#include "DetWitnessUtils.hpp"
#include "WitnessUtils.hpp"


DWG::Vertex DWG::createVertex(const std::string &name, const std::string &role)
{
  DWG::Vertex nv;

  nv.name = name;
  nv.role = role;

  return nv;
}

void DWG::addWGVertex(const WG::vDesc &wgv, DWG::Vertex &dwgv, DWG_t &dwg)
{
  (dwg[dwgv].vs).push_back(wgv);

  return;
}

DWG::vDesc DWG::addVertex(const DWG::Vertex &v, DWG_t &dwg)
{
  DWG::vDesc nv = boost::add_vertex(dwg);

  dwg[nv].name = v.name;
  dwg[nv].role = v.role;
  dwg[nv].vs   = v.vs;

  return nv;
}





void print(const DWG_t &dwg)
{
  return;
}

