#version 120

uniform float instanceColorSeed;

varying float vHeight;

varying vec3 Normal;
varying vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    float greenVariation = 0.5 + 0.35 * fract(sin(instanceColorSeed * 91.17) * 43758.5453);
    float redVariation = 0.1 + 0.1 * fract(sin(instanceColorSeed * 23.42) * 12345.6789);
    float blueVariation = 0.1 + 0.1 * fract(sin(instanceColorSeed * 56.78) * 98765.4321);
    
    float t = clamp((vHeight - 0.6) / 0.4, 0.0, 1.0); //gradienten höhe
    /*
    •	vHeight - 0.6 → verschiebt den Startpunkt des Übergangs (erst ab 0.6 beginnt es gelb zu werden)
	•	/ 0.4 → bestimmt, wie schnell der Übergang erfolgt (von 0.6 bis 1.0)
	•	clamp(..., 0.0, 1.0) → begrenzt den Wert auf den Bereich [0,1]
    */
    
    vec3 bottomColor = vec3(redVariation, greenVariation, blueVariation); // unten: grünlich
    vec3 topColor = vec3(0.65, 0.7, 0.2); // oben: gelblich
    
    vec3 finalColor = mix(bottomColor, topColor, t); // mischen basierend auf Höhe

    //lichtshader
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(1.0);

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0);

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    float specularStrength = 0.2;
    vec3 specular = specularStrength * spec * vec3(1.0);

    // Endfarbe aus Licht × Farbe
    vec3 litColor = (ambient + diffuse + specular) * finalColor;
    gl_FragColor = vec4(litColor, 1.0);

    //gl_FragColor = vec4(finalColor, 1.0);
}