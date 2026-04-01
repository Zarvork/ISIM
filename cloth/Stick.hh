//
// Created by anis on 28/03/2026.
//

#ifndef CLOTH_SIMULATION_STICK_HH
#define CLOTH_SIMULATION_STICK_HH
#include <memory>

#include "Particle.hh"


namespace cloth {
    class Stick {
    public:
        Stick(const std::shared_ptr<Particle> &p1, const std::shared_ptr<Particle> &p2, float length);
        void update();
    private:
        std::shared_ptr<Particle> p1;
        std::shared_ptr<Particle> p2;
        float length;
    };
}

#endif //CLOTH_SIMULATION_STICK_HH
