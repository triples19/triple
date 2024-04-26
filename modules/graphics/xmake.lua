target("triple_graphics")
    set_kind("static")
    add_files("*.mpp", {public = true})
    add_files("*.cpp")
    add_deps("triple_base", "triple_refl", "triple_math")
    add_packages("stb")

includes("opengl")
