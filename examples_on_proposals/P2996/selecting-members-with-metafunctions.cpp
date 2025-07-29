#include <meta>

struct S {
  unsigned i : 2, j : 6;
};

consteval auto member_number(int n) {
  auto ctx = std::meta::access_context::current();
  return std::meta::nonstatic_data_members_of(^^S, ctx)[n];
}

int main() {
  S s{0, 0};
  s.[:member_number(1):] = 42;
  // s.[:member_number(5):] = 0;
}
