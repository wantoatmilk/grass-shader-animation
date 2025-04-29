#version 120 //nötig für GLUT (veraltet)

attribute vec3 aPos;   // Vertex Position, Eingabeposition

uniform mat4 model; //model: lokale Transformation (z. B. Halm drehen, skalieren)
uniform mat4 view; //view: Kamera-Ansicht (wo die Kamera steht und hinschaut)
uniform mat4 projection; //projection: Perspektive (wie du den 3D-Raum auf 2D abbildest)
uniform float time; //zeit fürs animieren

void main()
{
    // Je höher das Vertex ist (je größer y), desto mehr verschieben wir in x-Richtung
    float bend = 0.1 * sin(2.0 * time + aPos.y * 5.0);

    vec4 modifiedPosition = vec4(aPos.x + bend * aPos.y, aPos.y, aPos.z, 1.0);

    gl_Position = projection * view * model * modifiedPosition;
}