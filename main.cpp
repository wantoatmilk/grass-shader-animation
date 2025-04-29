#define GL_SILENCE_DEPRECATION // deprication meldungen ausblenden

#include <iostream>
#include "Plane.h"
#include "Cam.h"
#include "Grass.h"
#include <GLUT/glut.h>

Plane plane;
Cam cam;

void keyboard(unsigned char key, int x, int y); // init der tastatur

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    // cam.applyView();  <-- NICHT mehr hier mit gluLookAt arbeiten!

    plane.draw(); // Plane zieht sich View/Projection-Matrix

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(cam.getFov(), (double)w / h, 0.1, 100.0); // Perspektivische Projektion

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Gras Shader");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f); // hintergrund

    plane.setup();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
}

void keyboard(unsigned char key, int x, int y)
{
    float step = 0.2f;

    switch (key)
    {
    case 'w':
        cam.moveForward(step);
        break;
    case 's':
        cam.moveBackward(step);
        break;
    case 'a':
        cam.moveLeft(step);
        break;
    case 'd':
        cam.moveRight(step);
        break;
    case 'q':
        cam.orbitLeft(0.1f);
        break;
    case 'e':
        cam.orbitRight(0.1f);
        break;
    case '+':
    case '=':
        cam.zoomIn(2.0f);
        break; // wegen tastaturlayout +
    case '-':
    case '_':
        cam.zoomOut(2.0f);
        break; // wegen tastaturlayout -
    case 27:
        exit(0); // ESC zum Beenden
    }

    glutPostRedisplay(); // Szene neu zeichnen

    // Projektion neu laden, wichtig da die fov verändert wurde
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(cam.getFov(), 800.0 / 600.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}