#include <iostream>

#define BOOST_MPL_LIMIT_STRING_SIZE 64

#include <boost/mpl/comparison.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/sizeof.hpp>
#include <boost/mpl/string.hpp>

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
  T* get() { return &value; }

  T value;
};

template <typename T, long N>
struct local<T[N]>
{
  local() { std::cout << "Default initialised array locally.\n"; }
  local(const T t[N])
  {
    std::cout << "Copying constructor array to local array...\n";
    for_each<
        range_c<long, 0, N> // generate indices
    >(array_copier<T>(t, value));
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
  heap(const T t[N])
    : heap()
  {
    std::cout << "Copying array to heap...\n";
    for_each<
        range_c<long, 0, N> // generate indices
    >(array_copier<T>(t, ptr));
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
struct small_object_optimisation
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

  small_object_optimisation() {}
  small_object_optimisation(const T t) : alloc(t) {}

  typename get_trait<T>::retty get()
  {
    return get_trait<T>::get_element(alloc);
  }
};

template <typename T, long Treshold>
using SOO = small_object_optimisation<T, Treshold>;

template <typename S>
using small_string_base = SOO< char[size<S>::value + 1], 8 >;

template <typename S> // S should me mpl::string...
struct small_string : small_string_base<S>
// Compile-time decides to put on stack or heap.
{
  struct copy_str
  {
    // Helper functor that copies a string char-by-char to the runtime
    // buffer.
    static char* To;
    static long Idx;
    copy_str(char* ptr)
    {
      To = ptr;
    }

    void operator()(char c)
    {
      To[Idx] = c;
      ++Idx;
    }
  };

  small_string() : small_string_base<S>(
      const_cast<char*>(c_str<S>::value))
  // const_cast needed with GCC 7.4.0, and not with Clang 9.0!
  {
    // Forward the string itself to the allocation so it can be used at
    // runtime.
  }
};

