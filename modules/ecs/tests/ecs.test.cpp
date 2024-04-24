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
const Type& type<Position>() {
    static Type ty("Position", sizeof(Position));
    return ty;
}

template<>
const Type& type<Velocity>() {
    static Type ty("Velocity", sizeof(Velocity));
    return ty;
}
} // namespace triple::refl

using namespace triple;

TEST_CASE("GenericQuery", "[ecs][query]") {
    ecs::Archetype a;
    a.m_components.push_back(&refl::type<Position>());
    a.m_components.push_back(&refl::type<Velocity>());

    ecs::GenericQuery q({&refl::type<Position>(), &refl::type<Velocity>()});
    q.add_if_matches(&a);
    REQUIRE(q.matched().size() == 1);
}

ecs::World world;

TEST_CASE("World & E-C-S", "[ecs]") {
    ecs::Entity e = world.entity();

    world.add_component<Position>(e);
    world.add_component<Velocity>(e);

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

    auto q1 = world.query<Position, Velocity>();
    REQUIRE(q1.matched().size() == 1);
    auto iter1 = q1.iter();
    do {
        auto& pos = iter1.get(refl::type<Position>()).value<Position>();
        auto& vel = iter1.get(refl::type<Velocity>()).value<Velocity>();
        REQUIRE(pos.x == 5.0f);
        REQUIRE(pos.y == 6.0f);
        REQUIRE(vel.x == 7.0f);
        REQUIRE(vel.y == 8.0f);
    } while (iter1.next());

    auto q2 = world.query<Position>();
    REQUIRE(q2.matched().size() == 2);

    auto q3 = world.query<Velocity>();
    REQUIRE(q3.matched().size() == 1);

    auto& s1 = world.system().add_query<Position, Velocity>().callback([](ecs::SystemCommands& commands) {
        auto& q = commands.query();
        auto iter = q.iter();
        do {
            auto& pos = iter.get(refl::type<Position>()).value<Position>();
            auto& vel = iter.get(refl::type<Velocity>()).value<Velocity>();
            REQUIRE(pos.x == 5.0f);
            REQUIRE(pos.y == 6.0f);
            REQUIRE(vel.x == 7.0f);
            REQUIRE(vel.y == 8.0f);
            pos.x = 9.0f;
            pos.y = 10.0f;
        } while (iter.next());
    });
    s1.run();

    // auto& s2 = world.system({&refl::type<Position>()}).each([](ecs::Iterator iter) {
    //     auto& pos = iter.field(refl::type<Position>()).value<Position>();
    //     REQUIRE(pos.x == 9.0f);
    //     REQUIRE(pos.y == 10.0f);
    // });
    // s2.run();
}

void sys(ecs::Query<Position, Velocity> q) {
    auto iter = q.iter();
    do {
        auto& pos = iter.get<Position>();
        auto& vel = iter.get<Velocity>();
        REQUIRE(pos.x == 9.0f);
        REQUIRE(pos.y == 10.0f);
        REQUIRE(vel.x == 7.0f);
        REQUIRE(vel.y == 8.0f);
    } while (iter.next());
}

TEST_CASE("System", "[ecs]") {
    auto& s2 = world.system(sys);
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