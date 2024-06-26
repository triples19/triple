target("triple_base")
    set_kind("moduleonly")
    add_files("*.mpp")
    add_includedirs(".", {public = true})

target("triple_base-tests")
    set_kind("binary")
    add_files("tests/*.test.cpp")
    add_packages("catch2")
    add_deps("triple_base")
