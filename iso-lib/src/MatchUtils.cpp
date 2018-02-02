#include <sstream>
#include <boost/algorithm/string/join.hpp>

#include "MatchUtils.hpp"


bool Match::hasLabel(const match &m, const label &l)
{
  return Alm::hasLabel(m.first, l) || Alm::hasLabel(m.second, l);
}

/**
 * return true iff match ma is an element of match set ms.
 */
bool Match::setHasMatch(const matchSet &ms, const match &ma)
{
  for (const match &m : ms) {
    if (m == ma)
      return true;
  }

  return false;
}

/**
 * For the case that both K_1 != \emptyset and K_2 != \emptyset check if there is a
 * match (G'_1, G'_2) in M with both G'_1 != G_1 and G'_2 != G_2, where (G_1, G_2) is
 * the passed argument ma.
 */
bool Match::hasMatchWithBothGroupsDifferentThan(const matchSet &matchSet, const match &match)
{
  for (auto &currentMatch : matchSet) {
    if (match.first != currentMatch.first && match.second != currentMatch.second)
    {
      // found a match with both G'_1 != G_1 and G'_2 != G_2
      return true;
    }
  }

  // didn't find a match with both G'_1 != G_1 and G'_2 != G_2
  return false;
}

bool Match::empty(const match &m)
{
  return (m.first).empty();
}

std::string Match::toString(const match &m)
{
  std::stringstream res;

  res << "(";
  res << boost::algorithm::join(m.first, ",");
  res << " <> ";
  res << boost::algorithm::join(m.second, ",");
  res << ")";

  return res.str();
}

std::string Match::setToString(const matchSet &ms)
{
  std::stringstream res;
  matchSet::const_iterator it;

  if (ms.empty()) {
    res << "{}";
    return res.str();
  }

  res << "{";

  for (it = ms.begin(); it != ms.end() - 1; ++it) {
    res << Match::toString(*it);
    res << ",";
  }

  res << Match::toString(*it);
  res << "}";

  return res.str();
}

/**
 * Get a new match set according to the first rule of the witness graph construction,
 * which assumes that both K_i != \emptyset.
 * We use the old match set ms, the alignment alm and the two labels
 * that were chosen to advance each FSM ( K_1 = G(l1) etc. ).
 */
matchSet Match::getMatchSet(const alignment &alm, const matchSet &ms,
                                          const label &l1, const label &l2)
{
  // create empty result match set
  matchSet resultMatchSet;

#if ISO_LIB_ENFORCE_MAXIMALITY
  
  if (ms.empty())
  {
    // M defined according to first case (of first rule).
    // previous M was empty, therefore new M will be K_1 x K_2 \cap \alignment
    for (const alignmentPair &p : alm)
    {
      // include all alignment pairs where the symbols l1 and l2 are included on each side respectively
      if ((Alm::hasLabel(p.first, l1)) && (Alm::hasLabel(p.second, l2)))
      {
        resultMatchSet.push_back(p);
      } // if
    } // for
  } // if ms.empty
  
  else
  {
    // M defined according to second case (of first rule).
    // previous M was not empty and K_1 != \emptyset and K_2 != \emptyset
    for (const alignmentPair &p : alm)
    {
      // include all alignment pairs where the symbols l1 and l2 are included on each side respectively
      if ((Alm::hasLabel(p.first, l1)) && (Alm::hasLabel(p.second, l2))
          // exclude alignment pairs that are present in the previous match set
          && ! Match::setHasMatch(ms, p)
          // exclude this alignment pair if there isn't another pair in the previous match set
          // that has different groupings on both sides (G'_1 != G_1 and G'_2 != G_2).
          && Match::hasMatchWithBothGroupsDifferentThan(ms, p)
      )
      {
        resultMatchSet.push_back(p);
      } // if
    } // for
  } // if ms.empty
  
#else
  
  // original paper definition which does not enforce maximality.
  // it now also excludes pairs that where present in the previous match set, to fit the paper definition.
  for (const alignmentPair &p : alm) {
    // include all alignment pairs where the symbols l1 and l2 are included on each side respectively
    if ((Alm::hasLabel(p.first, l1)) && (Alm::hasLabel(p.second, l2))
        // exclude alignment pairs that are present in the previous match set
        && ! Match::setHasMatch(ms, p) )
    {
      resultMatchSet.push_back(p);
    } // if
  } // for
  
#endif

  return resultMatchSet;
}

/**
 * Get a new match set according to the second rule of the witness graph construction,
 * which assumes one of the K_i = \emptyset.
 * We use the old match set ms, the alignment alm and the two labels
 * that were chosen to advance each FSM ( K_1 = G(l1) etc. ).
 */
matchSet Match::getMatchSet2(const matchSet &ms, const label &l)
{
  matchSet res;

  for (const match &m : ms) {
    if (Match::hasLabel(m, l))
      res.push_back(m);
  }

  return res;
}


















void Match::print(const match &m)
{
  Alm::printGroup(m.first);
  std::cout << "\t<> ";
  Alm::printGroup(m.second);

  std::cout << std::endl;

  return;
}

void Match::printSet(const matchSet &ms)
{
  for (const match &m : ms) {
    std::cout << "  ";
    Match::print(m);
  }

  return;
}
