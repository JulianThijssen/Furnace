#version 150

uniform vec3 color;

in vec3 pass_position;

out vec4 frag_Color;

void main() {
    frag_Color = vec4(color, 1);
}
