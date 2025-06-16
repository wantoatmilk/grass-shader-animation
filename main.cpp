#define GL_SILENCE_DEPRECATION // deprication meldungen ausblenden
#include <GL/glew.h>
#include <GLUT/glut.h>

#include <iostream>
#include "Plane.h"
#include "Cam.h"
#include "Grass.h"

#include "Model.h"
#include "Hair.h"

Model bunny;
Hair hair;
Plane plane;
Grass grass;
Cam cam;

// framerate zaehler
int frameCount = 0;
float lastTime = 0.0f;

void keyboard(unsigned char key, int x, int y); // init der tastatur

void display()
{
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // Sekunden
    frameCount++;

    if (currentTime - lastTime >= 1.0f) // alle 1 Sekunde
    {
        // std::cout << "FPS: " << frameCount << std::endl;
        std::string title = "Grass Shader - FPS: " + std::to_string(frameCount);
        glutSetWindowTitle(title.c_str());
        frameCount = 0;
        lastTime = currentTime;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    plane.draw();
    grass.draw();

    bunny.draw();
    // hair.draw();

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
    glutCreateWindow("Grass Shader - FPS: ");

    std::cout << "OpenGL-Version: " << glGetString(GL_VERSION) << std::endl;

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "[GLEW Fehler] " << glewGetErrorString(err) << std::endl;
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f); // hintergrund

    plane.setup();
    grass.setup();

    // bunny laden
    bunny.setup();
    // hair.setup(); //lol doesnt work

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(display); // wenn nix anderes zu tun, display neu laden!

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