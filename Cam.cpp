#include "Cam.h"
#include <GLUT/glut.h>

Cam::Cam()
    : eyeX(0.0f), eyeY(2.0f), eyeZ(5.0f),
      centerX(0.0f), centerY(0.0f), centerZ(0.0f),
      upX(0.0f), upY(1.0f), upZ(0.0f)
{}

void Cam::applyView() const {
    gluLookAt(eyeX, eyeY, eyeZ,
              centerX, centerY, centerZ,
              upX, upY, upZ);
}