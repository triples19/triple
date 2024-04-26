add_rules("mode.debug", "mode.release")
set_languages("cxxlatest")

add_requires("catch2", "glfw", "stb")
add_packages("stb", "glfw")

includes("thirdparty")
includes("modules")
includes("samples/001-helloworld")

after_link(function(target)
    local ifc_dir = target:autogendir()
    for _, filepath in ipairs(os.files(modules_dir)) do 
        local outpath = path.join("build", "ifc", path.filename(filepath))
        os.trycp(filepath, outpath)
    end 
end)
