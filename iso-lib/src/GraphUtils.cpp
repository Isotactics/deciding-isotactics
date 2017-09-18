#include <iostream>
#include <sstream>
#include <deque>
#include <unordered_map>

#include "GraphUtils.hpp"
#include "Utils.hpp"


Graph_t Graph::parse(const std::string &path)
{
  Graph_t g;

  boost::dynamic_properties dp(boost::ignore_other_properties);

  dp.property("node_id", boost::get(&VerticeProps::name, g));
  dp.property("role", boost::get(&VerticeProps::role, g));

  dp.property("label", boost::get(&EdgeProps::label, g));
  dp.property("lowlink", boost::get(&EdgeProps::lowlink, g));

  std::ifstream m(path);

  if (!m) {
    std::cout << "[Err] Graph::parse // ifstream open" << std::endl;
    return g;
  }

  boost::read_graphviz(m, g, dp);

  return g;
}

bool Graph::isFinalState(const Graph_t &g, const Graph::vDesc &vd)
{
  return (g[vd].role == "end");
}

Graph::vDesc Graph::getStart(const Graph_t &g)
{
  Graph::vDesc start;
  const Range<Graph::vIter> vertices = Util::makeRange(boost::vertices(g));

  for (const Graph::vDesc &v : vertices) {
    if (g[v].role == "start")
      start = v;
  }

  return start;
}


std::vector<Graph::vDesc> Graph::getEnds(const Graph_t &g)
{
  std::vector<Graph::vDesc> res;

  const Range<Graph::vIter> vertices = Util::makeRange(boost::vertices(g));

  for (const Graph::vDesc &v : vertices) {
    if (g[v].role == "end")
      res.push_back(v);
  }

  return res;
}


Range<Graph::oeIter> Graph::getOutEdges(const Graph_t &g, const Graph::vDesc &v)
{
  return Util::makeRange(boost::out_edges(v, g));
}

std::vector<Graph::eDesc> Graph::getOutEdgesVec(const Graph_t &g, const Graph::vDesc &v)
{
  std::vector<Graph::eDesc> res;

  Range<Graph::oeIter> outEdges = Util::makeRange(boost::out_edges(v, g));

  for (const Graph::eDesc &oe : outEdges)
    res.push_back(oe);

  return res;
}

Graph::vDesc Graph::getVertex(const std::string &vName, const Graph_t &g)
{
  Graph::vDesc res;
  const Range<Graph::vIter> vertices = Util::makeRange(boost::vertices(g));

  for (const Graph::vDesc &v : vertices) {
    if (g[v].name == vName)
      res = v;
  }

  return res;
}

Graph::vDesc Graph::getDst(const Graph::vDesc &v, const std::string &l, const Graph_t &g)
{
  Graph::vDesc dst;

  Range<Graph::oeIter> oes = Graph::getOutEdges(g, v);

  for (const Graph::eDesc &e : oes) {
    if (g[e].label == l) {
      dst = boost::target(e, g);
      break;
    }
  }

  return dst;
}

/**
 * @deprecated in favor of Graph::getDestinationsWithEpsilonClosure
 */
std::set<Graph::vDesc>
Graph::getDestinations(const Graph_t &g, std::vector<Graph::eDesc> edges)
{
  std::set<Graph::vDesc> res;
  
  for (const Graph::eDesc &e : edges)
    res.insert(boost::target(e, g));
  
  return res;
}

std::set<Graph::vDesc>
Graph::getDestinationsWithEpsilonClosure(const Graph_t &g, std::vector<Graph::eDesc> edges)
{
  // set of vertices that are connected to as the target of the edges and via epsilon-closure
  std::set<Graph::vDesc> outputVertexSet;
  
  // queue of vertices that may be have further outgoing epsilon edges
  std::deque<Graph::vDesc> epsilonTodo;
  
  // hash map of vertices that we already visited. this map will be used to prevent processing
  // a vertex more than once.
  std::unordered_map<Graph::vDesc, bool> epsilonDone;
  
  for (const Graph::eDesc &e : edges)
  {
    auto target = boost::target(e, g);
    
    // add to vertex list
    outputVertexSet.insert(target);
    
    // add to todo list so that we check for epsilon edges
    epsilonTodo.push_back(target);
  }
  
  // now recursively check all vertices for epsilon-edges and add
  // every vertex that is connected this way (calculate epsilon closure).
  while (!epsilonTodo.empty())
  {
    // pop current item
    auto vertex = epsilonTodo.front();
    epsilonTodo.pop_front();
    
    // mark this vertex as done so that we don't push it into our todo queue once more
    epsilonDone.insert({vertex, true});
    
    // for all outgoing edges
    for (const auto &edgeDescriptor : Graph::getOutEdges(g, vertex))
    {
      auto edge = g[edgeDescriptor];
      
      // empty grouping on edge label means the label is considered an epsilon.
      // the grouping property is the set of all groups this label is contained in.
      // if the label is not contained in any group then this set of groups will be empty.
      if ( edge.gp.empty() )
      {
        // this target vertex is connected via epsilon edge
        auto target = boost::target(edgeDescriptor, g);
        
        // check first if we already did this vertex and only push it if not.
        if (epsilonDone.find(target) == epsilonDone.end())
        {
          // add to vertex list
          outputVertexSet.insert(target);
        
          // add to todo list so that we check for epsilon edges
          epsilonTodo.push_back(target);
        }
      }
    }
  }
  
  return outputVertexSet;
}
















void Graph::print(const Graph_t &g)
{
  std::cout << "digraph {" << std::endl;

  Range<Graph::vIter> vertices = Util::makeRange(boost::vertices(g));

  for (const Graph::vDesc &v : vertices) {
    if (g[v].role == "start") {
      std::cout << "  \"" << g[v].name << "\" [role=\"start\"]" << std::endl;
      continue;
    }

    if (g[v].role == "end") {
      std::cout << "  \"" << g[v].name << "\" [role=\"end\"]" << std::endl;
      continue;
    }
  }

  Util::printLine();

  for (const Graph::vDesc &v : vertices)
    Graph::printOutEdges(g, v);

  std::cout << "}\n\n" << std::endl;

  return;
}

void Graph::printVertices(const Graph_t &g)
{
  const Range<Graph::vIter> vertices = Util::makeRange(boost::vertices(g));

  for (const Graph::vDesc &v : vertices)
    std::cout << g[v].name << std::endl;

  return;
}

void Graph::printEdges(const Graph_t &g)
{
  const Range<Graph::eIter> edges = Util::makeRange(boost::edges(g));

  for (const Graph::eDesc &e : edges)
    std::cout << g[e].label << std::endl;

  return;
}

void Graph::printOutEdge(const Graph_t &g, const eDesc &e)
{
  const Graph::vDesc src = boost::source(e, g);
  const Graph::vDesc dst = boost::target(e, g);

  std::cout << "  \"" << g[src].name << "\" -> \"" << g[dst].name << "\"";
  std::cout << " [label=\"" << g[e].label << " gp: " << Alm::groupingToStr(g[e].gp) << "\"]" << std::endl;
}

void Graph::printOutEdges(const Graph_t &g, const Graph::vDesc &vd) {

  const Range<Graph::oeIter> outEdges = Util::makeRange(boost::out_edges(vd, g));

  if (outEdges.empty())
    return;

  for (const Graph::eDesc &e : outEdges)
    Graph::printOutEdge(g, e);

  return;
}


