//
// Created by anis on 28/03/2026.
//

#include "Particle.hh"
#include "projective_geometry/Point4.hh"
#include "projective_geometry/Vector4.hh"
#include <algorithm>
#include <iostream>
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
                // Normalize the normal vector
                sphere_to_point.normalize();
                // Project the particle to the closest point on the sphere’s surface
                geometry::Point4 new_position = center + sphere_to_point * radius;
  
                // Compute the velocity
                geometry::Point4 prev_point{prev_x, prev_y, prev_z, 1.0f};
                geometry::Vector4 velocity = *this - prev_point;
                
                // Compute the normal component of the velocity
                float normal_velocity = sphere_to_point.dotProduct(velocity);
                
                // Remove the normal component of the velocity when it points inside the sphere (only tangential velocity left)
                if (normal_velocity < 0.f) {

                    // Isolate the tangential velocity
                    geometry::Vector4 tangential_velocity = velocity - sphere_to_point*normal_velocity;

                    // Handling friction (Coulomb friction model)
                    float penetration_depth = radius - distance;
                    float friction_constant = 0.95f; //0.3f

                    float tangential_velocity_norm = tangential_velocity.norm();

                    // Case where the tangential velocity norm is very low (no movement to avoid division by zero)
                    if (tangential_velocity_norm <= 1e-6f) {
                        velocity = geometry::Vector4{0.f,0.f,0.f};
                    }
                    else {
                        // Amount of tangential velocity reduction due to friction
                        float friction_force = penetration_depth * friction_constant;

                        // Compute the tangential velocity norm reduced by friction
                        float new_tangential_velocity_norm = tangential_velocity_norm - friction_force;

                        // Case where the friction is stronger than the tangential velocity (no movement)
                        if (new_tangential_velocity_norm <= 0.f) {
                            velocity = geometry::Vector4{0.f,0.f,0.f};
                        }
                        else {
                            // Scale the tangential velocity vector with its new norm
                            velocity = tangential_velocity*(new_tangential_velocity_norm/tangential_velocity_norm);
                        }
                    }
                }

                // Correct the particle position so it's on the surface of the sphere
                x = new_position.get_x();
                y = new_position.get_y();
                z = new_position.get_z();

                // Modify the particle's previous position with the adjusted velocity
                prev_x = x - velocity.get_x();
                prev_y = y - velocity.get_y();
                prev_z = z - velocity.get_z();
            }
        }        
    }

    void Particle::update(float delta_time, float damping) {
        // delta_time = Time difference between the current frame and the previous one
        // damping = Controls how quickly the simulation loses energy

        // Don't update fixed (pinned) particle
        if (is_pinned) {return;}

        // Accumulated force acting on the particle
        geometry::Vector4 force{0.f, -9.81f * mass, 0.f};

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