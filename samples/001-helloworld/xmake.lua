target("001-helloworld")
    set_kind("binary")
    add_files("main.cpp")
    add_deps("triple_base", "triple_math", "triple_refl")
