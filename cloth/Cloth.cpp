//
// Created by anis on 29/03/2026.
//

#include "Cloth.hh"
#include "cloth/Particle.hh"
#include "object/Sphere.hh"
#include "object/Triangle.hh"
#include <cmath>
#include <memory>
#include <iostream>
#include <random>

namespace cloth{
    Cloth::Cloth(int width, int height, float spacing, float startX, float startY, float startZ, float mass, const std::vector<std::shared_ptr<object::Sphere>>& spheres, bool is_xz_plane):
    width(width),
    height(height),
    spheres(spheres)
    {
        float diagonal_spacing = std::sqrt(spacing*spacing + spacing*spacing);
        // Necessary to generate random noise for z particles position
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> noise(-0.01f, 0.01f);
        
        // Create each particle of the cloth
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Add noise so particles are not in the same plane
                float noise_value = noise(gen);
                // Create the particle with the spacing
                // Check if we want the cloth to be in XZ plane or XY plane
                std::shared_ptr<Particle> p = nullptr;
                if (is_xz_plane) {
                    p = std::make_shared<Particle>(startX + x * spacing, startY + noise_value,startZ + y * spacing,1,mass);
                } 
                else {
                    p = std::make_shared<Particle>(startX + x * spacing, startY + y * spacing , startZ + noise_value,1,mass);
                }
                particles.push_back(p);
            }
        }

        // Create all the sticks
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Get the current particle
                std::shared_ptr<Particle> current_particle = particles.at((y * width) + x);

                // Add Structural Springs
                if (x != width - 1) {
                    std::shared_ptr<Particle>& right_particle = particles.at((y * width) + (x + 1));
                    std::shared_ptr<Stick> stick = std::make_shared<Stick>(right_particle, current_particle, spacing);
                    sticks.push_back(stick);
                }

                if (y != height - 1) {
                    std::shared_ptr<Particle>& bottom_particle = particles.at(((y + 1) * width) + x);
                    std::shared_ptr<Stick> stick = std::make_shared<Stick>(bottom_particle, current_particle, spacing);
                    sticks.push_back(stick);
                }

                // Add Shearing Springs
                if (y != height - 1 && x != width - 1) {
                    std::shared_ptr<Particle>& bottom_right_particle = particles.at(((y + 1) * width) + (x + 1));
                    std::shared_ptr<Stick> stick = std::make_shared<Stick>(bottom_right_particle, current_particle, diagonal_spacing);
                    sticks.push_back(stick); 
                }

                if (y != height - 1 && x != 0) {
                    std::shared_ptr<Particle>& bottom_left_particle = particles.at(((y + 1) * width) + (x - 1));
                    std::shared_ptr<Stick> stick = std::make_shared<Stick>(bottom_left_particle, current_particle, diagonal_spacing);
                    sticks.push_back(stick); 
                }

                // Add Bending Springs
                if (x < width - 2) {
                    std::shared_ptr<Particle>& next_next_right = particles.at((y * width) + (x + 2));
                    std::shared_ptr<Stick> stick = std::make_shared<Stick>(next_next_right, current_particle, spacing * 2.0f);
                    sticks.push_back(stick);
                }
                if (y < height - 2) {
                    std::shared_ptr<Particle>& next_next_bottom = particles.at(((y + 2) * width) + x);
                    std::shared_ptr<Stick> stick = std::make_shared<Stick>(next_next_bottom, current_particle, spacing * 2.0f);
                    sticks.push_back(stick);
                }
            }
        }

        // Pin the particles at the top (necessary for SIMULATION OF THE CLOTH ACTING LIKE A FLAG WITH WIND)
        //particles.at(((height - 1) * width) + 0)->set_is_pinned(true);
        //particles.at(((height - 1) * width) + (width - 1))->set_is_pinned(true);

        // Pin the particles at the 4 corners (necessary for SIMULATION OF THE CLOTH PINNED BY 4 corners)
        //particles.at(((height - 1) * width) + 0)->set_is_pinned(true);
        //particles.at(((height - 1) * width) + (width - 1))->set_is_pinned(true);
        //particles.at((0 * width) + 0)->set_is_pinned(true);
        //particles.at((0 * width) + (width - 1))->set_is_pinned(true);

    }

    void Cloth::update(float delta_time, float damping, float total_time, bool wind) {
        // Update the position of all particles in the cloth
        for (std::shared_ptr<Particle>&p: particles) {
            p->update(delta_time, damping, total_time, wind);
        }

        // Satisfy all the constraints
        for (int i = 0; i < NUM_ITERATIONS; i++) {
            
            // Distance constraint
            for (std::shared_ptr<Stick>&s: sticks) {
                s->update();
            }

            // Sphere Collision constraint
            for (std::shared_ptr<Particle>&p: particles) {
                p->handle_sphere_collision(spheres);
            }
        }        
    }

    std::vector<std::shared_ptr<object::Triangle>> Cloth::to_triangle(texture::Texture_Material &material) {
        std::vector<std::shared_ptr<object::Triangle>> result;
        
        // Iterate over the vector of particles
        for (int y = 0; y < height - 1; y++) {
            for (int x = 0; x < width - 1; x++) {
                // Get the 4 corners needed for the two triangles
                auto& p1 = particles.at((y * width) + x);
                auto& p2 = particles.at((y * width) + (x + 1));
                auto& p3 = particles.at(((y + 1) * width) + x);
                auto& p4 = particles.at(((y + 1) * width) + (x + 1));
                
                // Add the created triangles in the vector
                result.push_back(std::make_shared<object::Triangle>(material, *p1, *p2, *p3));
                result.push_back(std::make_shared<object::Triangle>(material, *p2, *p4, *p3));
            }
        }
        return result;
    }
}
