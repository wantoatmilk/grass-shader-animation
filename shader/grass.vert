#version 120 //nötig für GLUT (veraltet)

attribute vec3 aPos;   // Vertex Position, Eingabeposition

uniform mat4 model; //model: lokale Transformation (z. B. Halm drehen, skalieren)
uniform mat4 view; //view: Kamera-Ansicht (wo die Kamera steht und hinschaut)
uniform mat4 projection; //projection: Perspektive (wie du den 3D-Raum auf 2D abbildest)
uniform float time; //zeit fürs animieren

void main()
{
    vec3 pos = aPos; //position vom grashalm, kann auch verändert werden bsp: aPos = (-0.05, 0.0, 0.0)

    // Wind: bewege die X-Position leicht in Abhängigkeit von Y und Zeit, erstmal einfache bewegung
    pos.x += sin(time + pos.y * 5.0) * 0.05 * (1.0 - pos.y);

    gl_Position = projection * view * model * vec4(pos, 1.0);
}