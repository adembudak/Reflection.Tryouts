#include <meta>
#include <type_traits>
#include <memory>

using namespace std::meta;

template <typename... Ts>
class Variant {
  union Storage;
  struct Empty {};

  consteval {
    define_aggregate(^^Storage, { data_member_spec(^^Empty, { .name = "empty"}), data_member_spec(^^Ts)...}); }

  static consteval std::meta::info get_nth_field(std::size_t n) {
    auto ctx = std::meta::access_context::current();
    return nonstatic_data_members_of(^^Storage, ctx)[n + 1];
  }

  Storage storage_;
  int index_ = -1;

  // cheat: use libstdc++'s implementation
  template <typename T>
  static constexpr size_t accepted_index = std::__detail::__variant::__accepted_index<T, std::variant<Ts...>>;

  template <class F>
  constexpr auto with_index(F &&f) const -> decltype(auto) {
    return mp_with_index<sizeof...(Ts)>(index_, (F &&)f);
  }

public:
  constexpr Variant()
    requires std::is_default_constructible_v<Ts...[0]>
      // should this work: storage_{. [: get_nth_field(0) :]{} }
      :
      storage_{.empty = {}},
      index_(0) {
    std::construct_at(&storage_.[:get_nth_field(0):]);
  }

  constexpr ~Variant()
    requires(std::is_trivially_destructible_v<Ts> and ...)
  = default;
  constexpr ~Variant() {
    if(index_ != -1) {
      with_index([&](auto I) { std::destroy_at(&storage_.[:get_nth_field(I):]); });
    }
  }

  template <typename T, size_t I = accepted_index<T &&>>
    requires(!std::is_base_of_v<Variant, std::decay_t<T>>)
  constexpr Variant(T &&t) :
      storage_{.empty = {}},
      index_(-1) {
    std::construct_at(&storage_.[:get_nth_field(I):], (T &&)t);
    index_ = (int)I;
  }

  // you can't actually express this constraint nicely until P2963
  constexpr Variant(Variant const &)
    requires(std::is_trivially_copyable_v<Ts> and ...)
  = default;
  constexpr Variant(Variant const &rhs)
    requires((std::is_copy_constructible_v<Ts> and ...) and not(std::is_trivially_copyable_v<Ts> and ...))
      :
      storage_{.empty = {}},
      index_(-1) {
    rhs.with_index([&](auto I) {
      constexpr auto field = get_nth_field(I);
      std::construct_at(&storage_.[:field:], rhs.storage_.[:field:]);
      index_ = I;
    });
  }

  constexpr auto index() const -> int {
    return index_;
  }

  template <class F>
  constexpr auto visit(F &&f) const -> decltype(auto) {
    if(index_ == -1) {
      throw std::bad_variant_access();
    }

    return mp_with_index<sizeof...(Ts)>(
        index_, [&](auto I) -> decltype(auto) { return std::invoke((F &&)f, storage_.[:get_nth_field(I):]); });
  }
};

int main() {}
