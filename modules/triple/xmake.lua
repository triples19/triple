target("triple")
    set_kind("moduleonly")
    add_files("*.mpp")
    add_deps(
        "triple_app",
        "triple_base",
        "triple_core",
        "triple_ecs",
        "triple_graphics",
        "triple_graphics_opengl",
        "triple_math",
        "triple_refl",
        "triple_render2d",
        "triple_window"
    )