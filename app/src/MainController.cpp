//
// Created by jovan on 3/17/26.
//

#include "../include/MainController.hpp"

#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/platform/PlatformEventObserver.hpp>
#include <engine/resources/ResourcesController.hpp>
#include <spdlog/spdlog.h>

// clang-format off
#include <imgui_impl_opengl3_loader.h>
// clang-format on

namespace app {

class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
public:
    void on_mouse_move(engine::platform::MousePosition position) override;
};
void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    camera->rotate_camera(position.dx, position.dy);
}

void MainController::initialize() {
    engine::graphics::OpenGL::enable_depth_testing();
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());

    platform->set_enable_cursor(false);

    m_bulb_cube.emplace(0.05f);

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();
    camera->Position = glm::vec3(-2.5f, 3.0f, 25.0f);
    camera->Yaw = -90.0f;
    camera->Pitch = 0.0f;
    camera->rotate_camera(0.0f, 0.0f);
    camera->MovementSpeed = 6.0f;

    spdlog::info("MainController initialized");
}

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
        return false;
    }

    return true;
}

void MainController::update() {
    update_camera();
    update_lights();
    update_cars();
}

void MainController::update_camera() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();
    float dt = platform->dt();

    if (platform->key(engine::platform::KEY_W).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt);
    }
    if (platform->key(engine::platform::KEY_S).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt);
    }
    if (platform->key(engine::platform::KEY_A).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt);
    }
    if (platform->key(engine::platform::KEY_D).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt);
    }
    if (platform->key(engine::platform::KEY_SPACE).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::UP, dt);
    }
    if (platform->key(engine::platform::KEY_LEFT_SHIFT).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::DOWN, dt);
    }
    auto mouse = platform->mouse();
    camera->rotate_camera(mouse.dx, mouse.dy);
    camera->zoom(mouse.scroll);
}

