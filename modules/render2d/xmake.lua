target("triple_render2d")
    set_kind("moduleonly")
    add_files("*.mpp")
    add_deps("triple_graphics", "triple_core", "triple_window")
