//
// Created by anis on 25/03/2026.
//

#ifndef TP1_TRIANGLE_HH
#define TP1_TRIANGLE_HH
#include "Object.hh"

namespace object{
    class Triangle : public Object{
    public:
        Triangle(texture::Texture_Material &material,const geometry::Point4 &a,const geometry::Point4 &b,const geometry::Point4 &c);
        std::optional<geometry::Point4> does_ray_intersect(const geometry::Point4& p, const geometry::Vector4& v) const override;
        geometry::Vector4 get_normal_vector(const geometry::Point4& p) const override;
        texture::SurfaceProperties get_texture(const geometry::Point4& p) const override;

        const geometry::Point4& get_a() const {
            return a;
        }

        void set_a(const geometry::Point4 &a) {
            this->a = a;
        }

        const geometry::Point4& get_b() const {
            return b;
        }

        void set_b(const geometry::Point4 &b) {
            this->b = b;
        }

        const geometry::Point4& get_c() const {
            return c;
        }

        void set_c(const geometry::Point4 &c) {
            this->c = c;
        }

    private:
        geometry::Point4 a;
        geometry::Point4 b;
        geometry::Point4 c;
    };
}



#endif //TP1_TRIANGLE_HH