#include <vk-engine.h>
#include <wfc-lib.h>

#include <filesystem>
#include <fmt/core.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include <imgui.h>

#include <format>
#include <error_fmt.h>

struct gltf_tile_t : wfc::tile_i
{
    // NORTH, EAST, SOUTH, WEST, UP, DOWN
    std::array<bool, 6> faces = {};
    float weight = 1.f;
    std::optional<std::uint32_t> tile_index = std::nullopt;
    glm::mat4 rotation = glm::mat4(1.f);

    bool fits_dir(const tile_i& tile, const wfc::dir_t dir) const override
    {
        gltf_tile_t& t = (gltf_tile_t&) tile;
        switch (dir) {

            case wfc::dir_t::NORTH:
                return this->faces[2] == t.faces[0];
            case wfc::dir_t::EAST:
                return this->faces[1] == t.faces[3];
            case wfc::dir_t::SOUTH:
                return this->faces[0] == t.faces[2];
            case wfc::dir_t::WEST:
                return this->faces[3] == t.faces[1];
            case wfc::dir_t::UP:
                return this->faces[4] == t.faces[5];
            case wfc::dir_t::DOWN:
                return this->faces[5] == t.faces[4];
            default:
                return false;
        }
    };

    float get_weight() const override
    {
        return this->weight;
    };

    bool operator==(const tile_i& other) const override
    {
        return false;
    };
 
    bool operator==(const gltf_tile_t& other) const
    {
        return this->rotation == other.rotation && this->tile_index == other.tile_index;
    };

    gltf_tile_t(std::optional<std::uint32_t> index, glm::mat4 rotation, std::array<bool, 6> faces, float weight = 1.f)
    {
        this->tile_index = index;
        this->rotation = rotation;
        this->faces = faces;
        this->weight = weight;
    }
};

std::vector<std::string> tileset = {
    "./assets/tileset/all.glb",
    "./assets/tileset/all_but_one.glb",
    "./assets/tileset/all_but_two.glb",
    "./assets/tileset/plus_xz.glb",
    "./assets/tileset/3_corner.glb",
    "./assets/tileset/t_shape.glb",
    "./assets/tileset/2_corner.glb",
    "./assets/tileset/straight.glb",
    "./assets/tileset/straight_half.glb"
};

