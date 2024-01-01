#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <optional>
#include <set>
#include <vector>
#include <random>
#include <map>

enum struct dir_t : std::uint8_t
{
    NORTH =  1,
    EAST  =  2,
    SOUTH =  4,
    WEST  =  8,
    UP    = 16,
    DOWN  = 32
};

inline static std::vector<dir_t> ALL_DIRS = { dir_t::NORTH, dir_t::EAST, dir_t::SOUTH, dir_t::WEST, dir_t::UP, dir_t::DOWN };

inline dir_t operator|(dir_t self, dir_t other)
{
    return (dir_t)((std::uint8_t)self | (std::uint8_t)other);
}

inline dir_t operator&(dir_t self, dir_t other)
{
    return (dir_t)((std::uint8_t)self & (std::uint8_t)other);
}

struct tile_i
{
    virtual bool fits_dir(const tile_i& tile, const dir_t dir) const = 0;
    virtual float get_weight() const = 0;
};

template <typename T>
struct superposition_t
{
    static_assert(std::is_base_of<tile_i, T>::value, "`T` must implement `tile_i`");
    bool collapsed = false;
    std::vector<T> possibilities;
    static std::vector<T> all_possibilities;
    std::vector<T> find_possible_neighbors(const dir_t dir)
    {
        std::vector<T> neighbors;
        for (T& a_poss : all_possibilities)
        {
            for (T& poss : this->possibilities)
            {
                if (poss.fits_dir(a_poss, dir))
                {
                    neighbors.push_back(a_poss);
                    break;
                }
            }
        }
        return neighbors;
    }

    float shannon_entropy()
    {
        float total = 0.f, sum = 0.f;

        for (T poss : this->possibilities)
            total += poss.get_weight();

        for (T poss : this->possibilities)
            sum += poss.get_weight()/total * std::log2(poss.get_weight()/total);

        return -sum;
    }

    void collapse()
    {
        static std::random_device rd;
        static std::mt19937 rng(rd());
        std::vector<float> weights;
        for (T poss : this->possibilities)
            weights.push_back(poss.get_weight());
        std::discrete_distribution<std::size_t> idx(weights.begin(), weights.end());
        this->collapsed = true;
        std::size_t chosen = idx(rng);
        this->possibilities = { this->possibilities[chosen] };
    }
    void collapse(std::size_t chosen)
    {
        this->collapsed = true;
        this->possibilities = { this->possibilities[chosen] };
    }
};

template <typename T>
struct map_t
{
    static_assert(std::is_base_of<tile_i, T>::value, "`T` must implement `tile_i`");
    const std::size_t dim;
    const std::size_t width;
    const std::size_t depth;
    const std::size_t height;
    std::vector<superposition_t<T>> map;
    std::size_t collapses_left;
    superposition_t<T>& at(std::size_t x, std::size_t y)
    {
        return this->map[x + y * width];
    }
    std::optional<superposition_t<T>&> at(std::size_t x, std::size_t y, std::size_t z)
    {
        if (this->dim == 2) return std::nullopt;
        return this->map[x + y * width + z * width * depth];
    }

