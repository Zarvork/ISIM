//
// Created by anis on 13/03/2026.
//

#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "Image.hh"
#include "Scene.hh"
#include "cloth/Cloth.hh"
#include "cloth/Particle.hh"
#include "color/RGB_f.hh"
#include "light/Point_Light.hh"
#include "object/Object.hh"
#include "object/Sphere.hh"
#include "object/Triangle.hh"
#include "projective_geometry/Point4.hh"
#include "projective_geometry/Vector4.hh"
#include "texture/Uniform_Texture.hh"
#include <future>
#include <format>


void compute_camera_coordinate_system(const Camera& camera, geometry::Vector4& W, geometry::Vector4& U,  geometry::Vector4& V) {
    W = (camera.get_p() - camera.get_center());
    U = W.cross(camera.get_up());
    V = U.cross(W);
    W.normalize();
    U.normalize();
    V.normalize();
}

object::Object* compute_closest_object(const geometry::Point4& ray_origin,const geometry::Vector4& ray_direction,const std::vector<std::shared_ptr<object::Object>>& objects,std::optional<geometry::Point4>& closest_object_coord) {
    object::Object* closest_object{nullptr};
    // Iterate over each object
    for (const std::shared_ptr<object::Object>& o : objects) {
        object::Object* obj_ptr = o.get();
        // Get the intersection point between the ray and the object
        auto intersection_point = o->does_ray_intersect(ray_origin,ray_direction);
        // If intersection point found
        if (intersection_point.has_value()) {
            // Verify if we already have the closest object
            if (closest_object_coord.has_value()) {
                // Case where the closest object is a sphere and the compared object is a triangle
                object::Triangle* t = dynamic_cast<object::Triangle*>(obj_ptr);
                object::Sphere* s = dynamic_cast<object::Sphere*>(closest_object);

                // Case where the closest object is a triangle and the compared object is a sphere
                if (t == nullptr && s == nullptr) {
                    t = dynamic_cast<object::Triangle*>(closest_object);
                    s = dynamic_cast<object::Sphere*>(obj_ptr);
                }

                // Handle the case where a triangle is under the sphere but it has to be drawn
                if (t != nullptr && s != nullptr) {
                    // Get the center and radius of the sphere
                    geometry::Point4 center = s->get_center();
                    float radius = s->get_radius();
                    // Get the 3 points of the triangle
                    geometry::Vector4 sphere_to_a = t->get_a() - center;
                    geometry::Vector4 sphere_to_b = t->get_b() - center;
                    geometry::Vector4 sphere_to_c = t->get_c() - center;
                    // Check to see if at least one point is on the sphere
                    if (sphere_to_a.norm() - radius < 0.01f ||
                        sphere_to_b.norm() - radius < 0.01f ||
                        sphere_to_c.norm() - radius < 0.01f)
                    {
                        // Consider the triangle as the closest object
                        if (closest_object != t) {
                             closest_object_coord = intersection_point;
                             closest_object = obj_ptr;
                        }
                        continue;
                    }
                }
                // Check which object is closer to the ray origin point
                float distance_closest_object = (ray_origin - closest_object_coord.value()).norm();
                float distance_intersection_point = (ray_origin - intersection_point.value()).norm();
                if (distance_intersection_point < distance_closest_object) {
                    closest_object_coord = intersection_point;
                    closest_object = obj_ptr;
                }
            } else {
                closest_object_coord = intersection_point;
                closest_object = obj_ptr;
            }
        }
    }
    return closest_object;
}

