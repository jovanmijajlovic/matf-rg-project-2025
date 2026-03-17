//
// Created by jovan on 3/17/26.
//

#include "../include/MyApp.hpp"

#include <MainController.hpp>
#include <spdlog/spdlog.h>

namespace app {
void MyApp::app_setup() {
    spdlog::info("App setup completed!");
    auto main_controller = register_controller<app::MainController>();
    main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
}
}// namespace app