#version 150

uniform mat4 modelMatrix;

in vec3 position;

out vec3 pass_position;

void main() {
    pass_position = position;
    
    gl_Position = modelMatrix * vec4(position, 1);
}
