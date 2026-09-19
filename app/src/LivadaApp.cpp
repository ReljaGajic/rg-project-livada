#include <app/GUIController.hpp>
#include <app/LivadaApp.hpp>
#include <app/MainController.hpp>
#include <spdlog/spdlog.h>

namespace app {
void LivadaApp::app_setup() {
    auto main_controller = register_controller<MainController>();
    auto gui_controller = register_controller<GUIController>();
    main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
    gui_controller->after(main_controller);
}
}// namespace app

int main(int argc, char **argv) {
    try {
        return std::make_unique<app::LivadaApp>()->run(argc, argv);
    } catch (const engine::util::EngineError &e) {
        spdlog::error("{}", e.report());
        return 1;
    } catch (const std::exception &e) {
        spdlog::error("Unhandeled exception: {}", e.what());
        return 1;
    }
}