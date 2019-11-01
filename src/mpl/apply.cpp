#include <iostream>

#include <boost/type_traits.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/reverse.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>

using namespace boost;
using namespace boost::mpl;

struct add_pointer_
{
  template <typename T>
  struct apply
  {
    typedef T* type;
  };
};

template <typename F, typename... Args>
using apply_t = typename apply<F, Args...>::type;

using all_types = vector<int, long, char, short, void>;

using all_types_ptr = transform<all_types, add_pointer_>::type;
using all_types_ptr_2 = transform<all_types_ptr, add_pointer_>::type;
using all_types_ptr_3 = transform<all_types_ptr_2, add_pointer_>::type;

using reversed = reverse<all_types_ptr_3>::type;
using reverse_head = begin<reversed>::type;
using reverse_tail = prior<end<reversed>::type>::type;

int main()
{
  BOOST_MPL_ASSERT(( is_same< apply_t<add_pointer_, int>, int *> ));
  BOOST_MPL_ASSERT(( is_same< deref<reverse_head>::type, void ***> ));
  BOOST_MPL_ASSERT(( is_same< deref<reverse_tail>::type, int ***> ));
}
