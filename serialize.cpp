#include <meta>
#include <string>
#include <iostream>


// replicator:
namespace __impl {
  template<auto... vals>
  struct replicator_type {
    template<typename F>
      constexpr void operator>>(F body) const {
        (body.template operator()<vals>(), ...);
      }
  };

  template<auto... vals>
  replicator_type<vals...> replicator = {};
}

template<typename R>
consteval auto expand(R range) {
  std::vector<std::meta::info> args;
  for (auto r : range) {
    args.push_back(reflect_constant(r));
  }
  return substitute(^^__impl::replicator, args);
}
// replicator^


template<typename S>
constexpr std::string serialize(S s) {
  namespace r = std::meta;
  constexpr auto ctx = r::access_context::current();
  constexpr auto members = std::define_static_array(
      r::nonstatic_data_members_of(^^S, ctx)
    );
  std::string result = " ";
  [:expand(members):] >> [&]<auto m>{
    result += r::identifier_of(m);
    result += "=";
    if constexpr (type_of(m) == ^^int)
      result += std::string(s.[:m:] / 10, 'X');
    else
      result += s.[:m:];
    result += " ";
  };
  return result;
}


struct Person {
  std::string name;
  int age;
};

constexpr Person john{"John", 42};

static_assert(serialize(john) == " name=John age=XXXX ");

#include "structs.hpp"

int main() {
  using namespace std;
  string s = "john:"; 
  s += serialize(john);
  cout << s << endl;
  return 0;
}
