//
// Created by anis on 18/03/2026.
//

#ifndef TP1_RGB_F_HH
#define TP1_RGB_F_HH
#include <ostream>

namespace color{
    class RGB_f {
    public:
        RGB_f(float r, float g, float b);

        friend std::ostream & operator<<(std::ostream &os, const RGB_f &obj);

        float get_r() const {
            return r;
        }

        void set_r(float r) {
            this->r = r;
        }

        float get_g() const {
            return g;
        }

        void set_g(float g) {
            this->g = g;
        }

        float get_b() const {
            return b;
        }

        void set_b(float b) {
            this->b = b;
        }

    private:
        float r;
        float g;
        float b;
    };
    };



#endif //TP1_RGB_F_HH