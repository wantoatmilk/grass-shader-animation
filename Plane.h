#ifndef PLANE_H
#define PLANE_H

class Plane {
public:
    Plane(float size = 2.0f);
    void draw() const;

private:
    float mitte;
};

#endif