color::RGB_f compute_diffusion_intensity(const texture::SurfaceProperties& texture, float light_intensity,const geometry::Vector4& N,const geometry::Vector4& L) {
    color::RGB_f result{0.f,0.f,0.f};
    // Clamp value to avoid negative value
    float dot = std::max(0.0f, N.dotProduct(L));
    // Base formula to compute diffusion intensity
    float intensity_diffusion_base = texture.kd * light_intensity * dot;
    // Compute diffusion for R,G,B
    float intensity_diffusion_r = intensity_diffusion_base * (texture.color.get_r()/255.f);
    float intensity_diffusion_g = intensity_diffusion_base * (texture.color.get_g()/255.f);
    float intensity_diffusion_b = intensity_diffusion_base * (texture.color.get_b()/255.f);

    result.set_r(intensity_diffusion_r);
    result.set_g(intensity_diffusion_g);
    result.set_b(intensity_diffusion_b);

    return result;
}
geometry::Vector4 compute_reflected_ray_direction(const geometry::Vector4& ray_direction, const geometry::Vector4& N) {
    // Compute the inverse ray director vector (from the intersection point to the camera center)
    geometry::Vector4 inverse_ray_direction = ray_direction * -1;
    inverse_ray_direction.normalize();
    // Compute the reflected vector
    float dot_reflected = std::max(0.0f, N.dotProduct(inverse_ray_direction));
    geometry::Vector4 reflected_vector = N*(2*dot_reflected) - inverse_ray_direction;
    reflected_vector.normalize();
    return reflected_vector;
}

color::RGB_f compute_specular_intensity(const texture::SurfaceProperties& texture, float light_intensity,const geometry::Vector4& ray_direction,const geometry::Vector4& N,const geometry::Vector4& L) {

    // Calculating the direction vector of the reflected ray
    geometry::Vector4 reflected_vector = compute_reflected_ray_direction(ray_direction, N);

    // Clamp value to avoid negative value
    float dot = std::max(0.0f, reflected_vector.dotProduct(L));
    // Base formula to compute specular intensity
    float intensity_specular_base = texture.ks * light_intensity * std::pow(dot,texture.ns);
    color::RGB_f result{intensity_specular_base,intensity_specular_base,intensity_specular_base};
    return result;
}

