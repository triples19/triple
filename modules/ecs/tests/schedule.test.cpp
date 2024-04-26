#include <catch2/catch_test_macros.hpp>

import triple.ecs;
import triple.refl;
import triple.base;

using namespace triple;

int counter = 0;

TEST_CASE("Schedule", "[ecs]") {
    ecs::World world;
    world.add_schedule(0);
    world.add_system(
        0,
        +[]() {
            counter++;
        }
    );
    REQUIRE(counter == 0);
    world.run_schedule(0);
    REQUIRE(counter == 1);
}
