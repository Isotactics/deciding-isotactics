#include <iostream>
#include "Utils.hpp"

void Util::printLine()
{
  std::cout << std::endl;
}

void Util::printLineFile(std::ostream& target)
{
  target << "\n";
}

void Util::printLineDebug()
{
  std::cerr << std::endl;
}
