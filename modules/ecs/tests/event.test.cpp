#include <catch2/catch_test_macros.hpp>

import triple.ecs;
import triple.refl;
import triple.base;

using namespace triple;

struct MyEvent {
    int value;
};

namespace triple::refl {
template<>
const Type& type<MyEvent>() {
    static Type ty("MyEvent", sizeof(MyEvent));
    return ty;
}
} // namespace triple::refl

TEST_CASE("Event", "[event]") {
    ecs::Events events(refl::type<MyEvent>());
    events.send(MyEvent {42});
    ecs::GenericEventReader reader(events);
    auto evt = reader.next();
    REQUIRE(evt);
    REQUIRE(evt->cast<MyEvent>().value == 42);
}

TEST_CASE("Events & World", "[event]") {
    ecs::World world;
    world.add_event<MyEvent>();
    world
        .system(+[](ecs::EventWriter<MyEvent> writer) {
            writer.send(MyEvent {42});
        })
        .run();
    world
        .system(+[](ecs::EventReader<MyEvent> reader) {
            auto evt = reader.next();
            REQUIRE(evt);
            REQUIRE(evt->value == 42);
        })
        .run();
}