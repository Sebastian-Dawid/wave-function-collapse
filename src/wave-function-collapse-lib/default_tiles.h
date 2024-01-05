#pragma once

/*
 *
 * This file contains definitions for basic tile types with only simple adjacency rules.
 *
 * File:   default_tiles.h
 * Author: Sebastian Dawid
 */

#include "wfc-lib.h"
#include <glm/glm.hpp>

namespace wfc
{
    struct unicode_tile_t : tile_i
    {
        char16_t symbol;
        std::array<bool, 4> edges = {0};
        float weight = 1.f;

        bool fits_dir(const tile_i& tile, const dir_t dir) const override;
        float get_weight() const override;
        bool operator==(const tile_i& other) const override;
        bool operator==(const unicode_tile_t& other) const;

        unicode_tile_t(char16_t symbol, std::array<bool, 4> edges, float weight = 1.f);
    };

    struct mesh_tile_t : tile_i
    {
        char16_t symbol;
        struct mesh_t
        {
            std::vector<glm::vec3> vertices;
            std::vector<std::size_t> indices;
        } mesh;
        std::array<bool, 4> edges = {0};
        float weight = 1.f;

        bool fits_dir(const tile_i& tile, const dir_t dir) const override;
        float get_weight() const override;
        bool operator==(const tile_i& other) const override;
        bool operator==(const mesh_tile_t& other) const;

        mesh_tile_t(char16_t symbol, mesh_t mesh, std::array<bool, 4> edges, float weight = 1.f);
    };
};
