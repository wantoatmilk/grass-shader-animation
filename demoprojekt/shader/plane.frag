#version 120

varying vec3 Normal;
varying vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
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

    // Materialfarbe
    vec3 objectColor = vec3(0.6, 0.6, 0.6); //farbe

    vec3 result = (ambient + diffuse + specular) * objectColor;
    float d = dot(norm, lightDir);
    
    
    //gl_FragColor = vec4(abs(Normal), 1.0);

    //normale zeigen
    //gl_FragColor = vec4(normalize(Normal) * 0.5 + 0.5, 1.0);

    //mit licht
    //gl_FragColor = vec4(result, 1.0);

    //ohne licht
    gl_FragColor = vec4(0.6, 0.6, 0.6, 1.0);
}