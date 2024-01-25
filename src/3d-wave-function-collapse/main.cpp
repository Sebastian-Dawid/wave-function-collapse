#include <vk-engine.h>
#include <wfc-lib.h>

#include <filesystem>
#include <fmt/core.h>

#include <glm/gtx/transform.hpp>

#include <format>
#include <error_fmt.h>

struct gltf_tile_t : wfc::tile_i
{
    // NORTH, EAST, SOUTH, WEST, UP, DOWN
    std::array<bool, 6> faces = {};
    float weight = 1.f;
    std::optional<std::string> gltf_path = std::nullopt;
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
        return this->rotation == other.rotation && this->gltf_path == other.gltf_path;
    };

    gltf_tile_t(std::optional<std::string> path, glm::mat4 rotation, std::array<bool, 6> faces, float weight = 1.f)
    {
        this->gltf_path = path;
        this->rotation = rotation;
        this->faces = faces;
        this->weight = weight;
    }
};

template<>
std::vector<gltf_tile_t> wfc::superposition_t<gltf_tile_t>::all_possibilities = {
    gltf_tile_t(std::nullopt, glm::mat4(1.f), { 0 }),
    
    // one option
    gltf_tile_t("./assets/tileset/all.glb", glm::mat4(1.f), { true, true, true, true, true, true }),

    //// 6 options
    gltf_tile_t("./assets/tileset/all_but_one.glb", glm::mat4(1.f), { true, true, true, true, false, true }),
    gltf_tile_t("./assets/tileset/all_but_one.glb", glm::rotate(glm::radians( 90.f), glm::vec3(1, 0, 0)), { true, true, false, true, true, true }),
    gltf_tile_t("./assets/tileset/all_but_one.glb", glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { true, true, true, true, true, false }),
    gltf_tile_t("./assets/tileset/all_but_one.glb", glm::rotate(glm::radians(270.f), glm::vec3(1, 0, 0)), { false, true, true, true, true, true }),
    gltf_tile_t("./assets/tileset/all_but_one.glb", glm::rotate(glm::radians( 90.f), glm::vec3(0, 0, 1)), { true, true, true, false, true, true }),
    gltf_tile_t("./assets/tileset/all_but_one.glb", glm::rotate(glm::radians(270.f), glm::vec3(0, 0, 1)), { true, false, true, true, true, true }),

    // 10 options
    gltf_tile_t("./assets/tileset/all_but_two.glb", glm::mat4(1.f), { true, false, true, true, false, true }),
    gltf_tile_t("./assets/tileset/all_but_two.glb", glm::rotate(glm::radians( 90.f), glm::vec3(1, 0, 0)), { true, false, false, true, true, true }),
    gltf_tile_t("./assets/tileset/all_but_two.glb", glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { true, false, true, true, true, false }),
    gltf_tile_t("./assets/tileset/all_but_two.glb", glm::rotate(glm::radians(270.f), glm::vec3(1, 0, 0)), { false, false, true, true, true, true }),
    gltf_tile_t("./assets/tileset/all_but_two.glb", glm::rotate(glm::radians(180.f), glm::vec3(0, 0, 1)), { true, true, true, false, true, false }),
    gltf_tile_t("./assets/tileset/all_but_two.glb", glm::rotate(glm::radians( 90.f), glm::vec3(0, 0, 1)), { true, true, true, false, false, true }),
    gltf_tile_t("./assets/tileset/all_but_two.glb", glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)), { false, true, true, true, false, true }),
    gltf_tile_t("./assets/tileset/all_but_two.glb", glm::rotate(glm::radians(270.f), glm::vec3(0, 1, 0)), { true, true, false, true, false, true }),
    gltf_tile_t("./assets/tileset/all_but_two.glb", glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { false, true, true, true, true, false }),
    gltf_tile_t("./assets/tileset/all_but_two.glb", glm::rotate(glm::radians(270.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { true, true, false, true, true, false }),

    // 3 options
    gltf_tile_t("./assets/tileset/plus_xz.glb", glm::mat4(1.f), { false, true, false, true, true, true }),
    gltf_tile_t("./assets/tileset/plus_xz.glb", glm::rotate(glm::radians( 90.f), glm::vec3(1, 0, 0)), { true, true, true, true, false, false }),
    gltf_tile_t("./assets/tileset/plus_xz.glb", glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)), { true, false, true, false, true, true }),

    // 8 options
    gltf_tile_t("./assets/tileset/3_corner.glb", glm::mat4(1.f), { true, false, false, true, false, true }),
    gltf_tile_t("./assets/tileset/3_corner.glb", glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)), { false, false, true, true, false, true }),
    gltf_tile_t("./assets/tileset/3_corner.glb", glm::rotate(glm::radians(180.f), glm::vec3(0, 1, 0)), { false, true, true, false, false, true }),
    gltf_tile_t("./assets/tileset/3_corner.glb", glm::rotate(glm::radians(270.f), glm::vec3(0, 1, 0)), { true, true, false, false, false, true }),
    gltf_tile_t("./assets/tileset/3_corner.glb", glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { false, false, true, true, true, false }),
    gltf_tile_t("./assets/tileset/3_corner.glb", glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { false, true, true, false, true, false }),
    gltf_tile_t("./assets/tileset/3_corner.glb", glm::rotate(glm::radians(180.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { true, true, false, false, true, false }),
    gltf_tile_t("./assets/tileset/3_corner.glb", glm::rotate(glm::radians(270.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { true, false, false, true, true, false }),

    // 12 options
    gltf_tile_t("./assets/tileset/t_shape.glb", glm::mat4(1.f), { true, false, true, false, false, true }),
    gltf_tile_t("./assets/tileset/t_shape.glb", glm::rotate(glm::radians( 90.f), glm::vec3(0, 0, 1)), { true, true, true, false, false, false }),
    gltf_tile_t("./assets/tileset/t_shape.glb", glm::rotate(glm::radians(180.f), glm::vec3(0, 0, 1)), { true, false, true, false, true, false }),
    gltf_tile_t("./assets/tileset/t_shape.glb", glm::rotate(glm::radians(270.f), glm::vec3(0, 0, 1)), { true, false, true, true, false, false }),
    gltf_tile_t("./assets/tileset/t_shape.glb", glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)), { false, true, false, true, false, true }),
    gltf_tile_t("./assets/tileset/t_shape.glb", glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians( 90.f), glm::vec3(0, 0, 1)), { true, true, false, true, false, false }),
    gltf_tile_t("./assets/tileset/t_shape.glb", glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(180.f), glm::vec3(0, 0, 1)), { false, true, false, true, true, false }),
    gltf_tile_t("./assets/tileset/t_shape.glb", glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(270.f), glm::vec3(0, 0, 1)), { false, true, true, true, false, false }),
    gltf_tile_t("./assets/tileset/t_shape.glb", glm::rotate(glm::radians(90.f), glm::vec3(1, 0, 0)), { true, false, false, false, true, true }),
    gltf_tile_t("./assets/tileset/t_shape.glb", glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(90.f), glm::vec3(1, 0, 0)), { false, false, false, true, true, true }),
    gltf_tile_t("./assets/tileset/t_shape.glb", glm::rotate(glm::radians(180.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(90.f), glm::vec3(1, 0, 0)), { false, false, true, false, true, true }),
    gltf_tile_t("./assets/tileset/t_shape.glb", glm::rotate(glm::radians(270.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(90.f), glm::vec3(1, 0, 0)), { false, true, false, false, true, true }),
    
    // 10 options
    gltf_tile_t("./assets/tileset/2_corner.glb", glm::mat4(1.f), { true, false, false, false, false, true }),
    gltf_tile_t("./assets/tileset/2_corner.glb", glm::rotate(glm::radians( 90.f), glm::vec3(1, 0, 0)), { true, false, false, false, true, false }),
    gltf_tile_t("./assets/tileset/2_corner.glb", glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { false, false, true, false, true, false }),
    gltf_tile_t("./assets/tileset/2_corner.glb", glm::rotate(glm::radians(270.f), glm::vec3(1, 0, 0)), { false, false, true, false, false, true }),
    gltf_tile_t("./assets/tileset/2_corner.glb", glm::rotate(glm::radians(270.f), glm::vec3(0, 0, 1)), { true, false, false, true, false, false }),
    gltf_tile_t("./assets/tileset/2_corner.glb", glm::rotate(glm::radians( 90.f), glm::vec3(0, 0, 1)), { true, true, false, false, false, false }),
    gltf_tile_t("./assets/tileset/2_corner.glb", glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)), { false, false, false, true, false, true }),
    gltf_tile_t("./assets/tileset/2_corner.glb", glm::rotate(glm::radians(270.f), glm::vec3(0, 1, 0)), { false, true, false, false, false, true }),
    gltf_tile_t("./assets/tileset/2_corner.glb", glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { false, true, false, false, true, false }),
    gltf_tile_t("./assets/tileset/2_corner.glb", glm::rotate(glm::radians(270.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { false, false, false, true, true, false }),

    // 3 options
    gltf_tile_t("./assets/tileset/straight.glb", glm::mat4(1.f), { true, false, true, false, false, false }),
    gltf_tile_t("./assets/tileset/straight.glb", glm::rotate(glm::radians( 90.f), glm::vec3(1, 0, 0)), { false, false, false, false, true, true }),
    gltf_tile_t("./assets/tileset/straight.glb", glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)), { false, true, false, true, false, false }),
    
    // 6 options
    gltf_tile_t("./assets/tileset/straight_half.glb", glm::mat4(1.f), { true, false, false, false, false, false }),
    gltf_tile_t("./assets/tileset/straight_half.glb", glm::rotate(glm::radians( 90.f), glm::vec3(1, 0, 0)), { false, false, false, false, true, false }),
    gltf_tile_t("./assets/tileset/straight_half.glb", glm::rotate(glm::radians(180.f), glm::vec3(1, 0, 0)), { false, false, true, false, false, false }),
    gltf_tile_t("./assets/tileset/straight_half.glb", glm::rotate(glm::radians(270.f), glm::vec3(1, 0, 0)), { false, false, false, false, false, true }),
    gltf_tile_t("./assets/tileset/straight_half.glb", glm::rotate(glm::radians( 90.f), glm::vec3(0, 1, 0)), { false, false, false, true, false, false }),
    gltf_tile_t("./assets/tileset/straight_half.glb", glm::rotate(glm::radians(270.f), glm::vec3(0, 1, 0)), { false, true, false, false, false, false }),
};

