function add_sample(name)
    target("samples-" .. name)
        set_kind("binary")
        add_files(name .. ".cpp")
        add_deps("triple")
        set_targetdir("$(buildir)/samples")
end

add_sample("helloworld")
add_sample("assets")
add_sample("sprite")
add_sample("input")
add_sample("sprite_move")
add_sample("debug_draw")
