#ifndef PLANE_H
#define PLANE_H
#include <GLUT/glut.h>

class Plane
{
public:
    Plane();
    ~Plane();

    void draw() const;
    void setup();

private:
    GLuint vbo;
    GLuint shaderProgram;
    GLint aPosLocation;

    void setupPlane();
    void setupShader();
};

#endif