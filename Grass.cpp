#include "Grass.h"
#include <GLUT/glut.h>

Grass::Grass() 
{}

void Grass::draw() const {
    glBegin(GL_TRIANGLES);

    float width = 0.05f;     // Basisbreite
    float height = 1.0f;     // Gesamthöhe des Halms
    int segments = 5;        // Anzahl der Segmente
    float segmentHeight = height / segments;
    float bendFactor = 0.02f; // Biegung pro Segment nach rechts

    for (int i = 0; i < segments; ++i) {
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
        glColor3f(0.2f, 0.8f, 0.2f);
        glVertex3f(xLeft0, y0, 0.0f);
        glVertex3f(xRight0, y0, 0.0f);
        glVertex3f(xLeft1, y1, 0.0f);

        // Zweites Dreieck
        glVertex3f(xLeft1, y1, 0.0f);
        glVertex3f(xRight0, y0, 0.0f);
        glVertex3f(xRight1, y1, 0.0f);
    }

    glEnd();
}