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

#include <boost/tokenizer.hpp>

match Match::getMatch(const matchSet &ms, const alignment &alm, const label &l1, const label &l2)
{
  match m;
  matchSet res;

  /*
  // this code doesn't seem to have a purpose - smisch
  typedef boost::tokenizer<boost::char_separator<char>> tokenizer;

  boost::char_separator<char> sep{" | "};
  tokenizer tok{l2, sep};
  */

#if ISO_LIB_ENFORCE_MAXIMALITY
#endif
#if true

  if (ms.empty())
  {
    for (const alignmentPair &p : alm) {
      if ((Alm::hasLabel(p.first, l1)) && (Alm::hasLabel(p.second, l2)))
        res.push_back(p);
    }
  }
  else
  {
    for (const alignmentPair &p : alm) {
      if ((Alm::hasLabel(p.first, l1)) && (Alm::hasLabel(p.second, l2)) &&
          !Match::setHasMatch(ms, p) && Match::hasMatchWithBothGroupsDifferentThan(ms, p))
        res.push_back(p);
    }
  }



#else

  for (const alignmentPair &p : alm) {
    if ((Alm::hasLabel(p.first, l1)) && (Alm::hasLabel(p.second, l2)))
      res.push_back(p);
  }
#endif

  if (res.empty())
    return m;

  return res[0];
}

matchSet Match::getMatchSet(const alignment &alm, const matchSet &ms,
                                          const label &l1, const label &l2)
{
  matchSet res;

  match m = Match::getMatch(ms, alm, l1, l2);

  if (Match::empty(m))
    return res;

  if (!Match::setHasMatch(ms, m))
    res.push_back(m);

  return res;
}

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
