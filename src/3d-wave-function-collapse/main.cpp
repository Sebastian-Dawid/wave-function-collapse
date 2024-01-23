#include <vk-engine.h>
#include <wfc-lib.h>

#include <filesystem>
#include <fmt/core.h>

#include <glm/gtx/transform.hpp>

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

    std::size_t count = 0;
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (!engine.load_model(entry.path().string(), std::format("{}", count)))
        {
            return EXIT_FAILURE;
        }
        engine.loaded_scenes[std::format("{}", count)]->transform = glm::translate(glm::vec3(2 * count, 0, 0));
        count++;
    }

    engine.run();

    return EXIT_SUCCESS;
}
