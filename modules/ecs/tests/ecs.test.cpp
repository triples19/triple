#include <catch2/catch_test_macros.hpp>

import triple.ecs;
import triple.refl;
import triple.base;

struct Position {
    float x;
    float y;
};

struct Velocity {
    float x;
    float y;
};

namespace triple::refl {
template<>
const Type* type<Position>() {
    static Type ty("Position", sizeof(Position));
    return &ty;
}

template<>
const Type* type<Velocity>() {
    static Type ty("Velocity", sizeof(Velocity));
    return &ty;
}
} // namespace triple::refl

using namespace triple;

TEST_CASE("Query", "[ecs][query]") {
    ecs::Archetype a;
    a.m_components.push_back(refl::type<Position>());
    a.m_components.push_back(refl::type<Velocity>());

    ecs::Query q({refl::type<Position>(), refl::type<Velocity>()});
    q.add_if_matches(&a);
    REQUIRE(q.matched().size() == 1);
}

TEST_CASE("World & E-C-S", "[ecs]") {
    ecs::World world;
    ecs::Entity e = world.entity();

    world.add_component(e, refl::type<Position>());
    world.add_component(e, refl::type<Velocity>());

    refl::Ref c = world.get_component(e, refl::type<Position>());
    c.value<Position>().x = 5.0f;
    c.value<Position>().y = 6.0f;
    c = world.get_component(e, refl::type<Position>());
    REQUIRE(c.value<Position>().x == 5.0f);
    REQUIRE(c.value<Position>().y == 6.0f);

    c = world.get_component(e, refl::type<Velocity>());
    c.value<Velocity>().x = 7.0f;
    c.value<Velocity>().y = 8.0f;
    c = world.get_component(e, refl::type<Velocity>());
    REQUIRE(c.value<Velocity>().x == 7.0f);
    REQUIRE(c.value<Velocity>().y == 8.0f);

    auto q1 = world.query({refl::type<Position>(), refl::type<Velocity>()});
    REQUIRE(q1->matched().size() == 1);

    auto q2 = world.query({refl::type<Position>()});
    REQUIRE(q2->matched().size() == 2);

    auto q3 = world.query({refl::type<Velocity>()});
    REQUIRE(q3->matched().size() == 1);

    auto s1 = world.system({refl::type<Position>(), refl::type<Velocity>()})->each([](ecs::Iterator iter) {
        auto& pos = iter.field(refl::type<Position>()).value<Position>();
        auto& vel = iter.field(refl::type<Velocity>()).value<Velocity>();
        REQUIRE(pos.x == 5.0f);
        REQUIRE(pos.y == 6.0f);
        REQUIRE(vel.x == 7.0f);
        REQUIRE(vel.y == 8.0f);
        pos.x = 9.0f;
        pos.y = 10.0f;
    });
    s1.run();

    auto s2 = world.system({refl::type<Position>()})->each([](ecs::Iterator iter) {
        auto& pos = iter.field(refl::type<Position>()).value<Position>();
        REQUIRE(pos.x == 9.0f);
        REQUIRE(pos.y == 10.0f);
    });
    s2.run();
}

TEST_CASE("World & its template functions", "[ecs]") {
    ecs::World world;
    ecs::Entity e = world.entity();

    world.add_component(e, Position {5.0f, 6.0f});
    world.add_component(e, Velocity {7.0f, 8.0f});

    Position& pos = world.get_component<Position>(e);
    REQUIRE(pos.x == 5.0f);
    REQUIRE(pos.y == 6.0f);

    Velocity& vel = world.get_component<Velocity>(e);
    REQUIRE(vel.x == 7.0f);
    REQUIRE(vel.y == 8.0f);
}