    void propagate(std::size_t index)
    {
        std::vector<std::size_t> queue = {};
        std::set<std::size_t> seen = {};
        queue.push_back(index);

        while (!queue.empty())
        {
            std::size_t val = queue.front();
            seen.insert(val);
            queue.erase(queue.begin());
            std::size_t width, depth, height;
            width = val % this->width;
            depth = ((val % (this->width * this->depth)) / this->width);
            height = val / (this->width * this->depth);
            superposition_t<T>& pos = this->map[val];
            for (const dir_t dir : ALL_DIRS)
            {
                std::vector<T> neighbors = pos.find_possible_neighbors(dir);
                std::vector<T> intersection = {};
                superposition_t<T>* next_pos = NULL;
                std::function set_possibilities_to_intersection = [&] {
                    for (T val : next_pos->possibilities)
                    {
                        for (T n : neighbors)
                        {
                            if (n.symbol == val.symbol)
                            {
                                intersection.push_back(val);
                                break;
                            }
                        }
                    }
                    next_pos->possibilities = intersection;
                };
                switch (dir) {
                    case dir_t::NORTH:
                        if (depth != this->depth - 1 && !seen.contains(val + this->width))
                        {
                            next_pos = &this->map[val + this->width];
                            if (next_pos->collapsed) break;
                            set_possibilities_to_intersection();
                            queue.push_back(val + this->width);
                            if (std::count(queue.begin(), queue.end(), val + this->width) > 1)
                            {
                                queue.erase(std::find(queue.begin(), queue.end(), val + this->width));
                            }
                        }
                        break;
                    case dir_t::EAST:
                        if (width != this->width - 1 && !seen.contains(val + 1))
                        {
                            next_pos = &this->map[val + 1];
                            if (next_pos->collapsed) break;
                            set_possibilities_to_intersection();
                            queue.push_back(val + 1);
                            if (std::count(queue.begin(), queue.end(), val + 1) > 1)
                            {
                                queue.erase(std::find(queue.begin(), queue.end(), val + 1));
                            }
                        }
                        break;
                    case dir_t::SOUTH:
                        if (depth != 0 && !seen.contains(val - this->width))
                        {
                            next_pos = &this->map[val - this->width];
                            if (next_pos->collapsed) break;
                            set_possibilities_to_intersection();
                            queue.push_back(val - this->width);
                            if (std::count(queue.begin(), queue.end(), val - this->width) > 1)
                            {
                                queue.erase(std::find(queue.begin(), queue.end(), val - this->width));
                            }
                        }
                        break;
                    case dir_t::WEST:
                        if (width != 0 && !seen.contains(val - 1))
                        {
                            next_pos = &this->map[val - 1];
                            if (next_pos->collapsed) break;
                            set_possibilities_to_intersection();
                            queue.push_back(val - 1);
                            if (std::count(queue.begin(), queue.end(), val - 1) > 1)
                            {
                                queue.erase(std::find(queue.begin(), queue.end(), val - 1));
                            }
                        }
                        break;
                    case dir_t::UP:
                        if (this->dim == 3 && height != this->height - 1 && !seen.contains(val + this->width * this->depth))
                        {
                            next_pos = &this->map[val + this->width * this->depth];
                            if (next_pos->collapsed) break;
                            set_possibilities_to_intersection();
                            queue.push_back(val + this->width * this->depth);
                            if (std::count(queue.begin(), queue.end(), val + this->width * this->depth) > 1)
                            {
                                queue.erase(std::find(queue.begin(), queue.end(), val + this->width * this->depth));
                            }
                        }
                        break;
                    case dir_t::DOWN:
                        if (this->dim == 3 && height != 0 && !seen.contains(val - this->width * this->depth))
                        {
                            next_pos = &this->map[val - this->width * this->depth];
                            if (next_pos->collapsed) break;
                            set_possibilities_to_intersection();
                            queue.push_back(val - this->width * this->depth);
                            if (std::count(queue.begin(), queue.end(), val - this->width * this->depth) > 1)
                            {
                                queue.erase(std::find(queue.begin(), queue.end(), val - this->width * this->depth));
                            }
                        }
                        break;
                }
            }
        }
    }

    map_t(std::size_t width, std::size_t height) : dim(2), width(width), depth(height), height(0)
    {
        this->map.reserve(width * height);
        this->collapses_left = width * height;
    }
    map_t(std::size_t width, std::size_t depth, std::size_t height) : dim(3), width(width), depth(depth), height(height)
    {
        this->map.reserve(width * depth * height);
        this->collapses_left = width * depth * height;
    }
};

template <typename T>
bool wave_function_collapse(map_t<T>& map)
{
    static std::random_device rd;
    static std::mt19937 rng(rd());
    
    while (map.collapses_left != 0)
    {
        float min_entropy = std::numeric_limits<float>::infinity();
        std::map<float, std::vector<std::size_t>> entropy_map;
        for (std::size_t i = 0; i < map.map.size(); ++i)
        {
            superposition_t<T>& pos = map.map[i];
            float entropy = std::numeric_limits<float>::infinity();
            if (pos.possibilities.size() > 1)
                entropy = pos.shannon_entropy();
            entropy_map[entropy].push_back(i);
            min_entropy = (min_entropy < entropy) ? min_entropy : entropy;
        }

        std::size_t possible_collapses = 0;
        std::vector<std::size_t> vec;

        for (std::pair<float, std::vector<std::size_t>> val : entropy_map)
        {
            if (val.first == min_entropy)
            {
                possible_collapses = val.second.size() - 1;
                vec = val.second;
                break;
            }
        }

        std::uniform_int_distribution<std::size_t> idx(0, possible_collapses);
        std::size_t chosen = idx(rng);

        superposition_t<T>& to_collapse = map.map[vec[chosen]];
        if (to_collapse.possibilities.size() == 0) return false;
        to_collapse.collapse();
        map.propagate(vec[chosen]);
        map.collapses_left--;
    }
    return true;
}
