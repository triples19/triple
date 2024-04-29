target("triple_graphics")
    set_kind("moduleonly")
    add_files("*.mpp", {public = true})
    -- add_files("*.cpp")
    add_deps("triple_refl", "triple_math", "triple_app", "triple_base")
    add_packages("stb")

includes("opengl")
