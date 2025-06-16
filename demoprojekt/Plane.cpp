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

float flatTerrain(float x, float z)
{
    return 0.0f;
}

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
    vboNormals = 0;
    shaderProgram = 0;
    aPosLocation = -1;
    aNormalLocation = -1;
    mitte = 5.0f;

    // HIER PERLIN!!
    usePerlin = false;
}

Plane::~Plane()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &vboNormals);
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

float Plane::getHeightAt(float x, float z) const
{
    return terrainNoise(x, z);
}

void Plane::setupPlane()
{

    std::vector<float> vertices;
    std::vector<float> normals;
    int resolution = 50; // Anzahl Teilstücke pro Achse

    for (int i = 0; i <= resolution; ++i)
    {
        for (int j = 0; j <= resolution; ++j)
        {
            float x = -mitte + 2 * mitte * (float)i / resolution;
            float z = -mitte + 2 * mitte * (float)j / resolution;
            float y = usePerlin ? terrainNoise(x, z) : flatTerrain(x, z); // HÖHE aus Noise oder flach

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            glm::vec3 p = glm::vec3(x, y, z);
            float eps = 0.1f;
            glm::vec3 px = glm::vec3(x + eps, usePerlin ? terrainNoise(x + eps, z) : flatTerrain(x + eps, z), z);
            glm::vec3 pz = glm::vec3(x, usePerlin ? terrainNoise(x, z + eps) : flatTerrain(x, z + eps), z + eps);
            glm::vec3 dx = px - p;
            glm::vec3 dz = pz - p;
            glm::vec3 normal = glm::normalize(glm::cross(dx, dz));

            normals.push_back(normal.x);
            normals.push_back(normal.y);
            normals.push_back(normal.z);
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

    glGenBuffers(1, &vboNormals);
    glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    vertexCount = vertices.size() / 3;
    indexCount = indices.size();
}

void Plane::setupShader()
{
    shaderProgram = LoadShaders("../shader/plane.vert", "../shader/plane.frag");
    aPosLocation = glGetAttribLocation(shaderProgram, "aPos");
    aNormalLocation = glGetAttribLocation(shaderProgram, "aNormal");
}

void Plane::draw() const
{
    glUseProgram(shaderProgram);

    // Uniform Locations holen
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    GLuint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");

    glm::vec3 lightPos(5.0f, 10.0f, 5.0f); // z.B. leicht schräg von oben
    glm::vec3 viewPos(0.0f, 5.0f, 10.0f);  // z.B. leicht schräg von vorne

    glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(viewPosLoc, viewPos.x, viewPos.y, viewPos.z);

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

    glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
    glVertexAttribPointer(aNormalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(aNormalLocation);

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(aPosLocation);
    glDisableVertexAttribArray(aNormalLocation);
    glUseProgram(0);
}