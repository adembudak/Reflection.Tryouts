
#include <meta>
#include <optional>
#include <print>
#include <ranges>
#include <sstream>
#include <string_view>
#include <vector>

struct Flags {
  bool use_short;
  bool use_long;
};

template <typename T, Flags flags>
struct Option {
  std::optional<T> initializer = {};
};

consteval auto spec_to_opts(std::meta::info opts, std::meta::info spec) -> std::meta::info {
  auto ctx = std::meta::access_context::current();

  std::vector<std::meta::info> new_members;
  for(std::meta::info member : std::meta::nonstatic_data_members_of(spec, ctx)) {
    auto type_new = std::meta::template_arguments_of(std::meta::type_of(member))[0];
    new_members.push_back(std::meta::data_member_spec(type_new, {.name = std::meta::identifier_of(member)}));
  }

  return std::meta::define_aggregate(opts, new_members);
}

struct Clap {
  template <typename Spec>
  auto parse(this Spec const &spec, int argc, char **argv) {
    std::vector<std::string_view> cmdline(argv + 1, argv + argc);

    struct Opts;
    consteval {
      spec_to_opts(^^Opts, ^^Spec);
    }
    Opts opts;

    auto constexpr ctx = std::meta::access_context::current();

    template for(constexpr auto elements : std::define_static_array(
                     std::views::zip(std::meta::nonstatic_data_members_of(^^Spec, ctx),
                                     std::meta::nonstatic_data_members_of(^^Opts, ctx)) |
                     std::views::transform([](auto z) { return std::pair(std::get<0>(z), std::get<1>(z)); }))) {

      auto const sm = elements.first;
      auto const om = elements.second;

      auto const &cur = spec.[:sm:];
      auto constexpr type = std::meta::type_of(om);

      auto it = std::ranges::find_if(cmdline, [&](std::string_view arg) {
        return (cur.use_short && arg.size() == 2 && arg[0] == '-' && arg[1] == std::meta::identifier_of(sm)[0]) ||
               (cur.use_long && arg.starts_with("--") && arg.substr(2) == std::meta::identifier_of(sm));
      });

      if(it == cmdline.end()) {
        if constexpr(std::meta::has_template_arguments(type) and std::meta::template_of(type) == ^^std::optional) {
          continue;
        } else if(cur.initializer) {
          opts.[:om:] = *cur.initializer;
          continue;
        } else {
          std::print(stderr, "Missing required option {}\n", display_string_of(sm));
          std::exit(EXIT_FAILURE);
        }
      } else if(it + 1 == cmdline.end()) {
        std::print(stderr, "Option {} for {} is missing a value\n", *it, display_string_of(sm));
        std::exit(EXIT_FAILURE);
      }

      auto iss = std::istringstream(it[1]);
      if(iss >> opts.[:om:]; !iss) {
        std::print(stderr, "Failed to parse {:?} into option {} of type {}\n", it[1], display_string_of(sm),
                   display_string_of(type));
        std::exit(EXIT_FAILURE);
      }
    }
    return opts;
  }
};

struct Args : Clap {
  Option<std::string, {.use_short = true, .use_long = true}> name;
  Option<int, {.use_short = true, .use_long = true}> count = {1};
};

// clang-format off
int main(int argc, char **argv) {
 // auto opts = Args{}.parse(argc, argv);

 // for(int i = 0; i < opts.count; ++i) { // opts.count has type int
  //  std::print("Hello {}!", opts.name); // opts.name has type std::string
 // }
}
