#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <default_tiles.h>

struct renderer_t
{
    std::size_t w, h, win_w, win_h;
    std::uint32_t VAO, VBO, EBO, shader;
    wfc::mesh_tile_t::mesh_t mesh;
    std::vector<float> vertices;
    std::vector<std::uint32_t> indices;
    GLFWwindow* window = NULL;
    bool init = false;
    
    void prep_mesh(std::vector<wfc::mesh_tile_t::mesh_t> meshes);
    bool compile_shader();
    std::int32_t render();
    renderer_t(std::size_t w, std::size_t h);
};