template<>
std::vector<gltf_tile_t> wfc::superposition_t<gltf_tile_t>::all_possibilities = {
    gltf_tile_t(std::nullopt, glm::mat4(1.f), { 0 }),
    
    // one option
    gltf_tile_t(0, glm::mat4(1.f), { true, true, true, true, true, true }),

    //// 6 options
    gltf_tile_t(1, glm::mat4(1.f), { true, true, true, true, false, true }),
    gltf_tile_t(1, glm::rotate(glm::radians( 90.f), glm::vec3(1, 0, 0)), { true, true, false, true, true, true }),
    gltf_tile_t(1, glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { true, true, true, true, true, false }),
    gltf_tile_t(1, glm::rotate(glm::radians(270.f), glm::vec3(1, 0, 0)), { false, true, true, true, true, true }),
    gltf_tile_t(1, glm::rotate(glm::radians( 90.f), glm::vec3(0, 0, 1)), { true, true, true, false, true, true }),
    gltf_tile_t(1, glm::rotate(glm::radians(270.f), glm::vec3(0, 0, 1)), { true, false, true, true, true, true }),

    // 10 options
    gltf_tile_t(2, glm::mat4(1.f), { true, false, true, true, false, true }),
    gltf_tile_t(2, glm::rotate(glm::radians( 90.f), glm::vec3(1, 0, 0)), { true, false, false, true, true, true }),
    gltf_tile_t(2, glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { true, false, true, true, true, false }),
    gltf_tile_t(2, glm::rotate(glm::radians(270.f), glm::vec3(1, 0, 0)), { false, false, true, true, true, true }),
    gltf_tile_t(2, glm::rotate(glm::radians(180.f), glm::vec3(0, 0, 1)), { true, true, true, false, true, false }),
    gltf_tile_t(2, glm::rotate(glm::radians( 90.f), glm::vec3(0, 0, 1)), { true, true, true, false, false, true }),
    gltf_tile_t(2, glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)), { false, true, true, true, false, true }),
    gltf_tile_t(2, glm::rotate(glm::radians(270.f), glm::vec3(0, 1, 0)), { true, true, false, true, false, true }),
    gltf_tile_t(2, glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { false, true, true, true, true, false }),
    gltf_tile_t(2, glm::rotate(glm::radians(270.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { true, true, false, true, true, false }),

    // 3 options
    gltf_tile_t(3, glm::mat4(1.f), { false, true, false, true, true, true }),
    gltf_tile_t(3, glm::rotate(glm::radians( 90.f), glm::vec3(1, 0, 0)), { true, true, true, true, false, false }),
    gltf_tile_t(3, glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)), { true, false, true, false, true, true }),

    // 8 options
    gltf_tile_t(4, glm::mat4(1.f), { true, false, false, true, false, true }),
    gltf_tile_t(4, glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)), { false, false, true, true, false, true }),
    gltf_tile_t(4, glm::rotate(glm::radians(180.f), glm::vec3(0, 1, 0)), { false, true, true, false, false, true }),
    gltf_tile_t(4, glm::rotate(glm::radians(270.f), glm::vec3(0, 1, 0)), { true, true, false, false, false, true }),
    gltf_tile_t(4, glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { false, false, true, true, true, false }),
    gltf_tile_t(4, glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { false, true, true, false, true, false }),
    gltf_tile_t(4, glm::rotate(glm::radians(180.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { true, true, false, false, true, false }),
    gltf_tile_t(4, glm::rotate(glm::radians(270.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { true, false, false, true, true, false }),

    // 12 options
    gltf_tile_t(5, glm::mat4(1.f), { true, false, true, false, false, true }),
    gltf_tile_t(5, glm::rotate(glm::radians( 90.f), glm::vec3(0, 0, 1)), { true, true, true, false, false, false }),
    gltf_tile_t(5, glm::rotate(glm::radians(180.f), glm::vec3(0, 0, 1)), { true, false, true, false, true, false }),
    gltf_tile_t(5, glm::rotate(glm::radians(270.f), glm::vec3(0, 0, 1)), { true, false, true, true, false, false }),
    gltf_tile_t(5, glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)), { false, true, false, true, false, true }),
    gltf_tile_t(5, glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians( 90.f), glm::vec3(0, 0, 1)), { true, true, false, true, false, false }),
    gltf_tile_t(5, glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(180.f), glm::vec3(0, 0, 1)), { false, true, false, true, true, false }),
    gltf_tile_t(5, glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(270.f), glm::vec3(0, 0, 1)), { false, true, true, true, false, false }),
    gltf_tile_t(5, glm::rotate(glm::radians(90.f), glm::vec3(1, 0, 0)), { true, false, false, false, true, true }),
    gltf_tile_t(5, glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(90.f), glm::vec3(1, 0, 0)), { false, false, false, true, true, true }),
    gltf_tile_t(5, glm::rotate(glm::radians(180.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(90.f), glm::vec3(1, 0, 0)), { false, false, true, false, true, true }),
    gltf_tile_t(5, glm::rotate(glm::radians(270.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(90.f), glm::vec3(1, 0, 0)), { false, true, false, false, true, true }),
    
    // 10 options
    gltf_tile_t(6, glm::mat4(1.f), { true, false, false, false, false, true }),
    gltf_tile_t(6, glm::rotate(glm::radians( 90.f), glm::vec3(1, 0, 0)), { true, false, false, false, true, false }),
    gltf_tile_t(6, glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { false, false, true, false, true, false }),
    gltf_tile_t(6, glm::rotate(glm::radians(270.f), glm::vec3(1, 0, 0)), { false, false, true, false, false, true }),
    gltf_tile_t(6, glm::rotate(glm::radians(270.f), glm::vec3(0, 0, 1)), { true, false, false, true, false, false }),
    gltf_tile_t(6, glm::rotate(glm::radians( 90.f), glm::vec3(0, 0, 1)), { true, true, false, false, false, false }),
    gltf_tile_t(6, glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)), { false, false, false, true, false, true }),
    gltf_tile_t(6, glm::rotate(glm::radians(270.f), glm::vec3(0, 1, 0)), { false, true, false, false, false, true }),
    gltf_tile_t(6, glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { false, true, false, false, true, false }),
    gltf_tile_t(6, glm::rotate(glm::radians(270.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { false, false, false, true, true, false }),

    // 3 options
    gltf_tile_t(7, glm::mat4(1.f), { true, false, true, false, false, false }),
    gltf_tile_t(7, glm::rotate(glm::radians( 90.f), glm::vec3(1, 0, 0)), { false, false, false, false, true, true }),
    gltf_tile_t(7, glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)), { false, true, false, true, false, false }),
    
    // 6 options
    gltf_tile_t(8, glm::mat4(1.f), { true, false, false, false, false, false }),
    gltf_tile_t(8, glm::rotate(glm::radians( 90.f), glm::vec3(1, 0, 0)), { false, false, false, false, true, false }),
    gltf_tile_t(8, glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { false, false, true, false, false, false }),
    gltf_tile_t(8, glm::rotate(glm::radians(270.f), glm::vec3(1, 0, 0)), { false, false, false, false, false, true }),
    gltf_tile_t(8, glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)), { false, false, false, true, false, false }),
    gltf_tile_t(8, glm::rotate(glm::radians(270.f), glm::vec3(0, 1, 0)), { false, true, false, false, false, false }),
};

float scale = .3f;