color::RGB_f compute_local_illumination(const std::vector<std::shared_ptr<light::Light>>& lights,
    const object::Object* closest_object,
    const geometry::Point4& closest_object_coord,
    const geometry::Vector4& ray_direction,
    float ambient_intensity,
    const std::vector<std::shared_ptr<object::Object>>& objects,
    int n
    )
{
    // Constant value to avoid the ray to touch the start object (very small distance)
    float epsilon = 1e-4f;
    texture::SurfaceProperties texture = closest_object->get_texture(closest_object_coord);
    color::RGB_f new_color{0.f,0.f,0.f};
    // Compute the normal vector of the intersection point
    geometry::Vector4 N = closest_object->get_normal_vector(closest_object_coord);

    // The normal vector must always point to the inverse direction of the camera ray (back-face culling for triangle)
    if (N.dotProduct(ray_direction) > 0) {
        N = N * -1.0f;
    }

    // Iterate over each light
    for (auto l: lights) {
        // Compute the vector from the intersection point to the source light
        geometry::Vector4 L = l->get_center() - closest_object_coord;
        L.normalize();

        // Ignore the source light if it is obscured by another object
        // Compute the closest object between the intersection point and the source light
        std::optional<geometry::Point4> closest_shadow_object_coord{std::nullopt};
        // We do + (N * epsilon) to avoid the intersection point to be itself
        object::Object* closest_shadow_object = compute_closest_object(closest_object_coord + (N * epsilon),L, objects, closest_shadow_object_coord);

        // Verify if the ray intersects an object. If it is the case we discard the light source
        // occluded by an object (because it is not visible).
        if (closest_shadow_object != nullptr) {
            // We don't exclude the source light if the intersection point is behind the source light
            float distance_between_object_and_light = (l->get_center() - closest_object_coord).norm();
            float distance_between_object_and_intersection_point = (closest_shadow_object_coord.value() - closest_object_coord).norm();
            if (distance_between_object_and_intersection_point <= distance_between_object_and_light) {
                continue;
            }
        }

        // Compute the diffusion intensity for the source light at intersection point
        color::RGB_f diffusion_intensity = compute_diffusion_intensity(texture, l->get_intensity(), N,L);

        // Compute the specular intensity for the source light at intersection point
        auto specular_intensity = compute_specular_intensity(texture,l->get_intensity(),ray_direction,N,L);

        // Add the intensity to the total intensity
        float final_intensity_r = new_color.get_r() + diffusion_intensity.get_r() + specular_intensity.get_r();
        float final_intensity_g = new_color.get_g() + diffusion_intensity.get_g() + specular_intensity.get_g();
        float final_intensity_b = new_color.get_b() + diffusion_intensity.get_b() + specular_intensity.get_b();

        new_color.set_r(final_intensity_r);
        new_color.set_g(final_intensity_g);
        new_color.set_b(final_intensity_b);
    }

    // Compute the ambient light intensity (takes into account the color of the object)
    float ambient_light_intensity_r = texture.ka * ambient_intensity * (texture.color.get_r()/255.f);
    float ambient_light_intensity_g = texture.ka * ambient_intensity * (texture.color.get_g()/255.f);
    float ambient_light_intensity_b = texture.ka * ambient_intensity * (texture.color.get_b()/255.f);

    // Compute the reflection intensity at intersection point (recursive)
    color::RGB_f reflection_intensity{0.f,0.f,0.f};
    if (n>0) {
        geometry::Vector4 reflected_vector = compute_reflected_ray_direction(ray_direction, N);

        // Compute the closest object
        std::optional<geometry::Point4> closest_reflected_object_coord{std::nullopt};
        // We do + (N * epsilon) to avoid the intersection point to be itself
        object::Object* closest_reflected_object = compute_closest_object(closest_object_coord + (N * epsilon), reflected_vector, objects, closest_reflected_object_coord);

        // Verify if the ray intersects an object
        if (closest_reflected_object != nullptr) {
            reflection_intensity = compute_local_illumination(lights, closest_reflected_object, closest_reflected_object_coord.value(),reflected_vector,ambient_intensity,objects,n-1);
        }
    }
    // Add the intensity to the total intensity
    float final_intensity_r = new_color.get_r() + ambient_light_intensity_r + (reflection_intensity.get_r() * texture.ks);
    float final_intensity_g = new_color.get_g() + ambient_light_intensity_g + (reflection_intensity.get_g() * texture.ks);
    float final_intensity_b = new_color.get_b() + ambient_light_intensity_b + (reflection_intensity.get_b() * texture.ks);

    // Clamp values of the final intensity between 0 and 1
    new_color.set_r(std::min(std::max(0.f,final_intensity_r),1.f));
    new_color.set_g(std::min(std::max(0.f,final_intensity_g),1.f));
    new_color.set_b(std::min(std::max(0.f,final_intensity_b),1.f));

    return new_color;
}

