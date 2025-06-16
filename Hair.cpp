#define GL_SILENCE_DEPRECATION
#include "Hair.h"
#include "Cam.h"
#include "Model.h"
#include "ShaderLoader.h"
#include <GLUT/glut.h>

#include <iostream>
#include "PerlinNoise.hpp"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

extern Cam cam;
extern Model bunny; // zugriff auf den bunny

Hair::Hair() : vbo(0), vboNormals(0), shaderProgram(0), aPosLocation(-1), aNormalLocation(-1) {}

void Hair::setup()
{
    float width = 0.05f;
    float height = 0.4f;
    int segments = 10;
    float segmentHeight = height / segments;
    float bendFactor = 0.03f;

    std::vector<float> vertices;
    std::vector<float> normals;

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

        glm::vec3 v0(xLeft0, y0, 0.0f);
        glm::vec3 v1(xRight0, y0, 0.0f);
        glm::vec3 v2(xLeft1, y1, 0.0f);
        glm::vec3 v3(xRight1, y1, 0.0f);

        glm::vec3 n1 = glm::normalize(glm::cross(v1 - v0, v2 - v0));
        glm::vec3 n2 = glm::normalize(glm::cross(v3 - v1, v2 - v1));

        for (int j = 0; j < 3; ++j)
        {
            normals.insert(normals.end(), {n1.x, n1.y, n1.z});
        }
        for (int j = 0; j < 3; ++j)
        {
            normals.insert(normals.end(), {n2.x, n2.y, n2.z});
        }
    }

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &vboNormals);
    glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size(), normals.data(), GL_STATIC_DRAW);

    shaderProgram = LoadShaders("../shader/hair.vert", "../shader/hair.frag");
    aPosLocation = glGetAttribLocation(shaderProgram, "aPos");
    aNormalLocation = glGetAttribLocation(shaderProgram, "aNormal");
    vertexCount = vertices.size() / 3;

    // Haarplatzierung direkt an den Modell-Vertices mit Ausrichtung entlang der Normalen
    auto BunPositions = bunny.getVertices();
    auto BunNormals = bunny.getNormals();

    int hairStep = 3; // jedes 3. Vertex → weniger Haare

    for (size_t i = 0; i < BunPositions.size(); i += hairStep)
    {
        glm::vec3 pos = BunPositions[i];
        glm::vec3 normal = BunNormals[i];

        HairInstance instance;
        glm::vec3 offset = normal * 0.005f;                                        // minimale Versetzung aus der Oberfläche
        instance.position = (pos + offset) * 24.0f + glm::vec3(0.5f, -1.0f, 0.0f); // transformiert wie das Model im draw()
        instance.scaleHeight = 0.6f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.2f - 0.6f)));
        instance.scaleWidth = 0.6f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.2f - 0.6f)));
        instance.rotationAngle = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX)) * glm::radians(360.0f);
        instance.colorSeed = static_cast<float>(rand()) / RAND_MAX;

        instances.push_back(instance);
    }
}

void Hair::draw() const
{
    glUseProgram(shaderProgram);

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    GLuint scaleHeightLoc = glGetUniformLocation(shaderProgram, "scaleHeight");
    GLuint scaleWidthLoc = glGetUniformLocation(shaderProgram, "scaleWidth");

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = cam.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(cam.getFov()), 800.0f / 600.0f, 0.1f, 100.0f);

    // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    GLuint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");

    glm::vec3 lightPos(10.0f, 15.0f, 10.0f); // z.B. leicht schräg von oben
    glm::vec3 viewPos(0.0f, 5.0f, 10.0f);    // z.B. leicht schräg von vorne

    glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(viewPosLoc, viewPos.x, viewPos.y, viewPos.z);

    glEnableVertexAttribArray(aPosLocation);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

    GLuint instancePosLoc = glGetUniformLocation(shaderProgram, "instancePos");
    GLuint colorSeedLoc = glGetUniformLocation(shaderProgram, "instanceColorSeed");

    for (const HairInstance &instance : instances)
    {
        glUniform3fv(instancePosLoc, 1, glm::value_ptr(instance.position));
        glUniform1f(colorSeedLoc, instance.colorSeed);
        glUniform1f(scaleHeightLoc, instance.scaleHeight);
        glUniform1f(scaleWidthLoc, instance.scaleWidth);

        glm::vec3 hairScale(instance.scaleWidth, instance.scaleHeight, instance.scaleWidth);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, instance.position);
        model = glm::rotate(model, instance.rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, hairScale);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
        glVertexAttribPointer(aNormalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(aNormalLocation);

        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

    // für die schwingungen
    GLuint timeLoc = glGetUniformLocation(shaderProgram, "time");
    float t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // Zeit in Sekunden
    glUniform1f(timeLoc, t);

    glDisableVertexAttribArray(aNormalLocation);
    glDisableVertexAttribArray(aPosLocation);

    glUseProgram(0);
}