//
// Created by anis on 13/03/2026.
//

#ifndef TP1_UNIFORM_TEXTURE_HH
#define TP1_UNIFORM_TEXTURE_HH
#include "Texture_Material.hh"
#include "color/RGB.hh"

namespace texture{
    class Uniform_Texture : public Texture_Material {
    public:
        Uniform_Texture(float kd, float ks,const color::RGB& color,float ns,float ka);
        SurfaceProperties get_properties(const geometry::Point4& p) const override;
    private:
        SurfaceProperties properties;
    };
}

#endif //TP1_UNIFORM_TEXTURE_HH