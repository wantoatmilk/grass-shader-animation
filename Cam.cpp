#include "Cam.h"
#include <GLUT/glut.h>

#include <cmath>

Cam::Cam()
    : eyeX(0.0f), eyeY(2.0f), eyeZ(5.0f),
      centerX(0.0f), centerY(0.0f), centerZ(0.0f),
      upX(0.0f), upY(1.0f), upZ(0.0f)
{}

//                y (hoch)
//                ↑
//                |
//                |
//                |      auge   ← camera eye (z.B. 0,2,5)
//                |     /
//                |    /
//                |   /
//                |  /
//                | /        Blickrichtung (zum center)
//                ●----------→ z (nach hinten)
//               /
//              /
//             x (rechts)

void Cam::applyView() const {
    gluLookAt(eyeX, eyeY, eyeZ,     //glulookat ist nur die blickrichtung, da die kamera sich nicht bewegen kann
              centerX, centerY, centerZ,
              upX, upY, upZ);
}

void Cam::moveForward(float delta) {
    float dirX = centerX - eyeX;    //x richtung
    float dirZ = centerZ - eyeZ;    //z richtung

    //länge berechnen (Norm, daher wurzel)
    float length = std::sqrt(dirX * dirX + dirZ * dirZ);

    //vorlesung für formeln!!
    dirX /= length;
    dirZ /= length;
    eyeX += dirX * delta;
    eyeZ += dirZ * delta;
    centerX += dirX * delta;
    centerZ += dirZ * delta;
}

void Cam::moveBackward(float delta) {
    moveForward(-delta);
}

void Cam::moveLeft(float delta) {
    float dirX = centerX - eyeX;
    float dirZ = centerZ - eyeZ;
    float rightX = -dirZ;
    float rightZ = dirX;
    float length = std::sqrt(rightX * rightX + rightZ * rightZ);
    rightX /= length;
    rightZ /= length;
    eyeX -= rightX * delta;
    eyeZ -= rightZ * delta;
    centerX -= rightX * delta;
    centerZ -= rightZ * delta;
}

void Cam::moveRight(float delta) {
    moveLeft(-delta);
}

//drehung

float orbitRadius = 5.0f;
float orbitAngle = 0.0f;

void Cam::orbitLeft(float delta) {
    orbitAngle -= delta;
    eyeX = centerX + orbitRadius * sin(orbitAngle);
    eyeZ = centerZ + orbitRadius * cos(orbitAngle);
}

void Cam::orbitRight(float delta) {
    orbitAngle += delta;
    eyeX = centerX + orbitRadius * sin(orbitAngle);
    eyeZ = centerZ + orbitRadius * cos(orbitAngle);
}