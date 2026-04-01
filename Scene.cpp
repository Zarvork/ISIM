//
// Created by anis on 15/03/2026.
//

#include "Scene.hh"

Scene::Scene(const std::vector<object::Object *> &objects, const std::vector<light::Light *> &lights, const Camera &camera,float ambient_intensity)
:
objects(objects),
lights(lights),
camera(camera),
ambient_intensity(ambient_intensity)
{}
