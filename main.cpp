#include <meta>

int main() {
  constexpr std::meta::info r = ^^int;
  typename[:r:] x = 42;
  typename[:^^char:] c = '*';
}
