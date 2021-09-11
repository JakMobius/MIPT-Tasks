//
// Created by Артем on 08.09.2021.
//

#include "camera.hpp"

void Camera::trace_pixel(World* world, uint32_t* image, int width, int x, int y, double ray_x, double ray_y, double ray_z) {
    Vec3d ray_direction(ray_x, ray_y, ray_z);
    ray_direction *= matrix;

    Ray current_ray(position, ray_direction);

    world->trace_ray(&current_ray, this);
    set_pixel(current_ray, image + x + (y * width));
}

void Camera::trace_rays(World* world, uint32_t* image, int width, int height, int thread_index, int threads_total) {

    if(should_update_matrix) update_matrix();

    double camera_size = fmax(width, height);

    int height_to_draw = height / 2 / threads_total;

    int sx_from = 0;
    int sy_from = height_to_draw * thread_index;

    int y_from = -height / 2 + sy_from;
    int y_to = y_from + height_to_draw;
    int x_from = -width / 2;
    int x_to = 0;

    if(thread_index == threads_total - 1) y_to = 0;

    for(int sy = sy_from, y = y_from; y <= y_to; y++, sy++) {
        double ray_pitch = y * fov / camera_size;
        double ray_pitch_sin = sin(ray_pitch);
        double ray_pitch_cos = cos(ray_pitch);

        for(int sx = sx_from, x = x_from; x <= x_to; x++, sx++) {
            double ray_heading = x * fov / camera_size;
            double ray_heading_sin = sin(ray_heading);
            double ray_heading_cos = cos(ray_heading);

            double ray_x = ray_heading_cos * ray_pitch_cos;
            double ray_y = ray_heading_sin * ray_pitch_cos;
            double ray_z = ray_pitch_sin;

            trace_pixel(world, image, width, sx,             sy,              ray_x,  ray_y,  ray_z);
            trace_pixel(world, image, width, width - sx - 1, sy,              ray_x, -ray_y,  ray_z);
            trace_pixel(world, image, width, width - sx - 1, height - sy - 1, ray_x, -ray_y,  -ray_z);
            trace_pixel(world, image, width, sx,             height - sy - 1, ray_x,  ray_y,  -ray_z);
        }
    }
}

void Camera::set_pixel(const Ray &ray, uint32_t* color) {
    unsigned char r = (unsigned char)(ray.color.x * 255);
    unsigned char g = (unsigned char)(ray.color.y * 255);
    unsigned char b = (unsigned char)(ray.color.z * 255);
    unsigned char a = 255;

    *color = a << 24 | b << 16 | g << 8 | r;
}

void Camera::update_matrix() {

    double heading_sin = sin(heading);
    double heading_cos = cos(heading);

    double pitch_sin = sin(pitch);
    double pitch_cos = cos(pitch);

    Matrix4<double> pitch_matrix({
             pitch_cos, 0, pitch_sin,  0,
             0,         1, 0,          0,
             pitch_sin, 0, -pitch_cos, 0,
             0,         0,  0,         1
    });

    Matrix4<double> heading_matrix({
           heading_cos, heading_sin, 0, 0,
           -heading_sin, heading_cos,  0, 0,
           0,           0,            1, 0,
           0,           0,            0, 1
    });

    matrix = pitch_matrix * heading_matrix;
    should_update_matrix = false;
}

