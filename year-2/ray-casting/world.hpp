#pragma once

class World;

#include "sphere.hpp"
#include "dynamic_array.hpp"
#include "light.hpp"
#include "ray.hpp"
#include "camera.hpp"

class World {
    dynamic_array<Sphere*> spheres;
    dynamic_array<LightPoint*> light_points;
    Vec3d ambient_light = {0.19, 0.19, 0.19};

public:
    World(): spheres() {}

    void add_sphere(Sphere* sphere) { spheres.push(sphere); }
    void add_light_point(LightPoint* point) { light_points.push(point); }

    void trace_ray(Ray* ray, Camera* camera) const;

    Vec3d calculate_ray_color(Ray* ray, Vec3d intersection_vector, Sphere* sphere, Camera* camera) const;
};