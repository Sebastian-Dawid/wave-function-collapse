#include <vk-engine.h>
#include <wfc-lib.h>

#include <filesystem>
#include <fmt/core.h>

#include <glm/gtx/transform.hpp>

#include <format>

struct gltf_tile_t : wfc::tile_i
{
    // NORTH, EAST, SOUTH, WEST, UP, DOWN
    std::array<bool, 6> faces = {};
    float weight = 1.f;
    std::string gltf_path = "<none>";
    glm::mat4 rotation = glm::mat4(1.f);

    bool fits_dir(const tile_i& tile, const wfc::dir_t dir) const override
    {
        gltf_tile_t& t = (gltf_tile_t&) tile;
        switch (dir) {

            case wfc::dir_t::NORTH:
                return this->faces[0] == t.faces[2];
            case wfc::dir_t::EAST:
                return this->faces[1] == t.faces[3];
            case wfc::dir_t::SOUTH:
                return this->faces[2] == t.faces[0];
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

    gltf_tile_t(std::string path, glm::mat4 rotation, std::array<bool, 6> faces, float weight = 1.f)
    {
        this->gltf_path = path;
        this->rotation = rotation;
        this->faces = faces;
        this->weight = weight;
    }
};

template<>
std::vector<gltf_tile_t> wfc::superposition_t<gltf_tile_t>::all_possibilities = {
    gltf_tile_t("", glm::mat4(1.f), { 0 }), // empty tile
    gltf_tile_t("/assets/tileset/all.glb", glm::mat4(1.f), { true, true, true, true, true, true }), // all connections
    gltf_tile_t("/assets/tileset/all_but_one.glb", glm::mat4(1.f), { true, true, true, true, false, true }) // all connections but up
};

int main()
{
    std::string path = std::filesystem::current_path().string() + "/assets/tileset";
    fmt::print("All asset files:\n");
    for (const auto& entry : std::filesystem::directory_iterator(path))
        fmt::print("\t{}\n", entry.path().string());

    engine_t engine(1024, 1024, "wfc", false, true);
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

    /*
    std::size_t count = 0;
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (!engine.load_model(entry.path().string(), std::format("{}", count)))
        {
            return EXIT_FAILURE;
        }
        engine.loaded_scenes[std::format("{}", count)]->transform = glm::translate(glm::vec3(2 * count, 0, 0));
        count++;
    }*/
    wfc::superposition_t<gltf_tile_t> gltf_superposition;
    gltf_superposition.possibilities = gltf_superposition.all_possibilities;

    wfc::map_t<gltf_tile_t> gltf_map(4, 4);
    for (int i = 0; i < 16; ++i)
        gltf_map.map.push_back(gltf_superposition);
    bool success = false;
    do {
        for (int i = 0; i < 16; ++i)
            gltf_map.map[i] = gltf_superposition;
        success = wfc::wave_function_collapse(gltf_map);
    } while (!success);

    engine.run();

    return EXIT_SUCCESS;
}
