#include <meta>
#include <string>
#include <sstream>
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
std::string serialize(S s) {
  namespace r = std::meta;
  constexpr auto ctx = r::access_context::current();
  constexpr auto members = std::define_static_array(
      r::nonstatic_data_members_of(^^S, ctx)
    );
  std::stringstream ss;
  [:expand(members):] >> [&]<auto m>{
    ss << r::identifier_of(m);
    ss << ": " << s.[:m:];
    ss << std::endl;
  };
  return ss.str(); 
}


struct Person {
  std::string name;
  int age;
};

constexpr Person john{"John", 42};

#include "structs.hpp"

int main() {
  using namespace std;
  cout << serialize(john);
  return 0;
}
