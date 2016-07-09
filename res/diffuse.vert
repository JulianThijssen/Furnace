#version 150

uniform mat4 projMatrix;
uniform mat4 modelMatrix;

in vec3 position;
in vec2 texCoords;
in vec3 normals;
in vec3 tangents;
in vec3 bitangents;

out vec3 pass_position;
out vec2 pass_texCoords;
out vec3 pass_normal;

void main() {
    pass_position = position;
    pass_texCoords = texCoords;
    pass_normal = normals;
    
    gl_Position = projMatrix * modelMatrix * vec4(position, 1);
}
