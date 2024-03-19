target("triple_refl")
    set_kind("moduleonly")
    add_files("*.cppm")

target("triple_refl-tests")
    set_kind("binary")
    add_files("tests/*.test.cpp")
    add_packages("catch2")
    add_deps("triple_refl")
