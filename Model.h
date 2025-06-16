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
    glm::vec3 getSize() const;
    float getHeightAt(const glm::vec3 &position, float radius) const;

    glm::vec3 getCenter() const;
    std::vector<glm::vec3> getVertices() const;
    std::vector<glm::vec3> getNormals() const;

private:
    GLuint vbo;
    GLuint ibo; // Index Buffer Object
    GLuint shaderProgram;
    GLint aPosLocation;
    int vertexCount;
    int indexCount;
    glm::vec3 position;

    GLuint vboNormals;
    GLint aNormalLocation;

    ModelLoader loader;

    void setupModel();
    void setupShader();
};

#endif