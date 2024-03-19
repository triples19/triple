add_rules("mode.debug", "mode.release")
set_languages("cxxlatest")

triple_dir = os.scriptdir()

add_requires("catch2 3.x")

includes("modules")
includes("samples/001-helloworld")
