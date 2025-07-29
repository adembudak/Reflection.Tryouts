
#include <algorithm>
#include <array>
#include <meta>

constexpr std::array types = {^^int, ^^float, ^^double};

constexpr std::array sizes = [] {
  std::array<std::size_t, types.size()> r;
  std::ranges::transform(types, r.begin(), std::meta::size_of);

  return r;
}();

static_assert(sizes == std::array{sizeof(int), sizeof(float), sizeof(double)});

int main() {}
