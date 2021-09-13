//
// Created by Артем on 08.09.2021.
//

#include "world.hpp"

static inline double bin_pow(double number, int power) {
    double result = 1;
    while(power) {
        if(power & 1) {
            result *= number;
            --power;
        } else {
            number *= number;
            power >>= 1;
        }
    }
    return result;
}

void World::trace_ray(Ray* ray, Camera* camera) const {
    Sphere* intersection_sphere = nullptr;
    Vec3d intersection_position;
    double yet_min_distance = std::numeric_limits<double>::infinity();

    for(int i = 0; i < spheres.size; i++) {
        Sphere* sphere = spheres.get(i);

        Vec3d b = ray->position - sphere->center;
        double a = ray->velocity.dot(b);

        double discriminant = a * a - (b.dot(b) - sphere->radius * sphere->radius);
        if(discriminant >= 0) {
            discriminant = sqrt(discriminant);
            double distance_a = -a - discriminant;
            double distance_b = -a + discriminant;
            double min_distance = 0;
            if(distance_a > 0) min_distance = distance_a;
            if(distance_b > 0 && distance_b < min_distance) min_distance = distance_b;
            if(min_distance != 0 && min_distance < yet_min_distance) {
                intersection_position = ray->velocity;
                intersection_position *= min_distance;
                intersection_position += ray->position;

                intersection_sphere = sphere;
                yet_min_distance = min_distance;
            }
        }
    }

    if(intersection_sphere != nullptr) {
        ray->color = calculate_ray_color(ray, intersection_position, intersection_sphere, camera);
    } else {
        double sky_position = floor((ray->velocity.z + 1) / 2 * 20) / 20;

        ray->color = (Vec3d( 0.05, 0.05, 0.05) *= (1 - sky_position)) + (Vec3d( 0.5, 0.5, 0.05 ) *= (sky_position));
    }
}

Vec3d World::calculate_ray_color(Ray* ray, Vec3d intersection_vector, Sphere* sphere, Camera* camera) const {
    Vec3d normal_vector = intersection_vector - sphere->center;
    normal_vector *= 1 / sphere->radius;

    Vec3d result_color = ambient_light;
    Vec3d specular_color;

    for(int i = 0; i < light_points.size; i++) {
        LightPoint* point = light_points.get(i);
        Vec3d radius_vector = point->position - intersection_vector;
        double radius = radius_vector.length();
        double cosine = radius_vector.dot(normal_vector) / radius_vector.length();
        if(cosine > 0) {
            Vec3d color_delta = point->color * cosine;
            color_delta *= point->intensity;
            result_color += color_delta;
        }

        // Specular

        Vec3d view_vector = intersection_vector - camera->get_position();
        Vec3d reflection_vector = normal_vector;
        reflection_vector *= view_vector.dot(normal_vector) * 2;
        reflection_vector -= view_vector;
        reflection_vector *= -1;

        cosine = reflection_vector.dot(radius_vector) / reflection_vector.length() / radius;
        if(cosine > 0) {
            cosine = bin_pow(cosine, sphere->mirroring);
            Vec3d color_delta = point->color * cosine;
            color_delta *= point->intensity;
            specular_color += color_delta;
        }
    }

    result_color.multiply_components(sphere->color);

    result_color += specular_color;

    if(result_color.x > 1.0) result_color.x = 1.0;
    if(result_color.y > 1.0) result_color.y = 1.0;
    if(result_color.z > 1.0) result_color.z = 1.0;

    return result_color;
}
