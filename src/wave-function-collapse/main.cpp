#include <cstdlib>
#include <clocale>
#include <wfc-lib.h>
#include <default_tiles.h>
#include "renderer/renderer.h"

template<>
std::vector<wfc::unicode_tile_t> wfc::superposition_t<wfc::unicode_tile_t>::all_possibilities = {
    unicode_tile_t(L' ', { 0, 0, 0, 0 }),
    unicode_tile_t(L'━', { 0, 1, 0, 1 }),
    unicode_tile_t(L'┃', { 1, 0, 1, 0 }),
    unicode_tile_t(L'╋', { 1, 1, 1, 1 }),
    unicode_tile_t(L'┻', { 1, 1, 0, 1 }),
    unicode_tile_t(L'┳', { 0, 1, 1, 1 }),
    unicode_tile_t(L'┫', { 1, 0, 1, 1 }),
    unicode_tile_t(L'┣', { 1, 1, 1, 0 }),
    unicode_tile_t(L'┏', { 0, 1, 1, 0 }),
    unicode_tile_t(L'┓', { 0, 0, 1, 1 }),
    unicode_tile_t(L'┗', { 1, 1, 0, 0 }),
    unicode_tile_t(L'┛', { 1, 0, 0, 1 })
};

std::vector<glm::vec3> vs = {
    glm::vec3(.4f, 0.f, 0.f), //  0
    glm::vec3(.6f, 0.f, 0.f), //  1
    glm::vec3(0.f, .4f, 0.f), //  2
    glm::vec3(0.f, .6f, 0.f), //  3
    glm::vec3(.4f, .4f, 0.f), //  4
    glm::vec3(.4f, .6f, 0.f), //  5
    glm::vec3(.6f, .4f, 0.f), //  6
    glm::vec3(.6f, .6f, 0.f), //  7
    glm::vec3(1.f, .4f, 0.f), //  8
    glm::vec3(1.f, .6f, 0.f), //  9
    glm::vec3(.4f, 1.f, 0.f), // 10
    glm::vec3(.6f, 1.f, 0.f), // 11
};

std::vector<wfc::mesh_tile_t::mesh_t> meshes = {
    {},
    { vs, { 3,  9,  8,  8, 2, 3 } },
    { vs, { 0, 10, 11, 11, 1, 0 } },
    { 
        vs, { 
            0, 1, 11, 11, 10, 0, // vertical
            7, 9,  8,  8,  6, 7, // horizontal right
            3, 5,  4,  4,  2, 3  // horizontal left
        }
    },
    {
        vs, {
            3,  9,  8,  8, 2, 3, // horizontal
            5, 10, 11, 11, 7, 5  // vertical middle to top
        }
    },
    {
        vs, {
            3, 9, 8, 8, 2, 3, // horizontal
            0, 4, 6, 6, 1, 0, // vertical bottom to middle
        }
    },
    {
        vs, {
            0, 1, 11, 11, 10, 0, // vertical
            3, 5,  4,  4,  2, 3  // horizontal left
        }
    },
    {
        vs, {
            0, 1, 11, 11, 10, 0, // vertical
            7, 9,  8,  8,  6, 7, // horizontal right
        }
    },
    {
        vs, {
            0, 5, 7, 7, 1, 0, // vertical bottom to middle
            7, 9, 8, 8, 6, 7  // horizontal right
        }
    },
    {
        vs, {
            0, 5, 7, 7, 1, 0, // vertical bottom to middle
            3, 5, 4, 4, 2, 3  // horizontal left
        }
    },
    {
        vs, {
            4, 10, 11, 11, 6, 4, // vertival middle to top
            7, 9, 8, 8, 6, 7  // horizontal right
        }
    },
    {
        vs, {
            4, 10, 11, 11, 6, 4, // vertival middle to top
            3, 5, 4, 4, 2, 3  // horizontal left
        }
    }
};

