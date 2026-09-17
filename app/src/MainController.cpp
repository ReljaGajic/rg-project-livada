#include <app/GUIController.hpp>
#include <app/MainController.hpp>
#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/resources/ResourcesController.hpp>
#include <memory>
#include <spdlog/spdlog.h>

namespace app {

void MainPlatformEventObserver::on_key(engine::platform::Key key) {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    spdlog::info("Key event: frame={} key={}, state={}", platform->frame_time().frame_count, key.name(), key.state_str());
}

void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    spdlog::info("MousePosition: {} {}", position.x, position.y);
}

void MainController::initialize() {
    engine::graphics::OpenGL::enable_depth_testing();
    auto observer = std::make_unique<MainPlatformEventObserver>();
    engine::core::Controller::get<engine::platform::PlatformController>()->register_platform_event_observer(
            std::move(observer));
}

bool MainController::loop() {
    const auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).state() == engine::platform::Key::State::JustPressed) {
        return false;
    }
    return true;
}

void MainController::poll_events() {
    const auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KEY_F1).state() == engine::platform::Key::State::JustPressed) {
        m_cursor_enabled = !m_cursor_enabled;
        platform->set_enable_cursor(m_cursor_enabled);
    }
    if (platform->key(engine::platform::KEY_T).state() == engine::platform::Key::State::JustPressed) {
        if (m_baterijska_stanje == FlashlightState::Off) {
            m_baterijska_stanje = FlashlightState::Ukljucivanje;
            m_baterijska_timer = 0.0f;
            spdlog::info("ACTION_X: T pressed, flashlight powering on");
        } else {
            m_baterijska_stanje = FlashlightState::Off;
            m_baterijska_timer = 0.0f;
            spdlog::info("Flashlight turned off");
        }
    }
}

void MainController::update() {
    update_camera();
    update_baterijska();
}

void MainController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}

void MainController::draw() {
    draw_skybox();
    draw_livada();
    draw_drvo();
}

void MainController::end_draw() {
    engine::core::Controller::get<engine::platform::PlatformController>()->swap_buffers();
}

void MainController::draw_skybox() {
    auto shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("skybox");
    auto skybox_cube = engine::core::Controller::get<engine::resources::ResourcesController>()->skybox("night");
    engine::core::Controller::get<engine::graphics::GraphicsController>()->draw_skybox(shader, skybox_cube);
}

void MainController::draw_livada() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("basic");
    auto livada = engine::core::Controller::get<engine::resources::ResourcesController>()->model("livada");
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_mat4("model", glm::mat4(1.0f));
    shader->set_vec3("viewPos", graphics->camera()->Position);
    shader->set_vec3("dirLightDirection", glm::normalize(m_dir_light_direction));
    shader->set_vec3("dirLightAmbient", glm::vec3(m_dir_light_ambient));
    shader->set_vec3("dirLightDiffuse", glm::vec3(m_dir_light_diffuse));

    float spot_intensity = baterijska_snaga();
    shader->set_vec3("spotLightPosition", graphics->camera()->Position);
    shader->set_vec3("spotLightDirection", graphics->camera()->Front);
    shader->set_vec3("spotLightAmbient", glm::vec3(0.0f, 0.0f, 0.0f));
    shader->set_vec3("spotLightDiffuse", spot_intensity * glm::vec3(1.0f, 1.0f, 1.0f));
    shader->set_vec3("spotLightSpecular", spot_intensity * glm::vec3(1.0f, 1.0f, 1.0f));
    shader->set_float("spotLightCutOff", glm::cos(glm::radians(12.5f)));
    shader->set_float("spotLightOuterCutOff", glm::cos(glm::radians(20.0f)));
    shader->set_float("spotLightC", 1.0f);
    shader->set_float("spotLightL", 0.1f);
    shader->set_float("spotLightQ", 0.1f);

    livada->draw(shader);
}

