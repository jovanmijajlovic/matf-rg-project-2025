/**
* @file Cube.hpp
 * @brief Defines the Cube class - a simple reusable unit-cube primitive for debug/marker rendering.
 */
#ifndef CUBE_HPP
#define CUBE_HPP

#include <cstdint>

namespace engine::graphics {

/**
 * @class Cube
 * @brief A simple generated cube mesh (no textures), useful for markers, light bulbs, debug shapes, etc.
 */
class Cube {
public:
    /**
     * @brief Constructs a cube of the given half-extent size, centered at origin.
     */
    explicit Cube(float half_size = 0.15f);

    void draw() const;
    void destroy();

private:
    uint32_t m_vao{0};
    uint32_t m_vbo{0};
};

}// namespace engine::graphics
#endif//CUBE_HPP