template<>
std::vector<wfc::mesh_tile_t> wfc::superposition_t<wfc::mesh_tile_t>::all_possibilities = {
    mesh_tile_t(L' ', meshes[0],  { 0, 0, 0, 0 }),
    mesh_tile_t(L'━', meshes[1],  { 0, 1, 0, 1 }),
    mesh_tile_t(L'┃', meshes[2],  { 1, 0, 1, 0 }),
    mesh_tile_t(L'╋', meshes[3],  { 1, 1, 1, 1 }),
    mesh_tile_t(L'┻', meshes[4],  { 1, 1, 0, 1 }),
    mesh_tile_t(L'┳', meshes[5],  { 0, 1, 1, 1 }),
    mesh_tile_t(L'┫', meshes[6],  { 1, 0, 1, 1 }),
    mesh_tile_t(L'┣', meshes[7],  { 1, 1, 1, 0 }),
    mesh_tile_t(L'┏', meshes[8],  { 0, 1, 1, 0 }),
    mesh_tile_t(L'┓', meshes[9],  { 0, 0, 1, 1 }),
    mesh_tile_t(L'┗', meshes[10], { 1, 1, 0, 0 }),
    mesh_tile_t(L'┛', meshes[11], { 1, 0, 0, 1 })
};

int main(int argc, char** argv)
{
    std::size_t w = 12, h = 6;
    if (argc >= 3)
    {
        w = atol(argv[1]);
        h = atol(argv[2]);
    }
    
    FILE* fd = stdout;
    if (argc >= 4)
    {
        fd = fopen(argv[3], "w");
    }

    size_t cap = w * h;

    {
        wfc::superposition_t<wfc::unicode_tile_t> unicode_superposition;
        unicode_superposition.possibilities = wfc::superposition_t<wfc::unicode_tile_t>::all_possibilities;

        wfc::map_t<wfc::unicode_tile_t> unicode_map(w, h);
        for (int i = 0; i < cap; ++i)
            unicode_map.map.push_back(unicode_superposition);
        setlocale(LC_ALL, "");
        bool success = false;
        size_t iter = 0;
        do
        {
            for (int i = 0; i < cap; ++i)
                unicode_map.map[i] = unicode_superposition;
            success = wave_function_collapse(unicode_map);
            iter++;
        } while (!success);
        wprintf(L"iterations: %lu\n", iter);
        for (std::size_t i = 0; i < unicode_map.depth; ++i)
        {
            for (std::size_t j = 0; j < unicode_map.width; ++j)
            {
                fwprintf(fd, L"%lc", unicode_map.at(j, i).possibilities[0].symbol);
            }
            fwprintf(fd, L"\n");
        }
    }

    {
        wfc::superposition_t<wfc::mesh_tile_t> mesh_superposition;
        mesh_superposition.possibilities = wfc::superposition_t<wfc::mesh_tile_t>::all_possibilities;

        wfc::map_t<wfc::mesh_tile_t> mesh_map(w, h);
        for (int i = 0; i < cap; ++i)
            mesh_map.map.push_back(mesh_superposition);

        bool success = false;
        size_t iter = 0;
        do
        {
            for (int i = 0; i < cap; ++i)
                mesh_map.map[i] = mesh_superposition;
            success = wave_function_collapse(mesh_map);
            iter++;
        } while (!success);
        wprintf(L"iterations: %lu\n", iter);
        for (std::size_t i = 0; i < mesh_map.depth; ++i)
        {
            for (std::size_t j = 0; j < mesh_map.width; ++j)
            {
                fwprintf(fd, L"%lc", mesh_map.at(j, i).possibilities[0].symbol);
            }
            fwprintf(fd, L"\n");
        }

        std::vector<wfc::mesh_tile_t::mesh_t> meshes;
        for (wfc::superposition_t<wfc::mesh_tile_t> pos : mesh_map.map)
        {
            meshes.push_back(pos.possibilities[0].mesh);
        }

        renderer_t renderer(w, h);
        renderer.prep_mesh(meshes);
        renderer.render();
    }

    return EXIT_SUCCESS;
}
