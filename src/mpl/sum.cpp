#include <iostream>

#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/range_c.hpp>

using namespace boost::mpl;

template <typename Seq>
using Sum = typename accumulate<Seq, long_<0>, plus<_1, _2>>::type;

int main()
{
  // Range is [N, M)
  std::cout << Sum< range_c<long, 1, 2> >::value << '\n';
  std::cout << Sum< range_c<long, 1, 11> >::value << '\n';
  std::cout << Sum< range_c<long, 1, 101> >::value << '\n';
  std::cout << Sum< range_c<long, 1, 1001> >::value << '\n';
  /*std::cout << Sum< range_c<long, 1, 10001> >::value << '\n';*/
  /* exceeds template instantiation depth maximum of 900 */
}
