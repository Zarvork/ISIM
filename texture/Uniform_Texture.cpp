//
// Created by anis on 13/03/2026.
//

#include "Uniform_Texture.hh"


namespace texture {
    Uniform_Texture::Uniform_Texture(float kd, float ks, const color::RGB& color,float ns,float ka) : properties(color, kd, ks, ns, ka) {
    }

    SurfaceProperties Uniform_Texture::get_properties(const geometry::Point4& p) const {
        return properties;
    }
}

