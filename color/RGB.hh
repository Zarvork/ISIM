//
// Created by anis on 15/03/2026.
//

#ifndef TP1_RGB_HH
#define TP1_RGB_HH

#include <cstdint>
#include <ostream>

namespace color{
    class RGB {
    public:
        RGB(uint8_t r, uint8_t g, uint8_t b);

        friend std::ostream & operator<<(std::ostream &os, const RGB &obj);

        uint8_t get_r() const {
            return r;
        }

        void set_r(uint8_t r) {
            this->r = r;
        }

        uint8_t get_g() const {
            return g;
        }

        void set_g(uint8_t g) {
            this->g = g;
        }

        uint8_t get_b() const {
            return b;
        }

        void set_b(uint8_t b) {
            this->b = b;
        }

    private:
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };
}




#endif //TP1_RGB_HH