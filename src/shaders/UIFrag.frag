#version 330 core

uniform bool playing;
uniform bool looping;
uniform bool currentButtonPlay;

out vec4 FragColour;

void main() {
    vec3 colour;

    if (currentButtonPlay) {
        colour = vec3(0.2, 0.2, 0.5);
    } else {
        colour = vec3(0.5, 0.2, 0.2);
        if (looping) {
            colour *= 2;
        }
    }

    if (playing) {
        colour *= 2;
    }

    FragColour = vec4(colour, 1.0);
}