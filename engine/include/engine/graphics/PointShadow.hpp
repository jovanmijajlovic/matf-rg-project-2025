/**
* @file PointShadow.hpp
 * @brief Defines the PointShadow class implementing omnidirectional (cubemap) shadow mapping for a point/spot light.
 */
#ifndef POINT_SHADOW_HPP
#define POINT_SHADOW_HPP

#include <cstdint>
#include <glm/glm.hpp>

namespace engine::resources {
class Shader;
}

namespace engine::graphics {

/**
 * @class PointShadow
 * @brief Depth cubemap framebuffer for omnidirectional shadow mapping from a single light position.
 */
class PointShadow {
public:
    explicit PointShadow(uint32_t resolution = 1024);

    void begin_depth_capture(const resources::Shader *depth_shader, const glm::vec3 &light_pos,
                             float near_plane = 1.0f, float far_plane = 25.0f) const;
    void end_depth_capture(int screen_width, int screen_height) const;

    void bind_shadow_map(const resources::Shader *shader, const std::string &uniform_name, int texture_unit) const;

    float far_plane() const {
        return m_far_plane;
    }

    void destroy();

private:
    uint32_t m_resolution;
    uint32_t m_fbo{0};
    uint32_t m_cubemap{0};
    mutable float m_far_plane{25.0f};
};

}// namespace engine::graphics
#endif//POINT_SHADOW_HPP