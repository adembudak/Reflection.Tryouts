#include <meta>

struct S {
  unsigned i : 2, j : 6;
};

consteval auto member_named(std::string_view name) {
  auto ctx = std::meta::access_context::current();
  for(std::meta::info field : std::meta::nonstatic_data_members_of(^^S, ctx)) {
    if(std::meta::has_identifier(field) && std::meta::identifier_of(field) == name) //
      return field;
  }
}

int main() {
  S s{0, 0};

  s.[:member_named("j"):] = 42;
  //  s.[:member_named("x"):] = 0;
}
