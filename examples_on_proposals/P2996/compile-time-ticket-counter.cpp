#include <meta>

template <int N>
struct Helper;

struct TU_Ticket {
  static consteval int latest() {
    int k = 0;

    // clang-format off
    while(std::meta::is_complete_type(std::meta::substitute(^^Helper, { std::meta::reflect_constant(k) })))
      ++k;

    return k;
  }

  static consteval void increment() {
    std::meta::define_aggregate(std::meta::substitute(^^Helper, { std::meta::reflect_constant(latest())}), {});
  }
  // clang-format on
};

constexpr int x = TU_Ticket::latest(); // x initialized to 0.

consteval {
  TU_Ticket::increment();
}
constexpr int y = TU_Ticket::latest(); // y initialized to 1.

consteval {
  TU_Ticket::increment();
}
constexpr int z = TU_Ticket::latest(); // z initialized to 2.

static_assert(x == 0);
static_assert(y == 1);
static_assert(z == 2);

int main() {}