void MainController::begin_draw() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    graphics->bloom()->begin_scene_capture();
}
void MainController::draw() {
    //lights setup

    setup_dir_light("basic");
    setup_dir_light("grass");

    //platform model

    glm::mat4 rt_model = glm::mat4(1.0f);
    draw_model("platform", "grass", rt_model);

    //road model

    glm::mat4 road_model = glm::mat4(1.0f);
    road_model = glm::translate(road_model, glm::vec3(0.0f, 0.02f, 6.45f));
    draw_model("road", "grass", road_model);


    glm::mat4 road_model_2 = glm::mat4(1.0f);
    road_model_2 = glm::translate(road_model_2, glm::vec3(0.0f, 0.02f, 13.5f));
    draw_model("road", "grass", road_model_2);


    //streetlight

    glm::mat4 streetlight_model = glm::mat4(1.0f);
    streetlight_model = glm::translate(streetlight_model, glm::vec3(-13.0f, -0.6f, 11.75f));
    streetlight_model = glm::rotate(streetlight_model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    streetlight_model = glm::scale(streetlight_model, glm::vec3(1.5f));
    draw_model("streetlight", "basic", streetlight_model);

    glm::mat4 streetlight_model_2 = glm::mat4(1.0f);
    streetlight_model_2 = glm::translate(streetlight_model_2, glm::vec3(8.0f, -0.6f, 11.75f));
    streetlight_model_2 = glm::rotate(streetlight_model_2, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    streetlight_model_2 = glm::scale(streetlight_model_2, glm::vec3(1.5f));
    draw_model("streetlight", "basic", streetlight_model_2);

    glm::vec3 bulb_offset_1(3.3672f, 4.5883f, 0.0166f);
    glm::vec3 bulb_offset_2(1.6576f, 4.4826f, 1.4452f);

    glm::vec3 lamp1_bulb1 = glm::vec3(streetlight_model   * glm::vec4(bulb_offset_1, 1.0f));
    glm::vec3 lamp1_bulb2 = glm::vec3(streetlight_model   * glm::vec4(bulb_offset_2, 1.0f));
    glm::vec3 lamp2_bulb1 = glm::vec3(streetlight_model_2 * glm::vec4(bulb_offset_1, 1.0f));
    glm::vec3 lamp2_bulb2 = glm::vec3(streetlight_model_2 * glm::vec4(bulb_offset_2, 1.0f));

    setup_lights("basic", lamp1_bulb1, lamp1_bulb2, lamp2_bulb1, lamp2_bulb2);
    setup_lights("grass", lamp1_bulb1, lamp1_bulb2, lamp2_bulb1, lamp2_bulb2);

    glm::vec3 bulb_color = glm::vec3(15.0f, 13.0f, 8.0f) * m_streetlight_intensity;
    glm::vec3 bulb_visual_offset(0.0f, -0.078f, 0.0f);

    draw_bulb(lamp1_bulb1 + bulb_visual_offset, bulb_color);
    draw_bulb(lamp1_bulb2 + bulb_visual_offset, bulb_color);
    draw_bulb(lamp2_bulb1 + bulb_visual_offset, bulb_color);
    draw_bulb(lamp2_bulb2 + bulb_visual_offset, bulb_color);

    //houses

    glm::mat4 house_model = glm::mat4(1.0f);
    house_model = glm::translate(house_model, glm::vec3(0.0f, 0.0f, -8.5f));
    house_model = glm::scale(house_model, glm::vec3(0.7f));
    draw_model("house", "basic", house_model);

    //cars

    glm::mat4 lambo_model = glm::mat4(1.0f);
    lambo_model = glm::translate(lambo_model, glm::vec3(-4.86f, 0.0f, -1.95f));
    lambo_model = glm::scale(lambo_model, glm::vec3(0.012f));
    draw_model("lambo", "basic", lambo_model);

    float drive_progress = (m_drive_state == DriveState::DRIVING || m_drive_state == DriveState::STOPPED) ? m_state_timer : 0.0f;
    const float N_SECONDS = 10.0f;
    float t = glm::clamp(drive_progress / N_SECONDS, 0.0f, 1.0f);

    glm::vec3 blue_start(16.5f, 0.0f, 6.0f);
    glm::vec3 blue_end(-16.238f, 0.0f, 6.0f);
    glm::vec3 blue_pos = glm::mix(blue_start, blue_end, t);

    glm::mat4 blue_model = glm::mat4(1.0f);
    blue_model = glm::translate(blue_model, blue_pos);
    blue_model = glm::rotate(blue_model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    blue_model = glm::scale(blue_model, glm::vec3(0.5f));
    draw_model("blue-car", "basic", blue_model);

    glm::vec3 taxi_start(-15.0f, 0.07f, 14.0f);
    glm::vec3 taxi_end(16.973f, 0.07f, 14.0f);
    glm::vec3 taxi_pos = glm::mix(taxi_start, taxi_end, t);

    glm::mat4 taxi_model = glm::mat4(1.0f);
    taxi_model = glm::translate(taxi_model, taxi_pos);
    taxi_model = glm::rotate(taxi_model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    taxi_model = glm::scale(taxi_model, glm::vec3(0.048f));
    draw_model("taxi", "basic", taxi_model);

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    graphics->bloom()->end_scene_capture();

    render_bloom_final();
}
void MainController::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}

void MainController::draw_model(const std::string &model_name, const std::string &shader_name, const glm::mat4 &model_matrix) {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    engine::resources::Model *car = resources->model(model_name);
    engine::resources::Shader *shader = resources->shader(shader_name);

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_mat4("model", model_matrix);

    shader->set_float("material.shininess", 32.0f);
    shader->set_vec3("material.specularColor", glm::vec3(0.5));

    car->draw(shader);
}

void MainController::setup_dir_light(const std::string &shader_name) {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    engine::resources::Shader *shader = resources->shader(shader_name);
    shader->use();

    shader->set_vec3("dirLight.direction", glm::vec3(0.3f, -1.0f, 0.2f));
    shader->set_vec3("dirLight.ambient",   glm::vec3(0.15f, 0.15f, 0.17f));
    shader->set_vec3("dirLight.diffuse",   glm::vec3(0.6f, 0.6f, 0.6f));
    shader->set_vec3("dirLight.specular",  glm::vec3(0.3f, 0.3f, 0.3f));
}

void MainController::setup_lights(const std::string &shader_name, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4) {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    engine::resources::Shader *shader = resources->shader(shader_name);

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    shader->use();

    glm::mat4 view = graphics->camera()->view_matrix();
    glm::vec3 cameraPos = glm::vec3(glm::inverse(view)[3]);
    shader->set_vec3("viewPos", cameraPos);

    glm::vec3 positions[4] = { pos1, pos2, pos3, pos4 };

    for (int i = 0; i < 4; i++) {
        std::string base = "spotLights[" + std::to_string(i) + "].";
        shader->set_vec3(base + "position", positions[i]);
        shader->set_vec3(base + "direction",    glm::vec3(0.0f, -1.0f, 0.0f));
        shader->set_vec3(base + "ambient",      glm::vec3(0.0f));
        shader->set_vec3(base + "diffuse",      glm::vec3(1.0f, 0.9f, 0.7f) * m_streetlight_intensity);
        shader->set_vec3(base + "specular",     glm::vec3(1.0f) * m_streetlight_intensity);
        shader->set_float(base + "cutOff",      glm::cos(glm::radians(25.0f)));
        shader->set_float(base + "outerCutOff", glm::cos(glm::radians(35.0f)));
        shader->set_float(base + "constant",    1.0f);
        shader->set_float(base + "linear",      0.09f);
        shader->set_float(base + "quadratic",   0.032f);
    }
}

void MainController::update_lights() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    float dt = platform->dt();

    const float change_speed = 2.0f;

    if (platform->key(engine::platform::KEY_1).is_down()) {
        m_streetlight_intensity -= change_speed * dt;
    }
    if (platform->key(engine::platform::KEY_2).is_down()) {
        m_streetlight_intensity += change_speed * dt;
    }

    m_streetlight_intensity = glm::clamp(m_streetlight_intensity, 0.0f, 3.0f);
}

void MainController::update_cars() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    float dt = platform->dt();

    const float M_SECONDS = 2.0f;
    const float N_SECONDS = 10.0f;

    bool action_down = platform->key(engine::platform::KEY_ENTER).is_down();
    if (action_down && !m_prev_action_down && m_drive_state == DriveState::IDLE) {
        m_drive_state = DriveState::WAITING;
        m_state_timer = 0.0f;
    }
    m_prev_action_down = action_down;

    m_state_timer += dt;

    if (m_drive_state == DriveState::WAITING && m_state_timer >= M_SECONDS) {
        m_drive_state = DriveState::DRIVING;
        m_state_timer = 0.0f;
    }
    else if (m_drive_state == DriveState::DRIVING && m_state_timer >= N_SECONDS) {
        m_drive_state = DriveState::STOPPED;
        m_state_timer = N_SECONDS;
    }
}

void MainController::draw_bulb(const glm::vec3 &position, const glm::vec3 &color) {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    engine::resources::Shader *shader = resources->shader("bulb");

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(1.6f, 0.3f, 6.0f));
    shader->set_mat4("model", model);
    shader->set_vec3("bulbColor", color);

    m_bulb_cube->draw();
}

void MainController::render_bloom_final() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

    engine::resources::Shader *blur_shader = resources->shader("blur");
    graphics->bloom()->blur_bright_texture(blur_shader, 10);

    engine::graphics::OpenGL::clear_buffers();

    engine::resources::Shader *final_shader = resources->shader("bloom_final");
    graphics->bloom()->render_final(final_shader, true, 0.8f);
}
}// namespace app