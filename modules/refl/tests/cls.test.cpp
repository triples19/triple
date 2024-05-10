#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>
using namespace std::string_literals;

#define NOMODULE
#include "refl_macros.hpp"

import triple.refl;
import triple.base;

struct Obj {
    int a;
};
REFL(Obj)

TEST_CASE("", "") {
    using namespace triple;
    using namespace triple::refl;
    Cls::new_cls<Obj>().add_member("a", &Obj::a);
    auto& c = cls<Obj>();
    auto& m = c.member("a");
    Obj obj;
    obj.a = 5;
    REQUIRE(type<int>().name() == "int"s);
    REQUIRE(m.name() == "a"s);
    REQUIRE(m.get(obj).value<int>() == 5);
    m.set(obj, 10);
    REQUIRE(obj.a == 10);
}

int main(int argc, char* argv[]) {
    return Catch::Session().run(argc, argv);
}
