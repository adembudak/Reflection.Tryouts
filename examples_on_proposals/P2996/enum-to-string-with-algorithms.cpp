#include <algorithm>
#include <meta>
#include <print>
#include <ranges>
#include <string_view>
#include <vector>

template <typename E>
  requires std::is_enum_v<E>
constexpr std::string_view enum_to_string(E value) {

  auto get_pairs = [] {
    return std::meta::enumerators_of(^^E) | std::views::transform([](std::meta::info e) {
             return std::pair<E, std::string_view>(std::meta::extract<E>(e), std::meta::identifier_of(e));
           });
  };

  auto get_name = [get_pairs](E value) -> std::optional<std::string_view> {
    auto enumerators = get_pairs() | std::ranges::to<std::vector>();
    auto it = std::ranges::find_if(enumerators, [value](const auto &pr) { return pr.first == value; });
    if(it == enumerators.end()) {
      return std::nullopt;
    } else {
      return it->second;
    }
  };

  return get_name(value).value_or("<unnamed>");
}

int main() {
  enum Color : int;
  std::println("Color 0: {}", enum_to_string(Color(0))); // prints '<unnamed>'

  enum Color : int { red, green, blue };
  std::println("Color 0: {}", enum_to_string(Color(0))); // prints 'red'
}
