#include <iostream>
#include <string>

class Student {
public:
  /**
  * Ctor.
  */
  Student(const std::string &name);
  void printName(void);

private:
  std::string _name;

};
