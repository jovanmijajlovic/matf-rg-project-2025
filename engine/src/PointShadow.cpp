// clang-format off
#include <glad/glad.h>
// clang-format on
#include <engine/graphics/OpenGL.hpp>
#include <engine/graphics/PointShadow.hpp>
#include <engine/resources/Shader.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace engine::graphics {

PointShadow::PointShadow(uint32_t resolution)
    : m_resolution(resolution) {
    CHECKED_GL_CALL(glGenFramebuffers, 1, &m_fbo);

    CHECKED_GL_CALL(glGenTextures, 1, &m_cubemap);
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_CUBE_MAP, m_cubemap);
    for (int i = 0; i < 6; i++) {
        CHECKED_GL_CALL(glTexImage2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                        (int) resolution, (int) resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }
    CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_fbo);
    CHECKED_GL_CALL(glFramebufferTexture, GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_cubemap, 0);
    CHECKED_GL_CALL(glDrawBuffer, GL_NONE);
    CHECKED_GL_CALL(glReadBuffer, GL_NONE);
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);
}

void PointShadow::begin_depth_capture(const resources::Shader *depth_shader, const glm::vec3 &light_pos,
                                       float near_plane, float far_plane) const {
    m_far_plane = far_plane;

    float aspect = 1.0f;
    glm::mat4 shadow_proj = glm::perspective(glm::radians(90.0f), aspect, near_plane, far_plane);

    glm::mat4 shadow_transforms[6] = {
        shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)),
        shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)),
        shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),
        shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)),
        shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)),
        shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3(0, 0, -1), glm::vec3(0, -1, 0)),
    };

    CHECKED_GL_CALL(glViewport, 0, 0, (int) m_resolution, (int) m_resolution);
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_fbo);
    CHECKED_GL_CALL(glClear, GL_DEPTH_BUFFER_BIT);

    depth_shader->use();
    for (int i = 0; i < 6; i++) {
        depth_shader->set_mat4("shadowMatrices[" + std::to_string(i) + "]", shadow_transforms[i]);
    }
    depth_shader->set_float("far_plane", far_plane);
    depth_shader->set_vec3("lightPos", light_pos);
}

void PointShadow::end_depth_capture(int screen_width, int screen_height) const {
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);
    CHECKED_GL_CALL(glViewport, 0, 0, screen_width, screen_height);
}

void PointShadow::bind_shadow_map(const resources::Shader *shader, const std::string &uniform_name, int texture_unit) const {
    CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE0 + texture_unit);
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_CUBE_MAP, m_cubemap);
    shader->set_int(uniform_name, texture_unit);
    shader->set_float("far_plane", m_far_plane);
}

void PointShadow::destroy() {
    glDeleteFramebuffers(1, &m_fbo);
    glDeleteTextures(1, &m_cubemap);
}

}// namespace engine::graphics