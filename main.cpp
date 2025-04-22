#include <GLUT/glut.h> // Für macOS

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    gluLookAt(0.0, 2.0, 5.0,   // Kamera-Position
              0.0, 0.0, 0.0,   // Blickpunkt
              0.0, 1.0, 0.0);  // Up-Vektor

    //2x2-Plane
    glBegin(GL_QUADS); 
        glColor3f(0.2f, 0.8f, 0.2f); // farbe, hier grün

        //zeichnungs start, (0, 0, 0) ist die plane mitte
        //       (-1,0,-1)------(1,0,-1)
        //           |             |
        //           |    Plane     |
        //           |             |
        //       (-1,0,1)-------(1,0,1)

        glVertex3f(-1.0f, 0.0f, -1.0f);
        glVertex3f( 1.0f, 0.0f, -1.0f);
        glVertex3f( 1.0f, 0.0f,  1.0f);
        glVertex3f(-1.0f, 0.0f,  1.0f);
    glEnd();

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
    glutMainLoop();
    return 0;
}