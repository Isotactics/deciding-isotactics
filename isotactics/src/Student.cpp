#include "Student.h"

Student::Student(const std::string &name)
  : _name{name}
{}


void Student::printName(void)
{
  std::cout << _name << std::endl;
}
