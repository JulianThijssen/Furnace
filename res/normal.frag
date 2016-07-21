#version 150

in vec4 pass_position;
in vec2 pass_texCoord;
in vec3 pass_normal;
in vec3 pass_tangent;
in vec3 pass_bitangent;
in vec3 pass_raydir;

out vec4 out_Color;

uniform sampler2D triList;
uniform sampler2D vertexList;
uniform sampler2D normalList;

uniform int arrayWidth;
uniform int arrayHeight;

#define EPSILON 0.0000001

struct Ray
{
    vec3 o;
    vec3 d;
};

struct Triangle
{
    vec3 v0;
    vec3 v1;
    vec3 v2;
};

bool triangle_intersection(in Triangle tri, in Ray ray, out vec3 hit) {
    vec3 e1, e2;
    vec3 P, Q, T;
    float det, inv_det, u, v;
    float t;

    // Find vectors for two edges sharing v1
    e1 = tri.v1 - tri.v0;
    e2 = tri.v2 - tri.v0;
    // Begin calculating determinant - also used to calculate u parameter
    P = cross(ray.d, e2);
    // If determinant is near zero, ray lies in plane of triangle
    det = dot(e1, P);
    // Not culling
    if (det > -EPSILON && det < EPSILON) {
        return false;
    }
    inv_det = 1.0 / det;

    // Calculate distance from v1 to ray origin
    T = ray.o - tri.v0;
    
    // Calculate u parameter and test bound
    u = dot(T, P) * inv_det;
    // The intersection lies outside of the triangle
    if (u < 0.0 || u > 1.0) {
        return false;
    }

    // Prepare to test v parameter
    Q = cross(T, e1);
    
    // Calculate V parameter and test bound
    v = dot(ray.d, Q) * inv_det;
    // The intersection lies outside of the triangle
    if (v < 0.0 || u + v > 1.0) {
        return false;
    }

    t = dot(e2, Q) * inv_det;
    
    if (t > EPSILON) {
        hit = vec3(u, v, t);
        return true;
    }
    
    // No intersection
    return false;
}

vec2 coord1Dto2D(float index, vec2 invSize) {
    float offset = index * invSize.x;
    float x = fract(offset);
    float y = (offset - x) * invSize.y;
    return vec2(x, y);
}

void main() {
    mat3 m = mat3(pass_tangent, pass_bitangent, pass_normal);
    
    Ray ray;
    ray.o = pass_position.xyz;
    ray.d = pass_raydir.xyz;
    
    float closest = 10;
    vec3 normal = vec3(0, 0, 0);
    
    vec2 invSize = vec2(1.0 / arrayWidth, 1.0 / arrayHeight);
    vec3 hitInfo;
    Triangle tri;
    
    for (int i = 0; i < arrayWidth * arrayHeight; i += 3) {
        float i1 = texture(triList, coord1Dto2D(i + 0, invSize)).r;
        float i2 = texture(triList, coord1Dto2D(i + 1, invSize)).r;
        float i3 = texture(triList, coord1Dto2D(i + 2, invSize)).r;
        
        tri.v0 = texture(vertexList, coord1Dto2D(i1, invSize)).xyz;
        tri.v1 = texture(vertexList, coord1Dto2D(i2, invSize)).xyz;
        tri.v2 = texture(vertexList, coord1Dto2D(i3, invSize)).xyz;
        
        if (triangle_intersection(tri, ray, hitInfo)) {
            vec3 n1 = texture(normalList, coord1Dto2D(i1, invSize)).xyz;
            vec3 n2 = texture(normalList, coord1Dto2D(i2, invSize)).xyz;
            vec3 n3 = texture(normalList, coord1Dto2D(i3, invSize)).xyz;
        
            float u = hitInfo.x;
            float v = hitInfo.y;  
            float w = 1 - (u + v);
            vec3 n = n1 * w + n2 * u + n3 * v; 
            
            float t = hitInfo.z;
            if (dot(n, ray.d) <= 0) {
                if (t < closest) {
                    closest = t;
                    normal = n;
                }
            }
        }
    }

    normal = normalize(transpose(m) * normalize(normal));
    normal = normal * 0.5 + 0.5;
    normal.g = 1-normal.g;
    
    if (closest != 10000) {
        out_Color = vec4(normal, 1);
        return;
    }
    
    //discard;
    
    //out_Color = vec4(texture(normalList, pass_texCoord).rgb, 1);
    //out_Color = vec4(color.rgb, 1);
    
    //out_Color = vec4(texture(triList, pass_texCoord));
    out_Color = vec4(pass_texCoord, 0, 1);
}
