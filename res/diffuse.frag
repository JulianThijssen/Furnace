#version 150

uniform sampler2D albedo;
uniform bool hasAlbedo;

in vec3 pass_position;
in vec2 pass_texCoords;
in vec3 pass_normal;

out vec4 frag_Color;

void main() {
    vec4 color = vec4(1, 0, 1, 1);
    if (hasAlbedo) {
        color = texture(albedo, pass_texCoords);
    }

    frag_Color = vec4(color.rgb, 1);
}
