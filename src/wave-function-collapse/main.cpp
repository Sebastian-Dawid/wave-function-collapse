#include <cstdlib>
#include <clocale>
#include <wfc-lib.h>

#define UNICODE_TILE_IMPLEMENTATION
#include <unicode_tile.h>

template<>
std::vector<unicode_tile_t> superposition_t<unicode_tile_t>::all_possibilities = {
    unicode_tile_t(L' ', { 0, 0, 0, 0 }), unicode_tile_t(L'━', { 0, 1, 0, 1 }),
    unicode_tile_t(L'┃', { 1, 0, 1, 0 }), unicode_tile_t(L'╋', { 1, 1, 1, 1 }),
    unicode_tile_t(L'┻', { 1, 1, 0, 1 }), unicode_tile_t(L'┳', { 0, 1, 1, 1 }),
    unicode_tile_t(L'┫', { 1, 0, 1, 1 }), unicode_tile_t(L'┣', { 1, 1, 1, 0 }),
    unicode_tile_t(L'┏', { 0, 1, 1, 0 }), unicode_tile_t(L'┓', { 0, 0, 1, 1 }),
    unicode_tile_t(L'┗', { 1, 1, 0, 0 }), unicode_tile_t(L'┛', { 1, 0, 0, 1 })
};

int main(int argc, char** argv)
{
    superposition_t<unicode_tile_t> superposition;
    superposition.possibilities = superposition_t<unicode_tile_t>::all_possibilities;

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

    map_t<unicode_tile_t> map(w, h);
    size_t cap = map.width * map.depth;
    for (int i = 0; i < cap; ++i)
        map.map.push_back(superposition);

    setlocale(LC_ALL, "");
    bool success = false;
    size_t iter = 1;
    do
    {
        for (int i = 0; i < cap; ++i)
            map.map[i] = superposition;
        success = wave_function_collapse(map);
        for (std::size_t i = 0; i < map.depth; ++i)
        {
            for (std::size_t j = 0; j < map.width; ++j)
            {
                fwprintf(fd, L"%lc", map.at(j, i).possibilities[0].symbol);
            }
            fwprintf(fd, L"\n");
        }
    } while (!success);

    return EXIT_SUCCESS;
}
