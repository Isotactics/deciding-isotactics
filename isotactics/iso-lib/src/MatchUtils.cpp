#include <sstream>
#include <boost/algorithm/string/join.hpp>

#include "MatchUtils.hpp"


bool Match::hasLabel(const match &m, const label &l)
{
  return Alm::hasLabel(m.first, l) || Alm::hasLabel(m.second, l);
}

bool Match::setHasMatch(const matchSet &ms, const match &ma)
{
  for (const match &m : ms) {
    if (m == ma)
      return true;
  }

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

match Match::getMatch(const alignment &alm, const label &l1, const label &l2)
{
  match m;
  matchSet res;

  std::cout << "getMatch() l1: " << l1 << " l2: " << l2 << "\n";

  typedef boost::tokenizer<boost::char_separator<char>> tokenizer;

  boost::char_separator<char> sep{" | "};
  tokenizer tok{l2, sep};

  for (const std::string &t : tok)
    std::cout << t << '\n';







  for (const alignmentPair &p : alm) {
    if ((Alm::hasLabel(p.first, l1)) && (Alm::hasLabel(p.second, l2)))
      res.push_back(p);
  }

  if (res.empty())
    return m;

  return res[0];
}

matchSet Match::getMatchSet(const alignment &alm, const matchSet &ms,
                                          const label &l1, const label &l2)
{
  matchSet res;

  match m = Match::getMatch(alm, l1, l2);

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
