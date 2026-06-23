#version 330 core

in vec3 position;

out vec4 FragColour;

void main() {
    float dist = abs(position.y);
    float scale = 6.5;
    float t = clamp(dist * scale, 0.0, 1);
    t = pow(t, 0.75);

    vec3 white = vec3(1.0);
    vec3 red = vec3(1.0, 0.0, 0.0);
    vec3 colour = mix(white, red, t);

    FragColour = vec4(colour, 1);
}