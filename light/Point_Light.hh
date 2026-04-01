//
// Created by anis on 15/03/2026.
//

#ifndef TP1_POINT_LIGHT_HH
#define TP1_POINT_LIGHT_HH
#include "Light.hh"
#include "projective_geometry/Point4.hh"

namespace light{
    class Point_Light : public Light {
    public:
        Point_Light(float intensity, const geometry::Point4 &center);
        float get_intensity() override;
        const geometry::Point4& get_center() override;
    private:
        float intensity;
        geometry::Point4 center;
    };
}




#endif //TP1_POINT_LIGHT_HH