void generate_image(Scene& scene, Image& image) {
    std::vector<color::RGB>& pixels = image.get_pixels();
    const std::vector<std::shared_ptr<object::Object>>& objects = scene.get_objects();
    const Camera& camera = scene.get_camera();
    const std::vector<std::shared_ptr<light::Light>>& lights = scene.get_lights();
    float ambient_intensity = scene.get_ambient_intensity();
    // Calculating the camera's coordinate system
    geometry::Vector4 W{};
    geometry::Vector4 U{};
    geometry::Vector4 V{};
    compute_camera_coordinate_system(camera, W, U, V);

    // Image plane dimensions (alpha, beta in radians)
    float plan_width = 2.0f * camera.get_z_min() * std::tan(camera.get_alpha() / 2.0f);
    float plan_height = 2.0f * camera.get_z_min() * std::tan(camera.get_beta() / 2.0f);

    // Compute the upper left point of the physical image
    geometry::Vector4 left_upper_corner = W*(camera.get_z_min()) - U*(plan_width/2) + V*(plan_height/2);

    // Create vector of threads
    std::vector<std::future<void>> futures;

    int number_of_ray_per_pixel = 1;
    // Iterate over each pixel
    for (int y = 0; y < image.get_height(); y++) {
        // Create a new thread for each row of the image
        futures.push_back(std::async(std::launch::async, [&, y](){
            // Necessary to generate random number
            std::random_device rd;
            std::mt19937 gen(rd());
            
            // Uniform distribution between 0 included and 1 excluded
            std::uniform_real_distribution<float> dist(0.0, 1.0);
            
            for (int x = 0; x < image.get_width(); x++) {
                float sum_r = 0;
                float sum_g = 0;
                float sum_b = 0;
                for (int i = 0; i < number_of_ray_per_pixel ;i++) {
                    // Random offset for x and y
                    float random_offset_x = dist(gen);
                    float random_offset_y = dist(gen);
                    // Calculation of dx and dy offsets
                    float dx = ((x + random_offset_x) / static_cast<float>(image.get_width())) * plan_width;
                    float dy = ((y + random_offset_y) / static_cast<float>(image.get_height())) * plan_height;

                    // Calculating the direction vector of the ray
                    geometry::Vector4 ray_direction = left_upper_corner + U * dx - V * dy;
                    ray_direction.normalize();

                    // Compute the closest object
                    std::optional<geometry::Point4> closest_object_coord{std::nullopt};
                    object::Object* closest_object = compute_closest_object(camera.get_center(), ray_direction, objects, closest_object_coord);

                    // Verify if the ray intersects an object
                    if (closest_object != nullptr) {
                        color::RGB_f intensity = compute_local_illumination(lights, closest_object, closest_object_coord.value(),ray_direction,ambient_intensity,objects,6);
                        // Add the color to the sum of all colors
                        sum_r += intensity.get_r() * 255.f;
                        sum_g += intensity.get_g() * 255.f;
                        sum_b += intensity.get_b() * 255.f;
                    }
                }
                // The value of the pixel is the mean value of all colors
                pixels[y * image.get_width() + x] = color::RGB{static_cast<uint8_t>(std::round(sum_r / number_of_ray_per_pixel)),static_cast<uint8_t>(std::round(sum_g / number_of_ray_per_pixel)),static_cast<uint8_t>(std::round(sum_b / number_of_ray_per_pixel))};
            }
        }));
        
    }
    for (auto& f : futures){
        f.get();
    }
}


