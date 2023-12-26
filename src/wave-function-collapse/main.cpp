#include <cstdlib>
#include <clocale>
#include <wfc-lib.h>

/* Forward declaration of `tile_t`.
 * 
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
    dir_t out_dirs;

    bool fits_dir(const tile_i& tile, const dir_t dir) const override;

    tile_t(char16_t symbol, dir_t out_dirs);
};

bool tile_t::fits_dir(const tile_i& tile, const dir_t dir) const
{
    tile_t& t = (tile_t&)tile;
    switch (dir) {
        case dir_t::NORTH:
            if ((out_dirs & dir_t::NORTH) == dir_t::NORTH && (t.out_dirs & dir_t::SOUTH) == dir_t::SOUTH
                    || !((out_dirs & dir_t::NORTH) == dir_t::NORTH) && !((t.out_dirs & dir_t::SOUTH) == dir_t::SOUTH))
                return true;
            break;
        case dir_t::EAST:
            if ((out_dirs & dir_t::EAST) == dir_t::EAST && (t.out_dirs & dir_t::WEST) == dir_t::WEST
                    || !((out_dirs & dir_t::EAST) == dir_t::EAST) && !((t.out_dirs & dir_t::WEST) == dir_t::WEST))
                return true;
            break;
        case dir_t::SOUTH:
            if ((out_dirs & dir_t::SOUTH) == dir_t::SOUTH && (t.out_dirs & dir_t::NORTH) == dir_t::NORTH
                    || !((out_dirs & dir_t::SOUTH) == dir_t::SOUTH) && !((t.out_dirs & dir_t::NORTH) == dir_t::NORTH))
                return true;
            break;
        case dir_t::WEST:
            if ((out_dirs & dir_t::WEST) == dir_t::WEST && (t.out_dirs & dir_t::EAST) == dir_t::EAST
                    || !((out_dirs & dir_t::WEST) == dir_t::WEST) && !((t.out_dirs & dir_t::EAST) == dir_t::EAST))
                return true;
            break;
        default:
            break;
    }
    return false;
}

tile_t::tile_t(char16_t symbol, dir_t out_dirs)
{
    this->symbol = symbol;
    this->out_dirs = out_dirs;
}

template<>
std::vector<tile_t> superposition_t<tile_t>::all_possibilities = {
    tile_t(L' ', (dir_t)0),
    tile_t(L'━', dir_t::EAST | dir_t::WEST),
    tile_t(L'┃', dir_t::NORTH | dir_t::SOUTH),
    tile_t(L'╋', (dir_t::NORTH | dir_t::SOUTH) | (dir_t::EAST | dir_t::WEST)),
    tile_t(L'┻', (dir_t::EAST | dir_t::WEST) | dir_t::NORTH),
    tile_t(L'┳', (dir_t::EAST | dir_t::WEST) | dir_t::SOUTH),
    tile_t(L'┏', dir_t::EAST | dir_t::SOUTH),
    tile_t(L'┓', dir_t::WEST | dir_t::SOUTH),
    tile_t(L'┗', dir_t::EAST | dir_t::NORTH),
    tile_t(L'┛', dir_t::NORTH | dir_t::WEST)
};

int main(int argc, char** argv)
{
    superposition_t<tile_t> superposition;
    superposition.possibilities = superposition_t<tile_t>::all_possibilities;

    map_t<tile_t> map(12, 6);
    for (int i = 0; i < 12 * 6; ++i)
        map.map.push_back(superposition);

    setlocale(LC_ALL, "");

#ifdef TEST
    wprintf(L"NORTH:\n");
    for (tile_t t1 : superposition_t<tile_t>::all_possibilities)
        for (tile_t t2 : superposition_t<tile_t>::all_possibilities)
            wprintf(L"%lc\n%lc\n=>%u\n", t2.symbol, t1.symbol, t1.fits_dir(t2, dir_t::NORTH));
    
    wprintf(L"EAST:\n");
    for (tile_t t1 : superposition_t<tile_t>::all_possibilities)
        for (tile_t t2 : superposition_t<tile_t>::all_possibilities)
            wprintf(L"%lc%lc\n=>%u\n", t1.symbol, t2.symbol, t1.fits_dir(t2, dir_t::EAST));

    wprintf(L"SOUTH:\n");
    for (tile_t t1 : superposition_t<tile_t>::all_possibilities)
        for (tile_t t2 : superposition_t<tile_t>::all_possibilities)
            wprintf(L"%lc\n%lc\n=>%u\n", t1.symbol, t2.symbol, t1.fits_dir(t2, dir_t::SOUTH));
    
    wprintf(L"WEST:\n");
    for (tile_t t1 : superposition_t<tile_t>::all_possibilities)
        for (tile_t t2 : superposition_t<tile_t>::all_possibilities)
            wprintf(L"%lc%lc\n=>%u\n", t2.symbol, t1.symbol, t1.fits_dir(t2, dir_t::WEST));
#else
    bool success = false;
    size_t iter = 1;
    do
    {
        for (int i = 0; i < 12 * 6; ++i)
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
#endif

    return EXIT_SUCCESS;
}
