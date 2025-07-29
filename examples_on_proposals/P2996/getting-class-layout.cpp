
#include <meta>

struct member_descriptor {
  std::size_t offset;
  std::size_t size;

  bool operator==(member_descriptor const &) const = default;
};

template <typename S>
consteval auto get_layout() {
  constexpr auto ctx = std::meta::access_context::current();
  constexpr size_t N = std::meta::nonstatic_data_members_of(^^S, ctx).size();
  auto members = std::meta::nonstatic_data_members_of(^^S, ctx);

  std::array<member_descriptor, N> layout;
  for(int i = 0; i < members.size(); ++i) {
    layout[i] = {.offset = static_cast<std::size_t>(std::meta::offset_of(members[i]).bytes),
                 .size = std::meta::size_of(members[i])};
  }
  return layout;
}

struct X {
  char a;
  int b;
  double c;
};

constexpr auto Xd = get_layout<X>();
static_assert(Xd.size() == 3);

static_assert(Xd[0] == member_descriptor{.offset = 0, .size = 1});
static_assert(Xd[1] == member_descriptor{.offset = 4, .size = 4});
static_assert(Xd[2] == member_descriptor{.offset = 8, .size = 8});

int main() {}
