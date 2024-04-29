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
