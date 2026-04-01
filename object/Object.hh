//
// Created by anis on 13/03/2026.
//

#ifndef TP1_OBJECT_HH
#define TP1_OBJECT_HH
#include <optional>

#include "../texture/Texture_Material.hh"
#include "../projective_geometry/Vector4.hh"

namespace object {
    class Object {
    public:
        virtual ~Object() = default;
        Object(texture::Texture_Material &material);

        virtual std::optional<geometry::Point4> does_ray_intersect(const geometry::Point4& p, const geometry::Vector4& v) const = 0;
        virtual geometry::Vector4 get_normal_vector(const geometry::Point4& p) const = 0;
        virtual texture::SurfaceProperties get_texture(const geometry::Point4& p) const = 0;
    protected:
        texture::Texture_Material& material;
    };
}
#endif //TP1_OBJECT_HH