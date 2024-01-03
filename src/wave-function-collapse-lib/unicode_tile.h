#pragma once

#include <wfc-lib.h>

struct unicode_tile_t : tile_i
{
    char16_t symbol;
    std::array<bool, 4> edges = {0};
    float weight = 1.f;

    bool fits_dir(const tile_i& tile, const dir_t dir) const override;
    float get_weight() const override;

    unicode_tile_t(char16_t symbol, std::array<bool, 4> edges, float weight = 1.f);
};

#ifdef UNICODE_TILE_IMPLEMENTATION
bool unicode_tile_t::fits_dir(const tile_i& tile, const dir_t dir) const
{
    unicode_tile_t& t = (unicode_tile_t&)tile;
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

float unicode_tile_t::get_weight() const
{
    return this->weight;
}

unicode_tile_t::unicode_tile_t(char16_t symbol, std::array<bool, 4> edges, float weight)
{
    this->symbol = symbol;
    this->edges  = edges;
    this->weight = weight;
}
#endif
