//
// Created by anis on 25/03/2026.
//

#include "Triangle.hh"

#include <cmath>

namespace object{
    Triangle::Triangle(texture::Texture_Material &material,const geometry::Point4 &a,const geometry::Point4 &b,const geometry::Point4 &c)
        : Object(material), a(a), b(b),c(c)
    {}

    std::optional<geometry::Point4> Triangle::does_ray_intersect(const geometry::Point4& p, const geometry::Vector4& v) const {
        // Define two vectors for the plane
        geometry::Vector4 ab = b-a;
        geometry::Vector4 ac = c-a;


        // Define the normal vector of the plane
        geometry::Vector4 N = ab.cross(ac);
        N.normalize();

        // Case where the triangle and ray are parallel (no possible intersection)
        float N_dot_v = N.dotProduct(v);
        if (std::fabs(N_dot_v) < 0.00001) {
            return std::nullopt;
        }

        // Compute D in the plane equation Ax+By+Cz+D=0
        float D = -(N.get_x() * a.get_x() + N.get_y() * a.get_y() + N.get_z() * a.get_z());

        // Compute t in the equation of the ray = p + t * v
        float t = -((N.get_x()*p.get_x() + N.get_y()*p.get_y() + N.get_z() * p.get_z()) + D) / N_dot_v;

        // Case where the intersection point is behind the ray origin (triangle behind the ray) so no intersection point
        if (t < 0) {
            return std::nullopt;
        }

        // Compute the intersection point
        geometry::Point4 intersection_point = p + v * t;

        // Verify if the intersection point is inside the triangle
        geometry::Vector4 Np{};
        // Compute the vector from 'a' point to the intersection point
        geometry::Vector4 a_to_intersection_point = intersection_point - a;
        // Compute cross product between ab and the intersection point
        Np = ab.cross(a_to_intersection_point);
        // Compute the dot product between N and Np.
        // The sign of the result tells us whether the intersection point is to the left or right of that edge ab
        if (N.dotProduct(Np) < 0) return std::nullopt;


        // Compute the vector from 'b' point to the intersection point
        geometry::Vector4 b_to_intersection_point = intersection_point - b;
        geometry::Vector4 bc = c - b;
        // Compute cross product between cb and the intersection point
        Np = bc.cross(b_to_intersection_point);
        // Compute the dot product between N and Np.
        // The sign of the result tells us whether the intersection point is to the left or right of that edge cb
        if (N.dotProduct(Np) < 0) return std::nullopt;

        // Compute the vector from 'c' point to the intersection point
        geometry::Vector4 c_to_intersection_point = intersection_point - c;
        geometry::Vector4 ca = a - c;
        // Compute cross product between ca and the intersection point
        Np = ca.cross(c_to_intersection_point);
        // Compute the dot product between N and Np.
        // The sign of the result tells us whether the intersection point is to the left or right of that edge ca
        if (N.dotProduct(Np) < 0) return std::nullopt;

        return std::optional<geometry::Point4>{intersection_point};

    }

    geometry::Vector4 Triangle::get_normal_vector(const geometry::Point4& p) const {
        // Define two vectors for the plane
        geometry::Vector4 ab = b-a;
        geometry::Vector4 ac = c-a;


        // Define the normal vector of the plane
        geometry::Vector4 N = ab.cross(ac);
        N.normalize();

        return N;
    }
    texture::SurfaceProperties Triangle::get_texture(const geometry::Point4& p) const {
        return material.get_properties(p);
    }
}
