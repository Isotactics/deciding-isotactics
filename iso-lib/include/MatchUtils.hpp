#ifndef __MATCHUTILS_HPP__
#define __MATCHUTILS_HPP__

#include "AlignmentUtils.hpp"
#include "Utils.hpp"


using match             = alignmentPair;
using matchSet          = alignment;


namespace Match {


  bool hasLabel(const match &m, const label &l);
  bool setHasMatch(const matchSet &ms, const match &m);
  bool hasMatchWithBothGroupsDifferentThan(const matchSet &matchSet, const match &match);

  bool empty(const match &m);

  std::string toString(const match &m);
  std::string setToString(const matchSet &m);


  matchSet getMatchSet(const alignment &alm, const matchSet &ms, const label &l1, const label &l2);

  matchSet getMatchSet2(const matchSet &ms, const label &l);





  void print(const match &m);
  void printSet(const matchSet &ms);



/*







*/

}




#endif // __MATCHUTILS_HPP__
