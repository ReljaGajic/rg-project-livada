
#ifndef LIVADAAPP_HPP
#define LIVADAAPP_HPP

#include <engine/core/Engine.hpp>

namespace app {
class LivadaApp final : public engine::core::App {
    void app_setup() override;
};
}// namespace app
#endif//LIVADAAPP_HPP
