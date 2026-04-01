//
// Created by anis on 15/03/2026.
//

#ifndef TP1_CAMERA_HH
#define TP1_CAMERA_HH
#include "projective_geometry/Point4.hh"


class Camera {
    public:
    const geometry::Point4& get_center() const {
        return center;
    }

    void set_center(const geometry::Point4 &center) {
        this->center = center;
    }

    const geometry::Point4& get_p() const {
        return p;
    }

    void set_p(const geometry::Point4 &p) {
        this->p = p;
    }

    const geometry::Vector4& get_up() const {
        return up;
    }

    void set_up(const geometry::Vector4 &up) {
        this->up = up;
    }

    float get_alpha() const {
        return alpha;
    }

    void set_alpha(float alpha) {
        this->alpha = alpha;
    }

    float get_beta() const {
        return beta;
    }

    void set_beta(float beta) {
        this->beta = beta;
    }

    float get_z_min() const {
        return z_min;
    }

    void set_z_min(float z_min) {
        this->z_min = z_min;
    }
 Camera(const geometry::Point4 &center, const geometry::Point4 &p, const geometry::Vector4 &up, float alpha,
        float beta, float z_min);

private:
    geometry::Point4 center;
    geometry::Point4 p;
    geometry::Vector4 up;
    // Angles are stored in radian but given in degree in the constructor
    float alpha;
    float beta;
    float z_min;
};


#endif //TP1_CAMERA_HH