#include <meta>
#include <utility>
#include <vector>

template <typename T>
consteval std::meta::info make_integer_seq_refl(T N) {
  std::vector args{^^T};

  for(T k = 0; k < N; ++k) {
    args.push_back(std::meta::reflect_constant(k));
  }

  return std::meta::substitute(^^std::integer_sequence, args);
}

template <typename T, T N>
using make_index_sequence = [:make_integer_seq_refl<T>(N):];

static_assert(std::is_same_v<make_index_sequence<int, 2>, std::make_integer_sequence<int, 2>>);

int main() {}
