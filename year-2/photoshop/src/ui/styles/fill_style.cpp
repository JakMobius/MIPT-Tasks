//
// Created by Артем on 20.10.2021.
//

#include "fill_style.hpp"

sf::Vertex UIFillStyleGradient::vertex(Vec2f vertex_position, Vec2f shape_position) const {
    Vec2f relative_position { shape_position };
    relative_position -= start_anchor.position;

    Vec2f gradient_vector = end_anchor.position;
    gradient_vector -= start_anchor.position;

    float fraction = relative_position.dot(gradient_vector) / gradient_vector.len_squared();
    if(fraction < 0) fraction = 0;
    else if(fraction > 1) fraction = 1;

    Vec4f end_color { end_anchor.color };
    end_color *= fraction;

    Vec4f start_color { start_anchor.color };
    start_color *= (1 - fraction);

    start_color += end_color;

    return {{vertex_position[0], vertex_position[1]}, start_color.to_sf_color()};
}
