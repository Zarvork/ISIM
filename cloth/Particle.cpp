//
// Created by anis on 28/03/2026.
//

#include "Particle.hh"
#include "projective_geometry/Vector4.hh"
#include <iostream>
namespace cloth {
    Particle::Particle(float x, float y, float z, float w, float mass)
        : Point4(x, y, z, w), prev_x(x), prev_y(y), prev_z(z), mass(mass), is_pinned(false)

    {
    }

    void Particle::update() {
        // Don't update fixed (pinned) particle
        if (is_pinned) {return;}

        // Time difference between the current frame and the previous one
        float delta_time = 1.f / 60.f; // 60 FPS 1.f/60.f

        // Controls how quickly the simulation loses energy
        float damping = 0.99f;

        // Accumulated force acting on the particle
        geometry::Vector4 force{0.f, -9.81f, 0.f};

        // Compute acceleration using Newton Second Law
        geometry::Vector4 acceleration{force.get_x()/mass, force.get_y()/mass, force.get_z()/mass};

        // The current position will become the prev position after change
        Point4 prevPosition{x,y,z,w};

        // Compute new position using Verlet Integration
        x = x + (x - prev_x) * damping + acceleration.get_x() * delta_time * delta_time;
        y = y + (y - prev_y) * damping + acceleration.get_y() * delta_time * delta_time;
        z = z + (z - prev_z) * damping + acceleration.get_z() * delta_time * delta_time;

        // Change the previous position to be the old current position
        prev_x = prevPosition.get_x();
        prev_y = prevPosition.get_y();
        prev_z = prevPosition.get_z();

    }
}