float scale = .3f;

int main()
{
    std::string path = std::filesystem::current_path().string() + "/assets/tileset";
    fmt::print("All asset files:\n");
    for (const auto& entry : std::filesystem::directory_iterator(path))
        fmt::print("\t{}\n", entry.path().string());

    engine_t engine(1024, 1024, "wfc", true, true);
    if (!engine.init_vulkan("wfc"))
    {
        return EXIT_FAILURE;
    }

    if (!engine.metal_rough_material.build_pipelines(&engine, "../vk-engine/tests/build/shaders/mesh.vert.spv", "../vk-engine/tests/build/shaders/mesh.frag.spv",
                sizeof(gpu_draw_push_constants_t), { { 0, vk::DescriptorType::eUniformBuffer }, { 1, vk::DescriptorType::eCombinedImageSampler }, { 2, vk::DescriptorType::eCombinedImageSampler } },
                { engine.scene_data.layout }))
    {
        return EXIT_FAILURE;
    }

    fmt::print("[ {} ]\tWFC started!\n", INFO_FMT("INFO"));
    wfc::superposition_t<gltf_tile_t> gltf_superposition;
    gltf_superposition.possibilities = gltf_superposition.all_possibilities;

    wfc::map_t<gltf_tile_t> gltf_map(2, 2, 2);
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
                if (!t.gltf_path.has_value()) continue;
                if (!engine.load_model(t.gltf_path.value(), std::format("({}, {}, {})", x, y, z))) return EXIT_FAILURE;
                engine.loaded_scenes[std::format("({}, {}, {})", x, y, z)]->transform = glm::translate(glm::vec3(x * 2, y * 2, z * 2) * scale) * t.rotation * glm::scale(glm::vec3(scale));
            }
        }
    }

    engine.run();

    return EXIT_SUCCESS;
}
