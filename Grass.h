#ifndef GRASS_H
#define GRASS_H
#include <GLUT/glut.h>
#include <iostream>
#include <glm/glm.hpp>

struct GrassInstance
{
    glm::vec3 position;
    float scaleHeight;
    float scaleWidth;
    float rotationAngle;
    float colorSeed;
};

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
    std::vector<GrassInstance> instances;

    GLuint vboNormals;
    GLint aNormalLocation;
};

#endif