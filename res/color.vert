#version 150

uniform mat4 projMatrix;
uniform mat4 modelMatrix;

in vec3 position;

void main() {
    gl_Position = projMatrix * modelMatrix * vec4(position, 1);
}
