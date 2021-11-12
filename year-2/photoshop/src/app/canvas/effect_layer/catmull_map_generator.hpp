
#include "catmull_rom_curve.hpp"

template<typename ArrayType>
struct CatmullRomMapGenerator {
    int resolution;
    int steps;

    void generate(ArrayType* buffer, const CatmullRomCurve& curve) {
        int old_index = -1;
        ArrayType old_value = 0;

        for(int step = 0; step < curve.get_steps(); step++) {
            for(int i = 0; i <= steps; i++) {
                if(i == steps - 1 && step != curve.get_steps() - 1) break;

                Vec2f point = curve.get_point(step, float(i) / float(steps));
                int new_index = (int)(point[0] * resolution);

                int skipped = new_index - old_index;
                ArrayType new_value = (ArrayType) point[1];

                for(int mid_index = old_index + 1, mid_counter = 1; mid_index <= new_index; mid_index++, mid_counter++) {
                    if(mid_index < 0 || mid_index >= resolution) continue;
                    float fraction = float(mid_counter) / float(skipped);
                    buffer[mid_index] = old_value * (1 - fraction) + new_value * fraction;
                }

                old_value = new_value;
                old_index = new_index;
            }
        }
    }
};