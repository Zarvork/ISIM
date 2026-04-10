//
// Created by anis & lucil on 28/03/2026.
//

#include "Particle.hh"
#include "projective_geometry/Point4.hh"
#include "projective_geometry/Vector4.hh"
#include <iostream>
#include <cmath>

namespace cloth {
    Particle::Particle(float x, float y, float z, float w, float mass)
        : Point4(x, y, z, w), prev_x(x), prev_y(y), prev_z(z), mass(mass), is_pinned(false)

    {
    }

    void Particle::handle_sphere_collision(const std::vector<std::shared_ptr<object::Sphere>>& spheres) {
        // Iterate over each sphere of the scene
        for (const auto& s: spheres) {
            // Get the center and radius of the sphere
            auto center = s->get_center();
            float radius = s->get_radius();
            // Compute a vector from the sphere center to the particle
            auto sphere_to_point = *this - center;
            // Compute distance between particle and sphere center
            float distance = sphere_to_point.norm();
            // Verify if the particle is inside the sphere
            if (distance < radius) {
                // Project the particle to the closest point on the sphere’s surface
                // Normalize the normal vector
                sphere_to_point.normalize();
                // Add a small offset to avoid the cloth to be under the sphere
                geometry::Point4 new_position = center + sphere_to_point * radius;
  
                // Compute the velocity
                float vx = x - prev_x;
                float vy = y - prev_y;
                float vz = z - prev_z;

                // Compute the normal component of the velocity
                float nx = sphere_to_point.get_x();
                float ny = sphere_to_point.get_y();
                float nz = sphere_to_point.get_z();
                float vn = vx * nx + vy * ny + vz * nz;

                // Remove the normal component of the velocity that points inside the sphere
                if (vn < 0.f) {
                    vx -= vn * nx;
                    vy -= vn * ny;
                    vz -= vn * nz;
                }

                // Correct the particle position so it's on the surface of the sphere
                x = new_position.get_x();
                y = new_position.get_y();
                z = new_position.get_z();

                // Modify the particle's previous position with the adjusted velocity
                prev_x = x - vx;
                prev_y = y - vy;
                prev_z = z - vz;
            }
        }        
    }

    void Particle::update(float delta_time, float damping, float total_time, bool wind) {
        // delta_time = Time difference between the current frame and the previous one
        // damping = Controls how quickly the simulation loses energy

        // Don't update fixed (pinned) particle
        if (is_pinned) {return;}

        // Accumulated force acting on the particle
        geometry::Vector4 force{0.f, -9.81f, 0.f};

        geometry::Vector4 wind_force{
            0.1f * std::sin(total_time * 5.f),
            0.f,
            1.5f * std::abs(std::sin(z + total_time * 5.f) + std::cos(y + total_time * 5.f) / 3.f
        )};

        if (wind) {
            force.set_x(force.get_x() + wind_force.get_x());
            force.set_z(force.get_z() + wind_force.get_z());
        }

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