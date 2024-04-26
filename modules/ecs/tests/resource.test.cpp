#include <catch2/catch_test_macros.hpp>

import triple.ecs;
import triple.refl;
import triple.base;

using namespace triple;

struct MyRes {
    int value {0};
};

namespace triple::refl {
template<>
const Type& type<MyRes>() {
    static Type ty("MyRes", sizeof(MyRes));
    return ty;
}
} // namespace triple::refl

TEST_CASE("Resource", "[ecs]") {
    ecs::World world;
    world.add_resource<MyRes>();
    REQUIRE(world.get_resource<MyRes>()->value == 0);
    world.get_resource<MyRes>()->value = 42;

    world
        .system(+[](ecs::Resource<MyRes> res, ecs::Commands commands) {
            REQUIRE(res->value == 42);
            res->value = 52;
        })
        .run();
    world
        .system(+[](ecs::Resource<MyRes> res) {
            REQUIRE(res->value == 52);
        })
        .run();
}
