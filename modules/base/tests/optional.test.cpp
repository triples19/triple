#include <catch2/catch_test_macros.hpp>

import triple.base;

struct Object {
    int x;
    Object(int x) : x(x) {}
};

TEST_CASE("Optional", "[optional]") {
    using namespace triple;
    using namespace triple::base;

    SECTION("Constructor") {
        Optional<int> opt;
        REQUIRE(!opt.has_value());

        Optional<int> opt2(42);
        REQUIRE(opt2.has_value());
        REQUIRE(opt2.value() == 42);

        Optional<int> opt3(opt2);
        REQUIRE(opt3.has_value());
        REQUIRE(opt3.value() == 42);

        Optional<int> opt4(std::move(opt2));
        REQUIRE(opt4.has_value());
        REQUIRE(opt4.value() == 42);

        Optional<int> opt5(nullopt);
        REQUIRE(!opt5.has_value());
    }

    SECTION("in_place") {
        Optional<Object> opt(in_place, 42);
        REQUIRE(opt.has_value());
        REQUIRE(opt.value().x == 42);
    }

    SECTION("Assign") {
        Optional<int> opt;
        opt = 42;
        REQUIRE(opt.has_value());
        REQUIRE(opt.value() == 42);

        Optional<int> opt2;
        opt2 = opt;
        REQUIRE(opt2.has_value());
        REQUIRE(opt2.value() == 42);

        Optional<int> opt3;
        opt3 = std::move(opt2);
        REQUIRE(opt3.has_value());
        REQUIRE(opt3.value() == 42);
        REQUIRE(!opt2.has_value());
    }

    SECTION("reset") {
        Optional<int> opt(42);
        opt.reset();
        REQUIRE(!opt.has_value());
    }

    SECTION("value") {
        Optional<Object> opt(Object(42));
        REQUIRE(opt.value().x == 42);
        REQUIRE((*opt).x == 42);
        REQUIRE(opt->x == 42);
    }

    SECTION("value_or") {
        Optional<int> opt;
        REQUIRE(opt.value_or(42) == 42);

        Optional<int> opt2(42);
        REQUIRE(opt2.value_or(0) == 42);
    }

    SECTION("and_then") {
        Optional<int> opt(42);
        auto res = opt.and_then([](int x) -> Optional<int> {
            return x + 1;
        });
        REQUIRE(res.has_value());
        REQUIRE(res.value() == 43);

        Optional<int> opt2;
        auto res2 = opt2.and_then([](int x) -> Optional<int> {
            return x + 1;
        });
        REQUIRE(!res2.has_value());
    }

    SECTION("transform") {
        Optional<int> opt(42);
        auto res = opt.transform([](int x) -> int {
            return x + 1;
        });
        REQUIRE(res.has_value());
        REQUIRE(res.value() == 43);

        Optional<int> opt2;
        auto res2 = opt2.transform([](int x) -> int {
            return x + 1;
        });
        REQUIRE(!res2.has_value());
    }

    SECTION("or_else") {
        Optional<int> opt(42);
        auto res = opt.or_else([]() -> Optional<int> {
            return 0;
        });
        REQUIRE(res.has_value());
        REQUIRE(res.value() == 42);

        Optional<int> opt2;
        auto res2 = opt2.or_else([]() -> Optional<int> {
            return 0;
        });
        REQUIRE(res2.has_value());
        REQUIRE(res2.value() == 0);
    }
}
