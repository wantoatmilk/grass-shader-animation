#version 120 //nötig für GLUT (veraltet)

attribute vec3 aPos;   // Vertex Position, Eingabeposition

uniform mat4 model; //model: lokale Transformation (z. B. Halm drehen, skalieren)
uniform mat4 view; //view: Kamera-Ansicht (wo die Kamera steht und hinschaut)
uniform mat4 projection; //projection: Perspektive (wie du den 3D-Raum auf 2D abbildest)
uniform float time; //zeit fürs animieren
uniform vec3 instancePos; // globale pos für instanzen

varying float vHeight; // Höhe des Halmpunktes für Farbe im Fragment-Shader

vec2 hash(vec2 p) {
    p = vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5,183.3)));
    return -1.0 + 2.0 * fract(sin(p) * 43758.5453123);
}

float perlinNoise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(
        mix(dot(hash(i + vec2(0.0, 0.0)), f - vec2(0.0, 0.0)),
            dot(hash(i + vec2(1.0, 0.0)), f - vec2(1.0, 0.0)), u.x),
        mix(dot(hash(i + vec2(0.0, 1.0)), f - vec2(0.0, 1.0)),
            dot(hash(i + vec2(1.0, 1.0)), f - vec2(1.0, 1.0)), u.x),
        u.y);
}

void main()
{
    vec3 worldPos = instancePos + aPos;

    vHeight = aPos.y; //y pos für gradient

    // Erzeuge Perlin-basiertes Grundrauschen auf Weltposition
    vec2 windSample = vec2(worldPos.z * 0.15 + time * 0.4, worldPos.x * 0.15 + time * 0.25);
    float baseNoise = perlinNoise(windSample);

    // Böen-Wellenform: bewegt sich entlang x-Achse in Weltkoordinaten
    float gust = sin((worldPos.x * 0.8 - time * 2.0)) * 0.5 + 0.5;

    // Windbeugung proportional zur Höhe (local y) und Windböe
    float sway = 0.0;
    if (aPos.y > 0.01) {
        sway = baseNoise * gust * (aPos.y * (0.6 + aPos.y * 0.8)); //0.8 für normal, höher für starker wind
    }

    /*
    (0.4 + aPos.y * 0.6)
    Das ist der Verstärkungsfaktor:
	•	0.4 ist die Basisbewegung (auch für kleine Halme),
	•	0.6 skaliert die Bewegung abhängig von der Höhe des Punktes im Halm (aPos.y)
    */

    // Bewege x-Position abhängig von sway
    vec4 localPos = vec4(aPos, 1.0);
    vec4 modelPos = model * localPos;
    modelPos.x += sway; // Wende sway im Welt-Raum an
    gl_Position = projection * view * modelPos;
}