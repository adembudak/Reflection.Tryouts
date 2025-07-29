#include <algorithm>
#include <iostream>
#include <meta>
#include <print>
#include <span>
#include <sstream>
#include <string>
#include <string_view>

template <typename Opts>
auto parse_options(std::span<std::string_view const> args) -> Opts {
  Opts opts;

  constexpr auto ctx = std::meta::access_context::current();
  template for(constexpr auto dm : std::define_static_array(std::meta::nonstatic_data_members_of(^^Opts, ctx))) {
    auto it = std::ranges::find_if(
        args, [](std::string_view arg) { return arg.starts_with("--") && arg.substr(2) == std::meta::identifier_of(dm); });

    if(it == args.end()) {
      continue;
    } else if(it + 1 == args.end()) {
      std::print(stderr, "Option {} is missing a value\n", *it);
      std::exit(EXIT_FAILURE);
    }

    using T = typename[:std::meta::type_of(dm):];
    auto iss = std::istringstream(it[1]);
    if(iss >> opts.[:dm:]; !iss) {
      std::print(stderr, "Failed to parse option {} into a {}\n", *it, std::meta::display_string_of(^^T));
      std::exit(EXIT_FAILURE);
    }
  }

  return opts;
}

struct MyOpts {
  std::string file_name = "input.txt"; // Option "--file_name <string>"
  int count = 1;                       // Option "--count <int>
};

int main(int argc, char *argv[]) {
  MyOpts opts = parse_options<MyOpts>(std::vector<std::string_view>(argv + 1, argv + argc));

  std::cout << "opts.file=" << opts.file_name << '\n';
  std::cout << "opts.count=" << opts.count << '\n';
}
