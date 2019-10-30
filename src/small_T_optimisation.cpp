#include <cstdio>
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
  {
    // Forward the string itself to the allocation so it can be used at
    // runtime.
  }
};

template <typename S>
char* small_string<S>::copy_str::To = nullptr;

template <typename S>
long small_string<S>::copy_str::Idx = 0;

struct HUGE
{
  char S[1000];
};

int main()
{
  SOO<int, 1> heapInt(0);
  std::cout << *heapInt.get() << '\n'; // 0
  *heapInt.get() = 2;
  std::cout << *heapInt.get() << '\n'; // 2

  SOO<int, sizeof(int)> localInt(42);
  std::cout << *localInt.get() << '\n'; // 42

  SOO<HUGE, 42> hugeObj;
  hugeObj.get()->S[0] = 'a';
  hugeObj.get()->S[1] = 'b';
  hugeObj.get()->S[2] = 0;
  printf("%s\n", (char*)&(hugeObj.get()->S)); // ab

  // Would be fun with CTAD...?
  std::cout << size<string<'H', 'e', 'l', 'l', 'o'>>::value << '\n';
  small_string<string<'H', 'e', 'l', 'l', 'o'>> A;
  printf("%s\n", (char*)A.get()); // Hello

  A.get()[0] = 'B';
  printf("%s\n", (char*)A.get()); // Bello

  using Whisperity = string<'W', 'h', 'i', 's', 'p', 'e', 'r', 'i', 't', 'y'>;
  small_string<Whisperity> Wh;
  printf("%s\n", (char*)Wh.get()); // Whisperity
  Wh.get()[size<Whisperity>::value - 1] = 'i';
  printf("%s\n", (char*)Wh.get()); // Whisperiti

  using Wh2 = string<'Whi', 'speri', 'ty'>;
  std::cout << size<Wh2>::value << '\n'; // 9
  printf("%s\n", (char*)(small_string<Wh2>{}.get())); // Whisperity
}
