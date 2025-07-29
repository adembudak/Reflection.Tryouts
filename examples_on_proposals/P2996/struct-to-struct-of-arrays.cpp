#include <array>
#include <meta>
#include <vector>

template <typename T, std::size_t N>
struct struct_of_arrays_impl {
  struct impl;

  consteval {
    auto ctx = std::meta::access_context::current();

    std::vector<std::meta::info> old_members = std::meta::nonstatic_data_members_of(^^T, ctx);
    std::vector<std::meta::info> new_members = {};

    for(std::meta::info member : old_members) {
      auto array_type = std::meta::substitute(^^std::array, { std::meta::type_of(member), std::meta::reflect_constant(N) });
      auto mem_descr = std::meta::data_member_spec(array_type, {.name = std::meta::identifier_of(member)});
      new_members.push_back(mem_descr);
    }

    std::meta::define_aggregate(^^impl, new_members);
  }
};

template <typename T, std::size_t N>
using struct_of_arrays = struct_of_arrays_impl<T, N>::impl;

struct point {
  float x;
  float y;
  float z;
};

using points = struct_of_arrays<point, 30>;

constexpr points p{};

static_assert(std::same_as<decltype(p.x), std::array<float, 30>>);
static_assert(std::same_as<decltype(p.y), std::array<float, 30>>);
static_assert(std::same_as<decltype(p.z), std::array<float, 30>>);

int main() {}
