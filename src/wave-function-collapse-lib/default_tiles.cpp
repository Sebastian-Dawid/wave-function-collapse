#include "default_tiles.h"
#include <cstring>

bool wfc::unicode_tile_t::fits_dir(const tile_i& tile, const dir_t dir) const
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

float wfc::unicode_tile_t::get_weight() const
{
    return this->weight;
}

bool wfc::unicode_tile_t::operator==(const tile_i& other) const
{
    return false;
}

bool wfc::unicode_tile_t::operator==(const unicode_tile_t& other) const
{
    return other.symbol == this->symbol;
}

wfc::unicode_tile_t::unicode_tile_t(char16_t symbol, std::array<bool, 4> edges, float weight)
{
    this->symbol = symbol;
    this->edges  = edges;
    this->weight = weight;
}

bool wfc::mesh_tile_t::fits_dir(const tile_i& tile, const dir_t dir) const
{
    mesh_tile_t& t = (mesh_tile_t&)tile;
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

float wfc::mesh_tile_t::get_weight() const
{
    return this->weight;
}

bool wfc::mesh_tile_t::operator==(const tile_i& other) const
{
    return false;
}

bool wfc::mesh_tile_t::operator==(const mesh_tile_t& other) const
{
    if (this->mesh.indices.size() != other.mesh.indices.size()) return false;
    if (this->mesh.vertices.size() != other.mesh.vertices.size()) return false;
    
    for (std::size_t i = 0; i < this->mesh.indices.size(); ++i)
        if (this->mesh.indices[i] != other.mesh.indices[i]) return false;

    for (std::size_t i = 0; i < this->mesh.vertices.size(); ++i)
        if (this->mesh.vertices[i] != other.mesh.vertices[i]) return false;

    return true;
}

wfc::mesh_tile_t::mesh_tile_t(char16_t symbol, mesh_t mesh, std::array<bool, 4> edges, float weight)
{
    this->symbol = symbol;
    this->mesh = mesh;
    this->edges  = edges;
    this->weight = weight;
}
