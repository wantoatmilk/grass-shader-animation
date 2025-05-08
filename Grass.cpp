#define GL_SILENCE_DEPRECATION
#include "Grass.h"
#include "Cam.h"
#include "Plane.h"
#include "ShaderLoader.h"
#include <GLUT/glut.h>

#include <iostream>
#include "PerlinNoise.hpp"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

extern Cam cam;
extern Plane plane; // zugriff auf den "boden"

Grass::Grass() : vbo(0), shaderProgram(0), aPosLocation(-1) {}

void Grass::setup()
{
    float width = 0.05f;
    float height = 1.0f;
    int segments = 5;
    float segmentHeight = height / segments;
    float bendFactor = 0.02f;

    std::vector<float> vertices;

    for (int i = 0; i < segments; ++i)
    {
        float y0 = i * segmentHeight;
        float y1 = (i + 1) * segmentHeight;

        float w0 = width * (1.0f - (float)i / segments);
        float w1 = width * (1.0f - (float)(i + 1) / segments);

        float bend0 = bendFactor * i;
        float bend1 = bendFactor * (i + 1);

        float xLeft0 = -w0 + bend0;
        float xRight0 = w0 + bend0;
        float xLeft1 = -w1 + bend1;
        float xRight1 = w1 + bend1;

        // Erstes Dreieck
        vertices.insert(vertices.end(), {xLeft0, y0, 0.0f, xRight0, y0, 0.0f, xLeft1, y1, 0.0f});
        // Zweites Dreieck
        vertices.insert(vertices.end(), {xLeft1, y1, 0.0f, xRight0, y0, 0.0f, xRight1, y1, 0.0f});
    }

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    shaderProgram = LoadShaders("../shader/grass.vert", "../shader/grass.frag");
    aPosLocation = glGetAttribLocation(shaderProgram, "aPos");
    vertexCount = vertices.size() / 3;

    float planeSize = plane.getSize();
    int numHalme = 40;
    float spacing = planeSize / numHalme;

    for (int i = 0; i < numHalme; ++i)
    {
        for (int j = 0; j < numHalme; ++j)
        {
            float x = -planeSize / 2 + i * spacing;
            float z = -planeSize / 2 + j * spacing;
            float y = plane.getHeightAt(x, z);

            GrassInstance instance;
            instance.position = glm::vec3(x, y, z);
            instance.scaleHeight = 0.8f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.2f - 0.8f)));
            instance.scaleWidth = 0.8f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.2f - 0.8f)));
            instance.rotationAngle = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX)) * glm::radians(360.0f);

            instances.push_back(instance);
        }
    }
}

void Grass::draw() const
{
    glUseProgram(shaderProgram);

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = cam.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(cam.getFov()), 800.0f / 600.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glEnableVertexAttribArray(aPosLocation);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

    for (const GrassInstance &instance : instances)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, instance.position);
        model = glm::rotate(model, instance.rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(instance.scaleWidth, instance.scaleHeight, instance.scaleWidth));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

    // für die schwingungen
    GLuint timeLoc = glGetUniformLocation(shaderProgram, "time");
    float t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // Zeit in Sekunden
    glUniform1f(timeLoc, t);

    glDisableVertexAttribArray(aPosLocation);

    glUseProgram(0);
}