add_rules("mode.debug", "mode.release")
set_languages("c++20")

add_requires("catch2", "glfw", "stb", "box2d")
add_requires("imgui", {configs = {glfw = true, opengl3 = true}})
add_packages("glfw", "stb", "box2d")

set_warnings("all")

includes("thirdparty")
includes("modules")
includes("samples")


after_link(function(target)
    local ifc_dir = target:autogendir()
    for _, filepath in ipairs(os.files(ifc_dir .. "/**/*.ifc")) do
        local outpath = path.join("build", "ifc", path.filename(filepath))
        os.trycp(filepath, outpath)
    end 
end)
