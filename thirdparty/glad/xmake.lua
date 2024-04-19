target("glad")
    set_kind("static")
    add_files("src/glad.c")
    add_includedirs("include", {public = true})