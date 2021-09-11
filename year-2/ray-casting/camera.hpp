#pragma once

class Camera;

#include <SFML/Graphics/Image.hpp>
#include "vec3.hpp"
#include "world.hpp"
#include "matrix4.hpp"

class Camera {
    Vec3d position;
    double heading;
    double pitch;
    double roll;
    double fov = 1.22;
    Matrix4<double> matrix;
    bool should_update_matrix = true;
public:

    explicit Camera(const Vec3d &position, double heading = 0, double pitch = 0, double roll = 0):
                position(position),
                heading(heading),
                pitch(pitch),
                roll(roll),
                matrix() {};

    const Vec3d &get_position() const { return position; }
    double get_heading() const { return heading; }
    double get_pitch() const { return pitch; }
    double get_roll() const { return roll; }
    double get_fov() const { return fov; }

    void set_position(const Vec3d& p_position) { position = p_position; should_update_matrix = true; }
    void set_heading(double p_heading) { heading = p_heading; should_update_matrix = true; }
    void set_pitch(double p_pitch) { pitch = p_pitch; should_update_matrix = true; }
    void set_roll(double p_roll) { roll = p_roll; should_update_matrix = true; }
    void set_fov(double p_fov) { fov = p_fov; should_update_matrix = true;
    }

    void trace_rays(World* world, uint32_t* image, int width, int height, int thread_index, int threads_total);
    static void set_pixel(const Ray& ray, uint32_t* color);
    void trace_pixel(World* world, uint32_t* image, int width, int x, int y, double ray_x, double ray_y, double ray_z);

protected:
    void update_matrix();
};
