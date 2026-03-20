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
    engine::graphics::OpenGL::clear_buffers();
}
void MainController::draw() {
    glm::mat4 rt_model = glm::mat4(1.0f);
    draw_model("platform", "grass", rt_model);

    glm::mat4 house_model = glm::mat4(1.0f);
    house_model = glm::translate(house_model, glm::vec3(-10.0f, 0.0f, -14.0f));
    house_model = glm::scale(house_model, glm::vec3(0.3f));
    draw_model("house", "basic", house_model);

    glm::mat4 house_model2 = glm::mat4(1.0f);
    house_model2 = glm::translate(house_model2, glm::vec3(10.f, 0.0f, -14.0f));
    house_model2 = glm::scale(house_model2, glm::vec3(0.3f));
    draw_model("house", "basic", house_model2);

    glm::mat4 blue_model = glm::mat4(1.0f);
    blue_model = glm::translate(blue_model, glm::vec3(7.7f, 0.0f, -11.0f));
    blue_model = glm::scale(blue_model, glm::vec3(0.28f));
    draw_model("blue-car", "basic", blue_model);

    glm::mat4 lambo_model = glm::mat4(1.0f);
    lambo_model = glm::translate(lambo_model, glm::vec3(-12.1f, 0.0f, -11.0f));
    lambo_model = glm::scale(lambo_model, glm::vec3(0.006f));
    draw_model("lambo", "basic", lambo_model);

    glm::mat4 taxi_model = glm::mat4(1.0f);
    taxi_model = glm::translate(taxi_model, glm::vec3(10.0f, 0.0f, 0.0f));
    taxi_model = glm::scale(taxi_model, glm::vec3(0.05f));
    // draw_model("taxi", "basic", taxi_model);
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

    car->draw(shader);
}
}// namespace app