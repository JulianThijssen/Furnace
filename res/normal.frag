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
 
float triangle_intersection(vec3 v1, vec3 v2, vec3 v3, vec3 ro, vec3 rd) {
	vec3 e1, e2;
	vec3 P, Q, T;
	float det, inv_det, u, v;
	float t;

	// Find vectors for two edges sharing v1
	e1 = v2 - v1;
	e2 = v3 - v1;
	// Begin calculating determinant - also used to calculate u parameter
	P = cross(rd, e2);
	// If determinant is near zero, ray lies in plane of triangle
	det = dot(e1, P);
	// Not culling
	if (det > -EPSILON && det < EPSILON) {
		return -1;
	}
	inv_det = 1.0 / det;

	// Calculate distance from v1 to ray origin
	T = ro - v1;
	
	// Calculate u parameter and test bound
	u = dot(T, P) * inv_det;
	// The intersection lies outside of the triangle
	if (u < 0.0 || u > 1.0) {
		return -1;
	}

	// Prepare to test v parameter
	Q = cross(T, e1);
	
	// Calculate V parameter and test bound
	v = dot(rd, Q) * inv_det;
	// The intersection lies outside of the triangle
	if (v < 0.0 || u + v > 1.0) {
		return -1;
	}

	t = dot(e2, Q) * inv_det;
	
	if (t > EPSILON) {
		return t;
	}
	
	// No intersection
	return -1;
}

vec2 coord1Dto2D(float index, vec2 invSize) {
	float offset = index * invSize.x;
	float x = fract(offset);
	float y = (offset - x) * invSize.y;
	return vec2(x, y);
}

void main() {
	mat3 m = mat3(pass_tangent, pass_bitangent, pass_normal);
	
	vec3 ro = pass_position.xyz;
	vec3 rd = pass_raydir.xyz;
	
	float closest = 10;
	vec3 normal = vec3(0, 0, 0);
	
	vec2 invSize = vec2(1.0 / arrayWidth, 1.0 / arrayHeight);
	
	for (int i = 0; i < arrayWidth * arrayHeight; i += 3) {
		float i1 = texture(triList, coord1Dto2D(i + 0, invSize)).r;
		float i2 = texture(triList, coord1Dto2D(i + 1, invSize)).r;
		float i3 = texture(triList, coord1Dto2D(i + 2, invSize)).r;
		
		vec3 v1 = texture(vertexList, coord1Dto2D(i1, invSize)).xyz;
		vec3 v2 = texture(vertexList, coord1Dto2D(i2, invSize)).xyz;
		vec3 v3 = texture(vertexList, coord1Dto2D(i3, invSize)).xyz;
		
		vec3 n1 = texture(normalList, coord1Dto2D(i1, invSize)).xyz;
		vec3 n2 = texture(normalList, coord1Dto2D(i2, invSize)).xyz;
		vec3 n3 = texture(normalList, coord1Dto2D(i3, invSize)).xyz;
		
		float t = triangle_intersection(v1, v2, v3, ro, rd);
		if (t > 0) {
			if (dot(n1, rd) <= 0) {
				if (t < closest) {
					closest = t;
					normal = n1;
				}
			}
		}
	}

	normal = normalize(transpose(m) * normalize(normal));
	normal = normal * 0.5 + 0.5;
	
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
