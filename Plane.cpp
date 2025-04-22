#include "Plane.h"
#include <GLUT/glut.h>

Plane::Plane(float size) {
    mitte = size / 2.0f;    //mitte der plane wird berechnet (hier ist die plane 2 groß also wäre die mitte 1)
    //für später zur automatischen anpassung bei größenveränderungen
}

void Plane::draw() const {
    
    //2x2-Plane
    glBegin(GL_QUADS); 
        glColor3f(0.2f, 0.8f, 0.2f); // farbe, hier grün

        //farbbeispiele:
        //glColor3f(0.5f, 1.0f, 0.0f);       -> Hellgrün
        //glColor4f(1.0f, 0.0f, 0.0f, 0.5f); -> Halbtransparentes Rot

        //zeichnungs start, (0, 0, 0) ist die plane mitte insgesamt
        //       (-1,0,-1)------(1,0,-1)
        //           |             |
        //           |    Plane     |
        //           |             |
        //       (-1,0,1)-------(1,0,1)

        glVertex3f(-mitte, 0.0f, -mitte);
        glVertex3f( mitte, 0.0f, -mitte);
        glVertex3f( mitte, 0.0f,  mitte);
        glVertex3f(-mitte, 0.0f,  mitte);
        
    glEnd();
}