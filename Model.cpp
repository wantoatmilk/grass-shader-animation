#define GL_SILENCE_DEPRECATION
#include "ModelLoader.h"
#include "Model.h"
#include "Cam.h"
#include <GLUT/glut.h>
#include "ShaderLoader.h"

#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <limits>

extern Cam cam;

Model::Model()
{
    vbo = 0;
    ibo = 0;
    shaderProgram = 0;
    aPosLocation = -1;
}

Model::~Model()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteProgram(shaderProgram);
}

void Model::setup()
{
    loader.loadModel("../bunny.obj");
    setupModel();
    setupShader();
}

void Model::setupModel()
{
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, loader.vertices.size() * sizeof(glm::vec3), loader.vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &vboNormals);
    glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
    glBufferData(GL_ARRAY_BUFFER, loader.normals.size() * sizeof(glm::vec3), loader.normals.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, loader.indices.size() * sizeof(unsigned int), loader.indices.data(), GL_STATIC_DRAW);

    vertexCount = loader.vertices.size() / 3;
    indexCount = loader.indices.size();
}

void Model::setupShader()
{
    shaderProgram = LoadShaders("../shader/model.vert", "../shader/model.frag");
    aPosLocation = glGetAttribLocation(shaderProgram, "aPos");
    aNormalLocation = glGetAttribLocation(shaderProgram, "aNormal");
}

void Model::draw() const
{
    glUseProgram(shaderProgram);

    // Framebuffer + Z-Buffer leeren (vor dem Zeichnen)
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Uniform Locations holen
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    GLuint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");

    // Beispielwerte (kannst du später dynamisch setzen)
    glm::vec3 lightPos(3.0f, 3.0f, 3.0f);
    glm::vec3 viewPos(0.0f, 0.0f, 5.0f); // Kamera von vorne

    glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(viewPosLoc, viewPos.x, viewPos.y, viewPos.z);

    // Model-Matrix wo bist duuuuuu :(
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, -1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(24.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

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
    glUseProgram(0);
}

glm::vec3 Model::getSize() const
{
    glm::vec3 min = loader.vertices[0];
    glm::vec3 max = loader.vertices[0];

    for (const auto &v : loader.vertices)
    {
        min = glm::min(min, v);
        max = glm::max(max, v);
    }

    return max - min;
}

float Model::getHeightAt(const glm::vec3 &position, float radius) const
{
    // Transformation rückgängig machen (Welt → Modellraum)
    glm::vec3 localPos = (position - glm::vec3(0.5f, -1.0f, 0.0f)) / 14.0f;

    float maxY = -std::numeric_limits<float>::infinity();

    for (const auto &v : loader.vertices)
    {
        float dxz = glm::distance(glm::vec2(v.x, v.z), glm::vec2(localPos.x, localPos.z));
        if (dxz < radius && v.y > maxY)
        {
            maxY = v.y;
        }
    }

    return (maxY > -9999.0f) ? maxY * 14.0f - 14.0f + 1.0f : position.y; // skaliert + rückverschoben in Welt
}

std::vector<glm::vec3> Model::getVertices() const
{
    return loader.vertices;
}

std::vector<glm::vec3> Model::getNormals() const
{
    return loader.normals;
}

glm::vec3 Model::getCenter() const
{
    glm::vec3 min = loader.vertices[0];
    glm::vec3 max = loader.vertices[0];

    for (const auto &v : loader.vertices)
    {
        min = glm::min(min, v);
        max = glm::max(max, v);
    }

    return (min + max) * 0.5f;
}