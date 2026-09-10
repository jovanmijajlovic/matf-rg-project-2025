// clang-format off
#include <glad/glad.h>
// clang-format on
#include <engine/graphics/Cube.hpp>
#include <engine/graphics/OpenGL.hpp>

namespace engine::graphics {

Cube::Cube(float s) {
    float vertices[] = {
        // clang-format off
        -s, -s, -s,  s, -s, -s,  s,  s, -s,  s,  s, -s, -s,  s, -s, -s, -s, -s,
        -s, -s,  s,  s, -s,  s,  s,  s,  s,  s,  s,  s, -s,  s,  s, -s, -s,  s,
        -s,  s,  s, -s,  s, -s, -s, -s, -s, -s, -s, -s, -s, -s,  s, -s,  s,  s,
         s,  s,  s,  s,  s, -s,  s, -s, -s,  s, -s, -s,  s, -s,  s,  s,  s,  s,
        -s, -s, -s,  s, -s, -s,  s, -s,  s,  s, -s,  s, -s, -s,  s, -s, -s, -s,
        -s,  s, -s,  s,  s, -s,  s,  s,  s,  s,  s,  s, -s,  s,  s, -s,  s, -s,
        // clang-format on
    };

    CHECKED_GL_CALL(glGenVertexArrays, 1, &m_vao);
    CHECKED_GL_CALL(glGenBuffers, 1, &m_vbo);
    CHECKED_GL_CALL(glBindVertexArray, m_vao);
    CHECKED_GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, m_vbo);
    CHECKED_GL_CALL(glBufferData, GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    CHECKED_GL_CALL(glEnableVertexAttribArray, 0);
    CHECKED_GL_CALL(glVertexAttribPointer, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    CHECKED_GL_CALL(glBindVertexArray, 0);
}

void Cube::draw() const {
    CHECKED_GL_CALL(glBindVertexArray, m_vao);
    CHECKED_GL_CALL(glDrawArrays, GL_TRIANGLES, 0, 36);
    CHECKED_GL_CALL(glBindVertexArray, 0);
}

void Cube::destroy() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

}// namespace engine::graphics