//
// Created by anis on 15/03/2026.
//

#ifndef TP1_LIGHT_HH
#define TP1_LIGHT_HH
#include "projective_geometry/Point4.hh"

namespace light {
    class Light {
        public:
        Light () = default;
        virtual ~Light() = default;
        virtual float get_intensity() = 0;
        virtual const geometry::Point4& get_center() = 0;
    };
}

#endif //TP1_LIGHT_HH