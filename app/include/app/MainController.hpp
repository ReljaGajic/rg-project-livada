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

struct DirLight {///STRUKTURA DIRLIGHT UBACENA KAO I SVE ODGOVARAJUCE PROMENE U CPP I GUI CPP
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 direction;
};

struct Flashlight {//STRUKTURA FLASHLIGHT UBACENA, KAO I SVE ODGOVARAJUCE PROMENE U CPP I GUI CPP
    enum class FlashlightState {
        Off,
        Ukljucivanje,
        Treperi,
        On
    };
    FlashlightState stanje{FlashlightState::Off};
    float timer{0.0f};
};


class MainController final : public engine::core::Controller {
public:
    std::string_view name() const override {
        return "app::MainController";
    }

    DirLight &dir_light() { return m_dir_light; }//GETTER PROMENJEN TKD POKAZUJE NA STRUKTURU

private:
    void initialize() override;

    bool loop() override;

    void poll_events() override;

    void update() override;

    void begin_draw() override;

    void draw() override;

    void draw_livada();

    void draw_drvo();

    void end_draw() override;

    void draw_skybox();

    void update_camera();

    bool m_cursor_enabled{true};

    DirLight m_dir_light{
            .ambient = {0.05f, 0.05f, 0.15f},
            .diffuse = {0.2f, 0.2f, 0.4f},
            .direction = {-0.8f, -1.0f, -0.3f}};

    Flashlight m_baterijska;

    void update_baterijska();

    float baterijska_snaga() const;
};
}// namespace app
#endif//MAINCONTROLLER_HPP