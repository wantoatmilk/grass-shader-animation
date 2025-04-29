#ifndef GRASS_H
#define GRASS_H
#include <GLUT/glut.h>

class Grass
{
public:
    Grass();
    void draw() const;

    void setup();

private:
    GLuint vbo;
    GLuint shaderProgram;
    GLint aPosLocation;
    int vertexCount;
};

#endif