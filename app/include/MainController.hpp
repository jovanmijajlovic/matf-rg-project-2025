//
// Created by jovan on 3/17/26.
//

#ifndef MATF_RG_PROJECT_MAINCONTROLLER_HPP
#define MATF_RG_PROJECT_MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>

namespace app {

class MainController : public engine::core::Controller {
public:
    std::string_view name() const override {
        return "app::MainController";
    };

private:
    void initialize() override;
};

}// namespace app

#endif//MATF_RG_PROJECT_MAINCONTROLLER_HPP
