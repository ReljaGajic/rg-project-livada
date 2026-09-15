#include <app/GUIController.hpp>
#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <imgui.h>

namespace app {
void GUIController::initialize() {
    set_enable(false);
}

void GUIController::poll_events() {
    const auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_F2).state() == engine::platform::Key::State::JustPressed) {
        set_enable(!is_enabled());
    }
}

void GUIController::draw() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto main_controller = engine::core::Controller::get<MainController>();
    auto camera = graphics->camera();
    graphics->begin_gui();
    ImGui::Begin("Camera info");
    const auto &c = *camera;
    ImGui::Text("Camera position: (%f, %f, %f)", c.Position.x, c.Position.y, c.Position.z);
    ImGui::Text("(Yaw, Pitch): (%f, %f)", c.Yaw, c.Pitch);
    ImGui::End();

    ImGui::Begin("Lighting");
    ImGui::Text("Directional light (moonlight)");
    ImGui::ColorEdit3("Moon ambient", &main_controller->dir_light_ambient()[0]);
    ImGui::ColorEdit3("Moon diffuse", &main_controller->dir_light_diffuse()[0]);
    ImGui::SliderFloat3("Moon direction", &main_controller->dir_light_direction()[0], -0.1f, 1.0f);
    ImGui::End();

    graphics->end_gui();
}
}// namespace app
