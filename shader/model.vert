#version 120

attribute vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//normalvektoren, für licht
attribute vec3 aNormal;
varying vec3 Normal;
//für beleuchtung:
varying vec3 FragPos;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(model) * aNormal;
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}