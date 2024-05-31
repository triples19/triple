add_rules("mode.debug", "mode.release", "mode.releasedbg")
set_languages("c++23")

add_requires("catch2", "stb", "box2d")
add_requires("glfw", {configs = {shared = false}})
add_requires("imgui", {configs = {glfw = true, opengl3 = true}})
add_packages("glfw", "stb", "box2d")

set_warnings("all")

local project_dir = os.scriptdir():gsub("\\", "/")
add_cxxflags("-DTRIPLE_ASSETS_PATH=\"" .. project_dir .. "/assets\"")

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
