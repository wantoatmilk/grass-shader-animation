#include <iostream>
#include "Plane.h"
#include "Cam.h"
#include <GLUT/glut.h>

Plane plane;

Cam cam;

void keyboard(unsigned char key, int x, int y); //init der tastatur

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    cam.applyView();

    plane.draw();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / h, 0.1, 100.0); // Perspektivische Projektion

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("gras shader");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

void keyboard(unsigned char key, int x, int y) {
    float step = 0.2f;

    switch (key) {
        case 'w': cam.moveForward(step); break;
        case 's': cam.moveBackward(step); break;
        case 'a': cam.moveLeft(step); break;
        case 'd': cam.moveRight(step); break;
        case 27: exit(0); // ESC zum Beenden
    }

    glutPostRedisplay(); // Szene neu zeichnen
}