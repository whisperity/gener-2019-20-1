#include <cstdio>
#include <iostream>

#include "local_or_heap_T.hpp"

template <typename S>
using loh_string_base = local_or_heap< char[size<S>::value + 1], 8 >;

template <typename S> // S should me mpl::string...
struct loh_string : loh_string_base<S>
// Compile-time decides to put on stack or heap.
{
  loh_string() : loh_string_base<S>(c_str<S>::value)
  {
    // Forward the string itself to the allocation so it can be used at
    // runtime.
  }
};

int main()
{
  std::cout << size<string<'H', 'e', 'l', 'l', 'o'>>::value << '\n';
  loh_string<string<'H', 'e', 'l', 'l', 'o'>> A;
  printf("%s\n", (char*)A.get()); // Hello

  A.get()[0] = 'B';
  printf("%s\n", (char*)A.get()); // Bello

  using Whisperity = string<'W', 'h', 'i', 's', 'p', 'e', 'r', 'i', 't', 'y'>;
  loh_string<Whisperity> Wh;
  printf("%s\n", (char*)Wh.get()); // Whisperity
  Wh.get()[size<Whisperity>::value - 1] = 'i';
  printf("%s\n", (char*)Wh.get()); // Whisperiti

  using Wh2 = string<'Whi', 'speri', 'ty'>;
  std::cout << size<Wh2>::value << '\n'; // 9
  printf("%s\n", (char*)(loh_string<Wh2>{}.get())); // Whisperity
}
