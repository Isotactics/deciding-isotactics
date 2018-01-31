#include <iostream>
#include "Utils.hpp"

#include "Logging.hpp"

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
  DEBUG << std::endl;
}