int main() {
    // SIMULATION OF THE CLOTH FALLING ON A SPHERE
    // Texture and Light
    auto texture3 = texture::Uniform_Texture{1, 0.5, color::RGB{0, 0, 255}, 1, 0.3};
    std::shared_ptr<light::Light> point_light = std::make_shared<light::Point_Light>(1, geometry::Point4{2, 3, 5, 1}); 
    std::vector<std::shared_ptr<light::Light>> lights{point_light};

    // Camera
    auto center_camera = geometry::Point4{4.0f, 3.0f, 4.0f, 1.0f};
    auto p = geometry::Point4{0.0f, 0.0f, 0.0f, 1.0f};
    auto up = geometry::Vector4{0, 1, 0};
    float alpha = 60.0f;
    float beta = 60.0f;
    float z_min = 1.0f;

    // Sphere Parameter
    auto center = geometry::Point4{0.0f, 0.0f, 0.0f, 1.0f};
    auto texture = texture::Uniform_Texture{1,0.5,color::RGB{255,0,0}, 1,0.3};
    std::shared_ptr<object::Sphere> sphere = std::make_shared<object::Sphere>(texture,center,1);

    // Cloth Paramater
    int grid_size = 60;
    float spacing = 0.06f; // 0.15f
    
    float startX = -((grid_size - 1) * spacing) / 2.0f; 
    float startY = 1.2f;//-((grid_size - 1) * spacing);
    float startZ = -((grid_size - 1) * spacing) / 2.0f;

    // Disable the wind
    bool wind = false;
    float total_time = 0.f;

    cloth::Cloth cloth{grid_size, grid_size, spacing, startX, startY, startZ, 1.0f, std::vector<std::shared_ptr<object::Sphere>>{sphere}, true};

    std::cout << "Begin of the simulation." << std::endl;
    
    int num_frames = 1000;
    // Time interval between two generated images
    float time_between_image = 0.033f; // 30 FPS
    // Time difference between the current frame and the previous one
    float delta_time = 1.f / 600.f;
    int nb_steps = time_between_image / delta_time;
    // Controls how quickly the simulation loses energy
    float damping_global = 0.98f;
    float damping_step = std::pow(damping_global, 1.f / static_cast<float>(nb_steps));

    std::cout << "Number of steps: " << nb_steps << std::endl;
    std::cout << "damping_step: " << damping_step << std::endl;

    for (int i = 0; i < num_frames; i++) {
        auto triangles = cloth.to_triangle(texture3);
        
        std::vector<std::shared_ptr<object::Object>> objects{sphere};
        for (auto& t : triangles) {
            objects.push_back(t);
        }
        
        Image image{400, 400}; 
        Camera camera{center_camera, p, up, alpha, beta, z_min};
        
        auto scene = Scene{objects, lights, camera, 0.2f}; 
        
        std::cout << "Generation of image " << i + 1 << "/" << num_frames << "." << std::endl;
        generate_image(scene, image);
        
        std::string file_name = std::format("test_{:03}.ppm", i);
        image.save(file_name);
        
        // Update multiple times so it moves faster between generated images
        for (int step = 0; step < nb_steps; step++) {
            cloth.update(delta_time,damping_step, total_time, wind);
            total_time += delta_time;
        }
    }
    std::cout << "Finished !" << std::endl;
    
    // SIMULATION OF THE CLOTH ACTING LIKE A FLAG WITH WIND
    /*
    // Texture and Light
    auto texture3 = texture::Uniform_Texture{1, 0.5, color::RGB{0, 0, 255}, 1, 0.3};
    //light::Point_Light point_light{1, geometry::Point4{2, 3, 5, 1}};
    //std::vector<light::Light *> lights{&point_light};
    std::shared_ptr<light::Light> point_light = std::make_shared<light::Point_Light>(1, geometry::Point4{10, 0, 5, 1});
    std::vector<std::shared_ptr<light::Light>> lights{point_light};

    // Camera
    auto center_camera = geometry::Point4{10, -2, 5, 1};
    auto p = geometry::Point4{0, -3, 0, 1};
    auto up = geometry::Vector4{0, 1, 0};
    float alpha = 80.0f;
    float beta = 80.0f;
    float z_min = 1.0f;

    // Cloth Paramater
    int grid_size = 60;
    float spacing = 0.15f;

    float startX = -((grid_size - 1) * spacing) / 2.0f;
    float startY = -((grid_size - 1) * spacing);
    float startZ = 0.0f;

    cloth::Cloth cloth{grid_size, grid_size, spacing, startX, startY, startZ, 1.0f, std::vector<std::shared_ptr<object::Sphere>>{}, false};

    std::cout << "Begin of the simulation." << std::endl;

    int num_frames = 60;
    float time_between_image = 0.033f;
    float delta_time = 1.f / 600.f;
    int nb_steps = time_between_image / delta_time;
    // Controls how quickly the simulation loses energy
    float damping_global = 0.98f;
    float damping_step = std::pow(damping_global, 1.f / static_cast<float>(nb_steps));

    float total_time = 0.f;

    bool wind = true;

    for (int i = 0; i < num_frames; i++) {
        auto triangles = cloth.to_triangle(texture3);

        std::vector<std::shared_ptr<object::Object>> objects{};
        for (auto& t : triangles) {
            objects.push_back(t);
        }

        Image image{400, 400};
        Camera camera{center_camera, p, up, alpha, beta, z_min};

        auto scene = Scene{objects, lights, camera, 0.2f};

        std::cout << "Generation of image " << i + 1 << "/" << num_frames << "." << std::endl;
        generate_image(scene, image);

        std::string file_name = "test_" + (i < 10 ? std::string("0") : std::string("")) + std::to_string(i) + ".ppm";
        image.save(file_name);

        // Update multiple times so it moves faster between generated images
        for (int step = 0; step < 30; step++) {
            cloth.update(delta_time,damping_step, total_time, wind);
            total_time += delta_time;
        }
    }
    std::cout << "Finished !" << std::endl;
    */
}