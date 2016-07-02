#version 330 core

uniform mat4 projMatrix;
uniform mat4 modelMatrix;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 texCoords;
layout(location = 2) in vec3 normals;
layout(location = 3) in vec3 tangents;
layout(location = 4) in vec3 bitangents;

void main() {
    gl_Position = projMatrix * modelMatrix * vec4(position, 1);
}
