//
// Created by jovan on 3/17/26.
//

#ifndef MATF_RG_PROJECT_MAINCONTROLLER_HPP
#define MATF_RG_PROJECT_MAINCONTROLLER_HPP
#include <cstdint>
#include <engine/core/Controller.hpp>
#include <glm/fwd.hpp>
#include <engine/graphics/Cube.hpp>
#include <optional>

namespace app {

class MainController : public engine::core::Controller {
public:
    std::string_view name() const override {
        return "app::MainController";
    };

private:
    void initialize() override;
    bool loop() override;
    void update() override;
    void begin_draw() override;
    void draw() override;
    void end_draw() override;

    void update_camera();
    void draw_model(const std::string &model_name, const std::string &shader_name, const glm::mat4 &model_matrix);
    void setup_dir_light(const std::string &shader_name);
    void setup_lights(const std::string &shader_name, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4);

    void update_lights();
    float m_streetlight_intensity = 1.0f;

    enum class DriveState {
        IDLE,
        WAITING,
        DRIVING,
        STOPPED
    };

    DriveState m_drive_state = DriveState::IDLE;
    float m_state_timer = 0.0f;
    bool m_prev_action_down = false;

    void update_cars();

    std::optional<engine::graphics::Cube> m_bulb_cube;
    void draw_bulb(const glm::vec3 &position, const glm::vec3 &color);
    void render_bloom_final();
};

}// namespace app

#endif//MATF_RG_PROJECT_MAINCONTROLLER_HPP
