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

    includedirs { "src/wave-function-collapse-lib/", "external/includes" }
    files { "src/wave-function-collapse/*.cpp", "src/wave-function-collapse/renderer/*.cpp", "external/includes/src/gl.c" }
    links { "wave-function-collapse-lib", "glfw", "GL" }

project "3d-wave-function-collapse"
    kind "ConsoleApp"
    language "C++"
    targetdir "build/bin/%{cfg.buildcfg}"

    libdirs { "external/vk-engine/build/lib/%{cfg.buildcfg}/" }
    includedirs { "src/wave-function-collapse-lib/", "external/vk-engine/include" }
    files { "src/3d-wave-function-collapse/main.cpp" }
    links { "fmt", "glfw", "vulkan", "vk-engine", "imgui", "fastgltf" }
