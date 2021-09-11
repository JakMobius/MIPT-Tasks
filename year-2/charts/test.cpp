//
// Created by Артем on 02.09.2021.
//

#include <vector>

void draw_line(float x, float y, float x2, float y2) {
    // рисует линию
}

struct Shape {
    void (*draw)(Shape* self);
};

void draw_vector(Shape* self);

struct aaaa
{
    int a;

    int init()
    {
        a = 0;
    }
};

struct Vector : Shape {
    float x, y, x2, y2;

    static Vector* create_vector(float x, float y, float x2, float y2) {
        Vector* vector = new Vector();
        vector->x = x;
        vector->y = y;
        vector->x2 = x2;
        vector->y2 = y2;
        vector->draw = &draw_vector;
        return vector;
    }
};

void draw_vector(Shape* self) {
    Vector* vector = (Vector*) self;
    draw_line(vector->x, vector->y, vector->x2, vector->y2);
}

struct Drawer {
    std::vector<Shape*> shapes;

    void draw_everything() {
        for(int i = 0; i < shapes.size(); i++) {
            Shape* current = shapes[i];

            current->draw(current);
        }
    }

    void init() {
        shapes.push_back(Vector::create_vector(1, 1, 1, 1));
        shapes.push_back(Vector::create_vector(1, 1, 1, 1));
        shapes.push_back(Vector::create_vector(1, 1, 1, 1));
        shapes.push_back(Vector::create_vector(1, 1, 1, 1));
        shapes.push_back(Vector::create_vector(1, 1, 1, 1));
        shapes.push_back(Vector::create_vector(1, 1, 1, 1));
        shapes.push_back(Vector::create_vector(1, 1, 1, 1));
    }
};