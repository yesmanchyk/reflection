#include <cassert>
#include <iostream>
#include <string>
#include <sstream>

#include <boost/hana.hpp>

namespace hana = boost::hana;

struct Person {
  BOOST_HANA_DEFINE_STRUCT(Person,
    (std::string, name),
    (int, age)
  );
};

namespace not_my_namespace {
 struct Person {
    std::string name;
 int age;
  };
}
 
BOOST_HANA_ADAPT_STRUCT(not_my_namespace::Person, name, age);

template <typename T>
std::string serialize(const T& o) {
  std::stringstream ss;
  hana::for_each(o, [&](auto pair) {
    ss << hana::to<char const*>(hana::first(pair)) << ": "
       << hana::second(pair) << std::endl;
  });
  return ss.str(); 
}

#include "hanas.hpp"

int main() {
  Person john{"John", 30};
  std::cout << serialize(john); 
  not_my_namespace::Person george{"George", 42};
  std::cout << serialize(george);
  return 0;
}
