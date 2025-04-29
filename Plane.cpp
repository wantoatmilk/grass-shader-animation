#include "Plane.h"
#include "Cam.h"
#include <GLUT/glut.h>
#include "ShaderLoader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// extern Zugriff auf die Kamera (z.B. wenn sie global deklariert ist)
extern Cam cam;

Plane::Plane()
{
    vbo = 0;
    shaderProgram = 0;
    aPosLocation = -1;
}

Plane::~Plane()
{
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shaderProgram);
}

void Plane::setup()
{
    setupPlane();
    setupShader();
}

void Plane::setupPlane()
{
    float vertices[] = {
        -1.0f, 0.0f, -1.0f,
        1.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, 1.0f};

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void Plane::setupShader()
{
    shaderProgram = LoadShaders("../shader/plane.vert", "../shader/plane.frag");
    aPosLocation = glGetAttribLocation(shaderProgram, "aPos");
}

void Plane::draw() const
{
    glUseProgram(shaderProgram);

    // Uniform Locations holen
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    // Model-Matrix (identisch, wir verschieben Plane nicht)
    float modelMatrix[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1};

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);

    // View-Matrix von der Kamera holen
    glm::mat4 viewMatrix = cam.getViewMatrix();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Projection-Matrix erstellen (perspektivisch)
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(cam.getFov()), 800.0f / 600.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glEnableVertexAttribArray(aPosLocation);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableVertexAttribArray(aPosLocation);
    glUseProgram(0);
}