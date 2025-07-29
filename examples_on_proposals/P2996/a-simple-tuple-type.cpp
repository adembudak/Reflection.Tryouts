#include <array>
#include <cassert>
#include <meta>
#include <tuple>

template <typename... Ts>
struct Tuple {
  struct storage;

  consteval {
    std::meta::define_aggregate(^^storage, {
                                               std::meta::data_member_spec(^^Ts)...});
  }
  storage data;

  Tuple() :
      data{} {}

  Tuple(Ts const &...vs) :
      data{vs...} {}
};

template <typename... Ts>
struct std::tuple_size<Tuple<Ts...>> : public std::integral_constant<std::size_t, sizeof...(Ts)> {};

template <std::size_t I, typename... Ts>
struct std::tuple_element<I, Tuple<Ts...>> {
  static constexpr std::array types = {^^Ts...};
  using type = [:types[I]:];
};

consteval std::meta::info get_nth_field(std::meta::info r, std::size_t n) {
  return std::meta::nonstatic_data_members_of(r, std::meta::access_context::current())[n];
}

template <std::size_t I, typename... Ts>
constexpr auto get(Tuple<Ts...> &t) noexcept -> std::tuple_element_t<I, Tuple<Ts...>> & {
  return t.data.[:get_nth_field(^^decltype(t.data), I):];
}

int main() {
  auto tuple = Tuple{1, 'c', 3.14};

  assert(get<0>(tuple) == 1);
  assert(get<1>(tuple) == 'c');
  assert(get<2>(tuple) == 3.14);
}
