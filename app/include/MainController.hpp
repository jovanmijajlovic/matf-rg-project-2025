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
    void draw_model(const std::string &model_name, const std::string &shader_name, const glm::mat4 &model_matrix);
    void setup_dir_light(const std::string &shader_name);
    void setup_lights(const std::string &shader_name, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4);
};

}// namespace app

#endif//MATF_RG_PROJECT_MAINCONTROLLER_HPP
