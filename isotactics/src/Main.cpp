#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include "GraphUtils.h"
#include "AlignmentUtils.h"




void printNewLine()
{
  std::cout << std::endl;
}

int main()
{
  Graph g1 = parseGraph("resources/m1.dot");
  Graph g2 = parseGraph("resources/m2.dot");

  printGraph(g1);
  printNewLine();

  printGraph(g2);
  printNewLine();

  alignment al = parseAlignment("resources/alignment.json");

  printAlignment(al);


  return 0;
}