void MainController::draw_drvo() {

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
            auto shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("basic");
            auto drvo = engine::core::Controller::get<engine::resources::ResourcesController>()->model("drvo");
            shader->use();
            shader->set_mat4("projection", graphics->projection_matrix());
            shader->set_mat4("view", graphics->camera()->view_matrix());
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(7.0f + 3.2f * i - 10.0f * j, -1.0f, -2.0f - 3.5f * i));
            shader->set_mat4("model", model);
            shader->set_vec3("viewPos", graphics->camera()->Position);
            shader->set_vec3("dirLightDirection", glm::normalize(m_dir_light_direction));
            shader->set_vec3("dirLightAmbient", glm::vec3(m_dir_light_ambient));
            shader->set_vec3("dirLightDiffuse", glm::vec3(m_dir_light_diffuse));

            float spot_intensity = baterijska_snaga();
            shader->set_vec3("spotLightPosition", graphics->camera()->Position);
            shader->set_vec3("spotLightDirection", graphics->camera()->Front);
            shader->set_vec3("spotLightAmbient", glm::vec3(0.0f, 0.0f, 0.0f));
            shader->set_vec3("spotLightDiffuse", spot_intensity * glm::vec3(1.0f, 1.0f, 1.0f));
            shader->set_vec3("spotLightSpecular", spot_intensity * glm::vec3(1.0f, 1.0f, 1.0f));
            shader->set_float("spotLightCutOff", glm::cos(glm::radians(12.5f)));
            shader->set_float("spotLightOuterCutOff", glm::cos(glm::radians(20.0f)));
            shader->set_float("spotLightC", 1.0f);
            shader->set_float("spotLightL", 0.1f);
            shader->set_float("spotLightQ", 0.1f);

            drvo->draw(shader);
        }
    }
}


void MainController::update_camera() {
    auto gui = engine::core::Controller::get<GUIController>();
    if (gui->is_enabled()) {
        return;
    }
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    float dt = platform->dt();
    if (platform->key(engine::platform::KEY_W).state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt);
    }
    if (platform->key(engine::platform::KEY_S).state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt);
    }
    if (platform->key(engine::platform::KEY_A).state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt);
    }
    if (platform->key(engine::platform::KEY_D).state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt);
    }
    auto mouse = platform->mouse();
    camera->rotate_camera(mouse.dx, mouse.dy);
    camera->zoom(mouse.scroll);
}

void MainController::update_baterijska() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    float dt = platform->dt();

    constexpr float trep_trep_trepni_s = 0.3f;// idemo za treptanje
    constexpr float pun_gas_svetli_s = 1.5f;  // ovde grmi lampa

    switch (m_baterijska_stanje) {
        case FlashlightState::Ukljucivanje:
            m_baterijska_timer += dt;
            if (m_baterijska_timer >= trep_trep_trepni_s) {
                m_baterijska_stanje = FlashlightState::Treperi;
                m_baterijska_timer = 0.0f;
                spdlog::info("EVENT_A triggered: flashlight flickering");
            }
            break;
        case FlashlightState::Treperi:
            m_baterijska_timer += dt;
            if (m_baterijska_timer >= pun_gas_svetli_s) {
                m_baterijska_stanje = FlashlightState::On;
                m_baterijska_timer = 0.0f;
                spdlog::info("EVENT_B triggered: flashlight stabilized");
            }
            break;
        default:
            break;
    }
}

float MainController::baterijska_snaga() const {
    switch (m_baterijska_stanje) {
        case FlashlightState::Treperi: {
            float trep = 0.6f + 0.4f * std::sin(m_baterijska_timer * 30.0f) * std::sin(m_baterijska_timer * 5.0f);
            return glm::clamp(trep, 0.0f, 1.0f);//da mi ne ispadne iz 0 1 opsega
        }
        case FlashlightState::On:
            return 1.0f;
        default:
            return 0.0f;
    }
}

}// namespace app
