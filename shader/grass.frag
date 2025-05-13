#version 120

uniform float instanceColorSeed;

void main()
{
    float greenVariation = 0.6 + 0.4 * fract(sin(instanceColorSeed * 91.17) * 43758.5453);
    float redVariation = 0.1 + 0.1 * fract(sin(instanceColorSeed * 23.42) * 12345.6789);
    float blueVariation = 0.1 + 0.1 * fract(sin(instanceColorSeed * 56.78) * 98765.4321);
    gl_FragColor = vec4(redVariation, greenVariation, blueVariation, 1.0);
}