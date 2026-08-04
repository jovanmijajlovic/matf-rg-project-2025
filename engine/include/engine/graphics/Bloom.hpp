/**
* @file Bloom.hpp
 * @brief Defines the Bloom class implementing HDR bright-pass extraction + gaussian blur bloom.
 */
#ifndef BLOOM_HPP
#define BLOOM_HPP

#include <cstdint>

namespace engine::resources {
class Shader;
}

namespace engine::graphics {

/**
 * @class Bloom
 * @brief HDR scene framebuffer (2 color attachments: scene + bright-pass) with ping-pong gaussian blur.
 */
class Bloom {
    friend class GraphicsController;

public:
    void begin_scene_capture() const;
    void end_scene_capture() const;
    void blur_bright_texture(const resources::Shader *blur_shader, int amount = 10) const;
    void render_final(const resources::Shader *final_shader, bool bloom_enabled, float exposure) const;
    void resize(int width, int height);
    void destroy();

private:
    Bloom() = default;
    void initialize(int width, int height);
    void create_framebuffers(int width, int height);
    void destroy_framebuffers();

    int m_width{0};
    int m_height{0};

    uint32_t m_hdr_fbo{0};
    uint32_t m_color_buffers[2]{0, 0}; // [0] scene, [1] bright-pass
    uint32_t m_depth_rbo{0};

    uint32_t m_pingpong_fbo[2]{0, 0};
    uint32_t m_pingpong_buffer[2]{0, 0};
    mutable bool m_last_horizontal{true};

    uint32_t m_quad_vao{0};
    uint32_t m_quad_vbo{0};
};

}// namespace engine::graphics
#endif//BLOOM_HPP