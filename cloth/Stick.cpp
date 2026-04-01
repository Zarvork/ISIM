//
// Created by anis on 28/03/2026.
//

#include "Stick.hh"
#include "projective_geometry/Point4.hh"
#include "projective_geometry/Vector4.hh"
#include <cmath>
#include <iostream>

namespace cloth {
    Stick::Stick(const std::shared_ptr<Particle> &p1, const std::shared_ptr<Particle> &p2, float length) : p1(p1),p2(p2),length(length) {}

    void Stick::update() {
        // Satisfy the constraint defined by the stick

        // Get the inverses of the two masses

        float invmass1 = p1->get_is_pinned() ? 0.f : 1.f / p1->get_mass();
        float invmass2 = p2->get_is_pinned() ? 0.f : 1.f / p2->get_mass();
        
        // Get the current position of the two particles
        geometry::Point4 position_p1{p1->get_x(), p1->get_y(), p1->get_z(), p1->get_w()};
        geometry::Point4 position_p2{p2->get_x(), p2->get_y(), p2->get_z(), p2->get_w()};

        // Compute the distance between the two particles
        geometry::Vector4 delta = position_p2 - position_p1;
        float deltalength = delta.norm();

        // Compute the difference between the current distance and the wanted distance
        if (deltalength < 1e-6f) return; // Avoid division by zero
        if (invmass1+invmass2 < 1e-6f) return; // Avoid division by zero

        float diff = (deltalength - length) / (deltalength*(invmass1+invmass2));
        // Modify the position of the two particles so the distance between them is correct
        float diff_x = delta.get_x() * diff;
        float diff_y = delta.get_y() * diff;
        float diff_z =  delta.get_z() * diff;
        
        p1->set_x(p1->get_x() + invmass1*diff_x);
        p1->set_y(p1->get_y() + invmass1*diff_y);
        p1->set_z(p1->get_z() + invmass1*diff_z);

        p2->set_x(p2->get_x() - invmass2*diff_x);
        p2->set_y(p2->get_y() - invmass2*diff_y);
        p2->set_z(p2->get_z() - invmass2*diff_z);         
    }
}