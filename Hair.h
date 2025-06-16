#ifndef HAIR_H
#define HAIR_H
#include <GLUT/glut.h>
#include <iostream>
#include <glm/glm.hpp>

struct HairInstance
{
    glm::vec3 position;
    float scaleHeight;
    float scaleWidth;
    float rotationAngle;
    float colorSeed;
};

class Hair
{
public:
    Hair();
    void draw() const;

    void setup();

private:
    GLuint vbo;
    GLuint shaderProgram;
    GLint aPosLocation;
    int vertexCount;
    std::vector<HairInstance> instances;

    GLuint vboNormals;
    GLint aNormalLocation;
};

#endif