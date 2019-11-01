#include <iostream>

#define BOOST_MPL_LIMIT_STRING_SIZE 64

#include <boost/core/enable_if.hpp>
#include <boost/type_traits.hpp>

#include <boost/mpl/comparison.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/sizeof.hpp>
#include <boost/mpl/string.hpp>

using namespace boost;
using namespace boost::mpl;

template <typename T>
struct array_copier
{
  const T* const From;
  T* const To;

  array_copier(const T* const from, T* const to)
    : From(from), To(to) {}

  template <typename Integral>
  void operator()(const Integral Idx)
  {
    To[Idx] = From[Idx];
  }
};

template <typename T>
struct local
{
  local() { std::cout << "Default initialised locally.\n"; }
  local(const T t) : value(t) { std::cout << "Copy initialised locally.\n"; }
  local(const T* p) : value(*p) { std::cout << "Copy initialised from ptr locally.\n"; }
  T* get() { return &value; }

  T value;
};

template <typename T, long N>
struct local<T[N]>
{
  local() { std::cout << "Default initialised array locally.\n"; }
  local(const T* p)
  {
    std::cout << "Copying constructor array to local array...\n";
    for_each<
        range_c<long, 0, N> // generate indices
    >(array_copier<T>(p, value));
  }

  T* get() { return &value[0]; }

  T value[N];
};

template <typename T>
struct heap
{
  heap()
  {
    std::cout << "Allocating on heap...\n";
    ptr = new T{};
  }
  heap(const T t)
    : heap()
  {
    std::cout << "Copying to heap allocation.\n";
    *ptr = t;
  }
  heap(const T* p)
    : heap()
  {
    std::cout << "Copying to heap allocation from pointer...\n";
    *ptr = *p;
  }
  ~heap()
  {
    std::cout << "Destroying heap allocation.\n";
    delete ptr;
  }
  T* get() { return ptr; }

  T* ptr;
};

template <typename T, long N>
struct heap<T[N]>
{
  heap()
  {
    std::cout << "Allocating array on heap...\n";
    ptr = new T[N];
  }
  heap(const T* p)
    : heap()
  {
    std::cout << "Copying array to heap...\n";
    for_each<
        range_c<long, 0, N> // generate indices
    >(array_copier<T>(p, ptr));
  }
  ~heap()
  {
    std::cout << "Destroying array on heap...\n";
    delete[] ptr;
  }

  T* get() { return ptr; }

  T* ptr;
};

template <typename T>
struct get_trait
{
  typedef T* retty;

  template <typename Alloc>
  static retty get_element(Alloc& a) { return a.get(); }
};
template <typename T, long N>
struct get_trait<T[N]>
    // Cast away array-ness from the getter.
{
  typedef T* retty;

  template <typename Alloc>
  static retty get_element(Alloc& a) { return a.get(); }
};

template <typename T, long Treshold>
struct local_or_heap
{
  using allocation_type = typename if_<
      less_equal<
          sizeof_<T>,
          long_<Treshold>
      >,
      local<T>,
      heap<T>
    >::type;

  allocation_type alloc;

  typedef T value_type;

  local_or_heap() {}

  /* local_or_heap(const T t) participates in overload resolution only if
   * T isn't an array type.
   */
  template <typename U = T>
  explicit local_or_heap(
      const typename disable_if<is_array<T>, U>::type u)
    : alloc(u) {}

  /* local_or_heap(const T* p) participates in overload resolution only if
   * T *is* an array type.
   */
  template <typename U = T>
  explicit local_or_heap(
      typename enable_if<is_array<T>, typename decay<const U>::type>::type p)
    : alloc(p) {}

  typename get_trait<T>::retty get()
  {
    return get_trait<T>::get_element(alloc);
  }
};

template <typename T, long Treshold>
using LOH = local_or_heap<T, Treshold>;

