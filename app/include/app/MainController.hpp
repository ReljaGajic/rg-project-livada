#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP

#include <engine/core/Engine.hpp>
#include <glm/glm.hpp>

namespace app {
class MainPlatformEventObserver final : public engine::platform::PlatformEventObserver {
public:
    void on_key(engine::platform::Key key) override;

    void on_mouse_move(engine::platform::MousePosition position) override;
};

class MainController final : public engine::core::Controller {
public:
    std::string_view name() const override {
        return "app::MainController";
    }

    glm::vec3 &dir_light_ambient() { return m_dir_light_ambient; }
    glm::vec3 &dir_light_diffuse() { return m_dir_light_diffuse; }
    glm::vec3 &dir_light_direction() { return m_dir_light_direction; }

private:
    void initialize() override;

    bool loop() override;

    void poll_events() override;

    void update() override;

    void begin_draw() override;

    void draw() override;

    void draw_livada();

    void end_draw() override;

    void draw_skybox();

    void update_camera();

    bool m_cursor_enabled{true};

    glm::vec3 m_dir_light_ambient{0.05f, 0.05f, 0.15f};
    glm::vec3 m_dir_light_diffuse{0.2f, 0.2f, 0.4f};
    glm::vec3 m_dir_light_direction{-0.8f, -1.0f, -0.3f};

    enum class FlashlightState {
        Off,
        Ukljucivanje,
        Treperi,
        On
    };

    void update_baterijska();
    float baterijska_snaga() const;

    FlashlightState m_baterijska_stanje{FlashlightState::Off};
    float m_baterijska_timer{0.0f};
};
}// namespace app
#endif//MAINCONTROLLER_HPP