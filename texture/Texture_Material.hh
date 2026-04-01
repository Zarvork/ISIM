//
// Created by anis on 13/03/2026.
//

#ifndef TP1_TEXTURE_MATERIAL_HH
#define TP1_TEXTURE_MATERIAL_HH

#include <vector>

#include "../projective_geometry/Point4.hh"
#include "color/RGB.hh"


namespace texture{
    struct SurfaceProperties {
        color::RGB color;
        float kd;
        float ks;
        float ns;
        float ka;
    };

    class Texture_Material {
    public:
        virtual ~Texture_Material() = default;
        virtual SurfaceProperties get_properties(const geometry::Point4& p) const = 0;
    };
}
#endif //TP1_TEXTURE_MATERIAL_HH