//
// Created by anis on 13/03/2026.
//

#ifndef TP1_SPHERE_HH
#define TP1_SPHERE_HH
#include "Object.hh"
#include <optional>
namespace object {
    class Sphere : public Object {
    public:
        Sphere(texture::Texture_Material &material,const geometry::Point4 &center, float radius);

        std::optional<geometry::Point4> does_ray_intersect(const geometry::Point4& p, const geometry::Vector4& v) const override;
        geometry::Vector4 get_normal_vector(const geometry::Point4& p) const override;
        texture::SurfaceProperties get_texture(const geometry::Point4& p) const override;

        const geometry::Point4& get_center() const {
            return center;
        }

        void set_center(const geometry::Point4 &p) {
            this->center = p;
        }

        float get_radius() const {
            return radius;
        }

        void set_radius(float radius) {
            this->radius = radius;
        }

    private:
        geometry::Point4 center;
        float radius;
    };
}



#endif //TP1_SPHERE_HH