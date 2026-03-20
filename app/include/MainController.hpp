//
// Created by jovan on 3/17/26.
//

#ifndef MATF_RG_PROJECT_MAINCONTROLLER_HPP
#define MATF_RG_PROJECT_MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>
#include <glm/fwd.hpp>

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
    void draw_car(const std::string &model_name, const std::string &shader_name, const glm::mat4 &model_matrix);
};

}// namespace app

#endif//MATF_RG_PROJECT_MAINCONTROLLER_HPP
