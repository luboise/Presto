constexpr auto DEFAULT_VERTEX_SHADER = R"(

#version 430

layout(location = 0) out vec2 tex_coords;
layout(location = 1) out vec4 colour;
layout(location = 2) out vec3 normal;

layout(location = 0) in vec3 _vp;
layout(location = 1) in vec3 _colour;
layout(location = 2) in vec3 _normal;
layout(location = 3) in vec2 _tex_coords;

// Global uniforms
layout(location = 0) uniform mat4 view;
layout(location = 1) uniform mat4 projection;

// Object uniforms
layout(location = 2) uniform mat4 transform;

void main() {
    gl_Position = projection * view * transform * vec4(_vp, 1.0);
    colour = vec4(_colour, 1.0);
    tex_coords = _tex_coords;

	mat3 rotationMatrix = transpose(inverse(mat3(transform)));
	normal = normalize(rotationMatrix * _normal);
}
)";

constexpr auto DEFAULT_FRAGMENT_SHADER = R"(
#version 430

out vec4 colour;

layout(location = 0) in vec2 _tex_coords;
layout(location = 1) in vec4 _colour;
layout(location = 2) in vec3 _normal;

layout(location = 3) uniform sampler2D sampler1;

void main() {
	vec3 the_sun = vec3(-20, 20, 0);
	float dotProd = dot(normalize(the_sun), normalize(_normal));

    
	float angle = acos(dotProd);
	float intensity = max(0.0, dotProd);  // Ensuring no negative values

	intensity = intensity * 0.8 + 0.2;

	// Disable lighting for debugging
    // intensity = 1;

    colour = texture(sampler1, _tex_coords) * intensity;
}
)";
