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
    float getSize() const;
    float getHeightAt(float x, float z) const;

private:
    GLuint vbo;
    GLuint ibo; // Index Buffer Object
    GLuint shaderProgram;
    GLint aPosLocation;
    GLuint vboNormals;
    GLint aNormalLocation;

    void setupPlane();
    void setupShader();
    float mitte;
    int vertexCount;
    int indexCount; // Anzahl der Indices
    bool usePerlin;
};

#endif