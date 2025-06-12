#version 120

uniform float instanceColorSeed;

varying float vHeight;

//hier soll später ein gradient von unten dunkel zu oben heller hin

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
    gl_FragColor = vec4(finalColor, 1.0);
}