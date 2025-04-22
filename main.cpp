#include <GL/glew.h>
#include <GLUT/glut.h> // <- wichtig für macOS GLUT
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

// Shaderprogramm und Buffers
GLuint shaderProgram;
GLuint vao, vbo;

// Kamera-Parameter
glm::vec3 cameraPos   = glm::vec3(0.0f, 2.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -0.3f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

float speed = 0.1f;

// ==== Shader Source Code ====
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(0.2, 0.8, 0.2, 1.0); // saftiges Grasgrün
}
)";

// ==== Shader Kompilieren ====
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    return shader;
}

// ==== Shader-Programm initialisieren ====
void initShader() {
    GLuint vert = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint frag = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vert);
    glAttachShader(shaderProgram, frag);
    glLinkProgram(shaderProgram);

    glDeleteShader(vert);
    glDeleteShader(frag);
}

// ==== Grashalm vorbereiten ====
void initGeometry() {
    // Rechteck als zwei Dreiecke
    float blade[] = {
        // Erstes Dreieck
        -0.05f, 0.0f, 0.0f,
         0.0f,  0.5f, 0.0f,
         0.05f, 0.0f, 0.0f,
        // Zweites Dreieck
         0.05f, 0.0f, 0.0f,
         0.0f,  0.5f, 0.0f,
         0.0f,  0.5f, 0.05f
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(blade), blade, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
}

// ==== Tastatursteuerung ====
void keyboard(unsigned char key, int x, int y) {
    if (key == 'w') cameraPos += speed * cameraFront;
    if (key == 's') cameraPos -= speed * cameraFront;
    if (key == 'a') cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
    if (key == 'd') cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;

    std::cout << "cameraPos: " << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << std::endl;
}

// ==== Blickrichtung mit Pfeiltasten ====
void special(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT)  cameraFront.x -= 0.1f;
    if (key == GLUT_KEY_RIGHT) cameraFront.x += 0.1f;
    if (key == GLUT_KEY_UP)    cameraFront.y += 0.1f;
    if (key == GLUT_KEY_DOWN)  cameraFront.y -= 0.1f;
}

// ==== Render-Funktion ====
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgram);

    // Kamera-Setup
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

    // Mehrere Halme nebeneinander
    for (int i = -3; i <= 3; ++i) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i * 0.4f, 0.0f, 0.0f));

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &proj[0][0]);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glutSwapBuffers();
    glutPostRedisplay(); // kontinuierlich neu zeichnen
}

// ==== OpenGL Setup ====
void initGL() {
    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.4f, 0.7f, 1.0f, 1.0f); // Himmelblau

    initShader();
    initGeometry();
}

// ==== main() ====
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Gras mit Kamera");

    initGL();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);

    glutMainLoop();
    return 0;
}