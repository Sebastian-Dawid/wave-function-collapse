#include <cstdlib>
#include <clocale>
#include <wfc-lib.h>

/*
 * Tiles: 
 * ━ horizontal.
 * ┃ vertical.
 * ╋ cross.
 * ┻ up, horizontal.
 * ┳ down, horizontal.
 * ┏ down, right corner.
 * ┓ down, left corner.
 * ┗ up, right corner.
 * ┛ up, left corner.
 */
struct tile_t : tile_i
{
    char16_t symbol;
    std::array<bool, 4> edges = {0};

    bool fits_dir(const tile_i& tile, const dir_t dir) const override;

    tile_t(char16_t symbol, std::array<bool, 4> edges);
};

bool tile_t::fits_dir(const tile_i& tile, const dir_t dir) const
{
    tile_t& t = (tile_t&)tile;
    switch (dir) {
        case dir_t::NORTH:
            return this->edges[2] == t.edges[0];
        case dir_t::EAST:
            return this->edges[1] == t.edges[3];
        case dir_t::SOUTH:
            return this->edges[0] == t.edges[2];
        case dir_t::WEST:
            return this->edges[3] == t.edges[1];
        default:
            return false;
    }
}

tile_t::tile_t(char16_t symbol, std::array<bool, 4> edges)
{
    this->symbol = symbol;
    this->edges = edges;
}

template<>
std::vector<tile_t> superposition_t<tile_t>::all_possibilities = {
    tile_t(L' ', { 0, 0, 0, 0 }), tile_t(L'━', { 0, 1, 0, 1 }),
    tile_t(L'┃', { 1, 0, 1, 0 }), tile_t(L'╋', { 1, 1, 1, 1 }),
    tile_t(L'┻', { 1, 1, 0, 1 }), tile_t(L'┳', { 0, 1, 1, 1 }),
    tile_t(L'┫', { 1, 0, 1, 1 }), tile_t(L'┣', { 1, 1, 1, 0 }),
    tile_t(L'┏', { 0, 1, 1, 0 }), tile_t(L'┓', { 0, 0, 1, 1 }),
    tile_t(L'┗', { 1, 1, 0, 0 }), tile_t(L'┛', { 1, 0, 0, 1 })
};

int main(int argc, char** argv)
{
    superposition_t<tile_t> superposition;
    superposition.possibilities = superposition_t<tile_t>::all_possibilities;

    std::size_t w = 12, h = 6;

    if (argc == 3)
    {
        w = atol(argv[1]);
        h = atol(argv[2]);
    }

    map_t<tile_t> map(w, h);
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
        wprintf(L"iteration: %lu\n", iter++);
        for (std::size_t i = 0; i < map.depth; ++i)
        {
            for (std::size_t j = 0; j < map.width; ++j)
            {
                wprintf(L"%lc", map.at(j, i).possibilities[0].symbol);
            }
            wprintf(L"\n");
        }
    } while (!success);

    return EXIT_SUCCESS;
}
