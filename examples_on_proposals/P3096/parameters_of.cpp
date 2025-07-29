#include <concepts>
#include <meta>

using namespace std;
using namespace std::meta;
using namespace std::string_view_literals;

int fun(const int a, int b);

static_assert(same_as<typename[:return_type_of(^^fun):], int>);

static_assert(same_as<typename[:type_of(parameters_of(^^fun)[0]):], int>);
static_assert(same_as<typename[:type_of(parameters_of(^^fun)[1]):], int>);

static_assert(!has_ellipsis_parameter(^^fun));
static_assert(!has_default_argument(parameters_of(^^fun)[0]));
static_assert(!is_explicit_object_parameter(parameters_of(^^fun)[0]));

static_assert(has_identifier(parameters_of(^^fun)[0]));
static_assert("a"sv == identifier_of(parameters_of(^^fun)[0]));
static_assert("b"sv == identifier_of(parameters_of(^^fun)[1]));

int main() {}
