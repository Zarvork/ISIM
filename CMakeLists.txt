cmake_minimum_required(VERSION 3.21)
project(Cloth_Simulation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(.)
#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

add_executable(cloth_simulation
        projective_geometry/Point4.cpp
        projective_geometry/Point4.hh
        projective_geometry/Vector4.cpp
        projective_geometry/Vector4.hh
        Image.cpp
        Image.hh
        engine.cpp
        texture/Texture_Material.hh
        texture/Uniform_Texture.cpp
        texture/Uniform_Texture.hh
        object/Object.hh
        object/Sphere.cpp
        object/Sphere.hh
        color/RGB.cpp
        color/RGB.hh
        object/Object.cpp
        light/Light.hh
        light/Point_Light.cpp
        light/Point_Light.hh
        Camera.cpp
        Camera.hh
        Scene.cpp
        Scene.hh
        color/RGB_f.cpp
        color/RGB_f.hh
        object/Triangle.cpp
        object/Triangle.hh
        cloth/Particle.cpp
        cloth/Particle.hh
        cloth/Stick.cpp
        cloth/Stick.hh
        cloth/Cloth.cpp
        cloth/Cloth.hh
)
