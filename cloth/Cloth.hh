//
// Created by anis on 29/03/2026.
//

#ifndef CLOTH_SIMULATION_CLOTH_HH
#define CLOTH_SIMULATION_CLOTH_HH
#include <vector>

#include "Stick.hh"
#include "object/Sphere.hh"
#include "object/Triangle.hh"

namespace cloth{
    class Cloth {
    public:
        Cloth(int width, int height, float spacing, float startX, float startY, float startZ, float mass, const std::vector<std::shared_ptr<object::Sphere>>& spheres, bool is_xz_plane);
        //Cloth(int width, int height, float spacing, float startX, float startY, float startZ, float mass);
        void update(float delta_time, float damping, float total_time, bool wind);
        std::vector<std::shared_ptr<object::Triangle>> to_triangle(texture::Texture_Material &material);
    private:
        int width;
        int height;
        std::vector<std::shared_ptr<Particle>> particles;
        std::vector<std::shared_ptr<Stick>> sticks;
        std::vector<std::shared_ptr<object::Sphere>> spheres;
        int NUM_ITERATIONS=4; // 30 7
    };
}

#endif //CLOTH_SIMULATION_CLOTH_HH
