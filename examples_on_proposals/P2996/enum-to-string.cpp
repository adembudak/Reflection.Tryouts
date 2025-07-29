#include <meta>
#include <optional>
#include <print>
#include <string_view>

template <typename E, bool Enumerable = std::meta::is_enumerable_type(^^E)>
  requires std::is_enum_v<E>
constexpr std::string_view enum_to_string(E value) {
  if constexpr(Enumerable)
    template for(constexpr auto e : std::define_static_array(std::meta::enumerators_of(^^E))) if(value == [:e:]) //
        return std::meta::identifier_of(e);

  return "<unnamed>";
}

template <typename E, bool Enumerable = std::meta::is_enumerable_type(^^E)>
  requires std::is_enum_v<E>
constexpr std::optional<E> string_to_enum(std::string_view name) {
  if constexpr(Enumerable)
    template for(constexpr auto e : std::define_static_array(std::meta::enumerators_of(^^E))) //
        if(name == std::meta::identifier_of(e))                                               //
        return [:e:];
  return std::nullopt;
}

int main() {
  enum Color : int;
  static_assert(enum_to_string(Color(0)) == "<unnamed>");
  std::println("Color 0: {}", enum_to_string(Color(0))); // prints '<unnamed>'

  enum Color : int { red, green, blue };
  static_assert(enum_to_string(Color::red) == "red");
  static_assert(enum_to_string(Color(42)) == "<unnamed>");
  std::println("Color 0: {}", enum_to_string(Color(0))); // prints 'red'

  static_assert(string_to_enum<Color>("red").has_value());
  static_assert(*string_to_enum<Color>("red") == Color::red);
  // static_assert(string_to_enum<Color>("orange").has_value());
}
