#include <app/GUIController.hpp>
#include <app/LivadaApp.hpp>
#include <app/MainController.hpp>

namespace app {
void LivadaApp::app_setup() {
    auto main_controller = register_controller<MainController>();
    auto gui_controller = register_controller<GUIController>();
    main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
    gui_controller->after(main_controller);
}
}// namespace engine::test::app

int main(int argc, char **argv) {
    return std::make_unique<app::LivadaApp>()->run(argc, argv);
}
