//
// Created by jovan on 3/17/26.
//

#include "../include/MyApp.hpp"

#include <spdlog/spdlog.h>

namespace app {
void MyApp::app_setup() {
    spdlog::info("App setup completed!");
}
}// namespace app