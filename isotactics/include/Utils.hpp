#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <utility>
#include <boost/range/iterator_range.hpp>


template <typename T>
using Range = boost::iterator_range<T>;


namespace Util {
  template <typename T>
  Range<T> makeRange(const std::pair<T, T> &p)
  {
    return boost::make_iterator_range(p);
  }

  void printLine();
}

#endif // __UTILS_HPP__
