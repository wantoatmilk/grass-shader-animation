#ifndef MODEL_H
#define MODEL_H
#include <GLUT/glut.h>
#include <iostream>
#include <glm/glm.hpp>
#include "ModelLoader.h"

class Model
{
public:
    Model();
    ~Model();

    void draw() const;
    void setup();

private:
    GLuint vbo;
    GLuint ibo; // Index Buffer Object
    GLuint shaderProgram;
    GLint aPosLocation;
    int vertexCount;
    int indexCount;
    glm::vec3 position;

    ModelLoader loader;

    void setupModel();
    void setupShader();
};

#endif