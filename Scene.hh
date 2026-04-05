//
// Created by anis on 15/03/2026.
//

#ifndef TP1_SCENE_HH
#define TP1_SCENE_HH
#include <memory>
#include <vector>

#include "Camera.hh"
#include "light/Light.hh"
#include "object/Object.hh"


class Scene {
public:

    Scene(const std::vector<std::shared_ptr<object::Object>> &objects, const std::vector<std::shared_ptr<light::Light>> &lights, const Camera &camera,float ambient_intensity);
    const std::vector<std::shared_ptr<object::Object>>& get_objects() const {
        return objects;
    }

    void set_objects(const std::vector<std::shared_ptr<object::Object>> &objects) {
        this->objects = objects;
    }

    const std::vector<std::shared_ptr<light::Light>>& get_lights() const {
        return lights;
    }

    void set_lights(const std::vector<std::shared_ptr<light::Light>> &lights) {
        this->lights = lights;
    }

    const Camera& get_camera() const {
        return camera;
    }

    void set_camera(const Camera &camera) {
        this->camera = camera;
    }

    float get_ambient_intensity() const {
        return ambient_intensity;
    }

    void set_ambient_intensity(float ambient_intensity) {
        this->ambient_intensity = ambient_intensity;
    }

private:
    std::vector<std::shared_ptr<object::Object>> objects;
    std::vector<std::shared_ptr<light::Light>> lights;
    Camera camera;
    float ambient_intensity;
};


#endif //TP1_SCENE_HH