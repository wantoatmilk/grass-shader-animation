#include "Plane.h"
#include "Cam.h"
#include <GLUT/glut.h>
#include "ShaderLoader.h"
#include "PerlinNoise.hpp"

#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern Cam cam;

siv::PerlinNoise perlin(1234); // Seed beliebig änderbar

// für hügel
float terrainNoise(float x, float z)
{
    float scale = 0.2f; // kleinere Werte = größere Hügel
    return perlin.noise2D_01(x * scale, z * scale) * 2.0f - 1.0f;
}

Plane::Plane()
{
    vbo = 0;
    ibo = 0;
    shaderProgram = 0;
    aPosLocation = -1;
    mitte = 5.0f;
}

Plane::~Plane()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteProgram(shaderProgram);
}

void Plane::setup()
{
    setupPlane();
    setupShader();
}

float Plane::getSize() const
{
    return mitte * 2.0f;
}

void Plane::setupPlane()
{

    std::vector<float> vertices;
    int resolution = 50; // Anzahl Teilstücke pro Achse

    for (int i = 0; i <= resolution; ++i)
    {
        for (int j = 0; j <= resolution; ++j)
        {
            float x = -mitte + 2 * mitte * (float)i / resolution;
            float z = -mitte + 2 * mitte * (float)j / resolution;
            float y = terrainNoise(x, z); // HÖHE aus Noise

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    std::vector<unsigned int> indices;
    for (int i = 0; i < resolution; ++i)
    {
        for (int j = 0; j < resolution; ++j)
        {
            int row1 = i * (resolution + 1);
            int row2 = (i + 1) * (resolution + 1);

            indices.push_back(row1 + j);
            indices.push_back(row2 + j);
            indices.push_back(row1 + j + 1);

            indices.push_back(row2 + j);
            indices.push_back(row2 + j + 1);
            indices.push_back(row1 + j + 1);
        }
    }

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    vertexCount = vertices.size() / 3;
    indexCount = indices.size();

    /*
        float vertices[] = {
            -mitte, 0.0f, -mitte,
            mitte, 0.0f, -mitte,
            mitte, 0.0f, mitte,
            -mitte, 0.0f, mitte};

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        */
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

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(aPosLocation);

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(aPosLocation);
    glUseProgram(0);
}