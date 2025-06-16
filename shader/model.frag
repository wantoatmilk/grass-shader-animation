#version 120

varying vec3 Normal;
varying vec3 FragPos;

uniform vec3 lightPos;   // Position der Lichtquelle
uniform vec3 viewPos;    // Kamera-Position

void main()
{
    // Normale normalisieren
    vec3 norm = normalize(Normal);

    // Lichtvektor berechnen
    vec3 lightDir = normalize(lightPos - FragPos);

    // Ambient-Beleuchtung (Grundhelligkeit)
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(1.0); // weißes Licht

    // Diffuse Beleuchtung (Lambert)
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0);

    // Spekulare Beleuchtung (Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0); // Glanzpunkt
    float specularStrength = 0.5;
    vec3 specular = specularStrength * spec * vec3(1.0);

    // Materialfarbe
    vec3 objectColor = vec3(0.6, 0.6, 0.6); // Grau wie vorher

    // Endfarbe
    vec3 result = (ambient + diffuse + specular) * objectColor;

    gl_FragColor = vec4(result, 1.0);
    //gl_FragColor = vec4(1.0); //für nur weiß
}

/*
#version 120

void main()
{
    gl_FragColor = vec4(0.6, 0.6, 0.6, 1.0);
}
*/