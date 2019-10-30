#include <cstdio>
#include <iostream>

#include "small_T_opt.hpp"

int main()
{
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
