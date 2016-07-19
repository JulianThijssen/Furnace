#version 150

uniform mat4 projMatrix;

in vec3 position;
in vec2 texCoord;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;
in vec3 raydir;

out vec4 pass_position;
out vec2 pass_texCoord;
out vec3 pass_normal;
out vec3 pass_tangent;
out vec3 pass_bitangent;
out vec3 pass_raydir;

void main() {
    pass_position = vec4(position, 1);
    pass_texCoord = texCoord;
    pass_normal = normal;
    pass_tangent = tangent;
    pass_bitangent = bitangent;
    pass_raydir = raydir;
    gl_Position = projMatrix * vec4(texCoord, 0, 1);
}
