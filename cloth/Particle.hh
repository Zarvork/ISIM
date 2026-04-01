//
// Created by anis on 28/03/2026.
//

#ifndef CLOTH_SIMULATION_PARTICLE_HH
#define CLOTH_SIMULATION_PARTICLE_HH
#include "projective_geometry/Point4.hh"

namespace cloth {
    class Particle : public geometry::Point4{
    public:
        Particle(float x, float y, float z, float w, float mass);
        void update();

        float get_mass() {return mass;}

        void set_is_pinned(bool is_pinned) {this->is_pinned = is_pinned;}

        bool get_is_pinned(){return is_pinned;}

    private:
        float prev_x;
        float prev_y;
        float prev_z;
        float mass; // Between 0.1f to 1.0f is good
        bool is_pinned;
    };
}





#endif //CLOTH_SIMULATION_PARTICLE_HH