int main()
{
    std::string path = std::filesystem::current_path().string() + "/assets/tileset";
    fmt::print("All asset files:\n");
    for (const auto& entry : std::filesystem::directory_iterator(path))
        fmt::print("\t{}\n", entry.path().string());

    engine_t engine(1024, 1024, "wfc", true, true);

    camera_t cam{ .position = glm::vec3(0.f) };
    glfwSetWindowUserPointer(engine.window.win, &cam);

    if (!engine.init_vulkan("wfc"))
    {
        return EXIT_FAILURE;
    }

    std::string pwd =  std::filesystem::current_path().string();
    if (!engine.metal_rough_material.build_pipelines(&engine, pwd + "/external/vk-engine/tests/build/shaders/mesh.vert.spv", pwd + "/external/vk-engine/tests/build/shaders/mesh.frag.spv",
                sizeof(gpu_draw_push_constants_t), { { 0, vk::DescriptorType::eUniformBuffer }, { 1, vk::DescriptorType::eCombinedImageSampler }, { 2, vk::DescriptorType::eCombinedImageSampler } },
                { engine.scene_data.layout }, { vk::VertexInputBindingDescription(0, sizeof(glm::mat4), vk::VertexInputRate::eInstance) },
                { vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32A32Sfloat, 0),
                vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32A32Sfloat, sizeof(float) * 4),
                vk::VertexInputAttributeDescription(2, 0, vk::Format::eR32G32B32A32Sfloat, sizeof(float) * 8),
                vk::VertexInputAttributeDescription(3, 0, vk::Format::eR32G32B32A32Sfloat, sizeof(float) * 12)}))
    {
        return EXIT_FAILURE;
    }

    std::vector<std::shared_ptr<loaded_gltf_t>> tiles;
    for (std::string asset : tileset)
    {
        tiles.push_back(load_gltf(&engine, asset).value());
    }

    fmt::print("[ {} ]\tWFC started!\n", INFO_FMT("INFO"));
    wfc::superposition_t<gltf_tile_t> gltf_superposition;
    gltf_superposition.possibilities = gltf_superposition.all_possibilities;

    wfc::map_t<gltf_tile_t> gltf_map(6, 6, 6);
    std::size_t capacity = gltf_map.width * gltf_map.height * gltf_map.depth;
    for (int i = 0; i < capacity; ++i)
        gltf_map.map.push_back(gltf_superposition);
    bool success = false;
    do {
        for (int i = 0; i < capacity; ++i)
            gltf_map.map[i] = gltf_superposition;
        success = wfc::wave_function_collapse(gltf_map);
    } while (!success);
    fmt::print("[ {} ]\tWFC done!\n", INFO_FMT("INFO"));

    for (std::size_t y = 0; y < gltf_map.height; ++y)
    {
        for (std::size_t z = 0; z < gltf_map.depth; ++z)
        {
            for (std::size_t x = 0; x < gltf_map.width; ++x)
            {
                gltf_tile_t t = gltf_map.at(x, z, y).value()->possibilities[0];
                if (!t.tile_index.has_value()) continue;
                engine.loaded_scenes[std::format("{}", t.tile_index.value())] = tiles[t.tile_index.value()];
                engine.loaded_scenes[std::format("{}", t.tile_index.value())]->transform.push_back(glm::translate(glm::vec3(x * 2, y * 2, z * 2) * scale) * t.rotation * glm::scale(glm::vec3(scale)));
            }
        }
    }

    engine.define_imgui_windows = [&]()
    {
        if (ImGui::Begin("Resolution"))
        {
            ImGui::SliderFloat("Render Scale",&engine.render_scale, 0.01f, 1.f);
            ImGui::Text("Render Resolution: (%d, %d)", engine.draw_extent.width, engine.draw_extent.height);
            ImGui::Text("Window Resolution: (%d, %d)", engine.swapchain.extent.width, engine.swapchain.extent.height);
            ImGui::Text("Buffer Resolution: (%d, %d)", engine.draw_image.extent.width, engine.draw_image.extent.height);
            ImGui::End();
        }

        if (ImGui::Begin("Light"))
        {
            ImGui::ColorEdit4("ambient color", (float*) &engine.scene_data.gpu_data.ambient_color);
            ImGui::ColorEdit4("light color", (float*) &engine.scene_data.gpu_data.sunlight_color);
            ImGui::InputFloat4("light dir", (float*) &engine.scene_data.gpu_data.sunlight_dir);
            ImGui::End();
        }
    };

    engine.input_handler = [&]()
    {
        if (glfwGetKey(engine.window.win, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(engine.window.win, GLFW_TRUE);
        cam.process_glfw_event(engine.window.win);
    };

    engine.update = [&]()
    {
        engine.background_effects[0].data.data3.x = engine.window.width;
        engine.background_effects[0].data.data3.y = engine.window.height;
        engine.background_effects[0].data.data3.z = engine.render_scale;
        cam.update();

        engine.scene_data.gpu_data.view = cam.get_view_matrix();
        engine.scene_data.gpu_data.proj = glm::perspective(glm::radians(70.f), (float)engine.window.width / (float)engine.window.height, .1f, 10000.f);
        engine.scene_data.gpu_data.proj[1][1] *= -1;
        engine.scene_data.gpu_data.viewproj = engine.scene_data.gpu_data.proj * engine.scene_data.gpu_data.view ;
    };

    engine.run();

    return EXIT_SUCCESS;
}
