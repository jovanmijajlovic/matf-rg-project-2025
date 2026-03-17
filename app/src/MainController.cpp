//
// Created by jovan on 3/17/26.
//

#include "../include/MainController.hpp"

#include <engine/platform/PlatformController.hpp>
#include <engine/platform/PlatformEventObserver.hpp>
#include <spdlog/spdlog.h>

namespace app {

void MainController::initialize() {
    spdlog::info("MainController initialized");
}

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
        return false;
    }

    return true;
}
}// namespace app