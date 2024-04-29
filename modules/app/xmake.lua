target("triple_app")
    set_kind("moduleonly")
    add_files("*.mpp")
    add_deps("triple_base", "triple_math", "triple_refl", "triple_ecs")
