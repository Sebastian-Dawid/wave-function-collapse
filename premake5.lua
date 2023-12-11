workspace "wave-function-collapse"
    toolset "clang"
    cppdialect "c++20"
    configurations {"Debug", "Release"}
    location "build"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "wave-function-collapse-lib"
    kind "StaticLib"
    language "C++"
    targetdir "build/lib/%{cfg.buildcfg}"

    files { "src/wave-function-collapse-lib/**.cpp", "src/wave-function-collapse-lib/**.c" }

project "wave-function-collapse"
    kind "ConsoleApp"
    language "C++"
    targetdir "build/bin/%{cfg.buildcfg}"

    files { "src/wave-function-collapse/**.cpp", "src/wave-function-collapse/**.c" }
    links { "wave-function-collapse-lib" }
