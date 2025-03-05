constexpr auto DEFAULT_VERTEX_SHADER = R"(

#version 430

layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec3 a_colour;
layout(location = 2) in vec3 a_normal;
layout(location = 3) in vec2 a_texcoords;

layout(location = 0) out vec4 colour;
layout(location = 1) out vec3 normal;
layout(location = 2) out vec2 tex_coords;

layout(std140, binding = 0) uniform GlobalUniforms {
    mat4 view;
    mat4 projection;
};

layout(std140, binding = 1) uniform ObjectUniforms {
    mat4 model;
};

layout(std140, binding = 2) uniform PBROptions {
	vec4 u_baseColour;
	float u_metallic;
	float u_roughness;
};

void main() {
    gl_Position = projection * view * model * vec4(a_vertexPosition, 1.0);
    colour = vec4(a_colour, 1.0);
    tex_coords = a_texcoords;

	mat3 rotationMatrix = transpose(inverse(mat3(model)));
	normal = normalize(rotationMatrix * a_normal);
}
)";

constexpr auto DEFAULT_FRAGMENT_SHADER = R"(
#version 430

out vec4 colour;

layout(location = 0) in vec4 _colour;
layout(location = 1) in vec3 _normal;
layout(location = 2) in vec2 _tex_coords;

layout(binding = 3) uniform sampler2D u_diffuseTexture;

void main() {
	vec3 the_sun = vec3(-20, 20, 0);
	float dotProd = dot(normalize(the_sun), normalize(_normal));

    
	float angle = acos(dotProd);
	float intensity = max(0.0, dotProd);  // Ensuring no negative values

	intensity = intensity * 0.8 + 0.2;

	// Disable lighting for debugging
    // intensity = 1;

    colour = texture(u_diffuseTexture, _tex_coords) * intensity;
}
)";

constexpr auto DEFAULT_UI_VERTEX_SHADER = R"(
#version 430

layout(location = 0) in vec2 a_vertexPosition;
layout(location = 1) in vec3 a_colour;
layout(location = 2) in vec2 a_texcoords;

layout(location = 0) out vec4 colour;
layout(location = 1) out vec2 tex_coords;

layout(std140, binding = 0) uniform GlobalUniforms {
    mat4 view;
    mat4 projection;
};

void main() {
    gl_Position = projection * view * vec4(a_vertexPosition.x, 
	// Need to flip the Y pos as OpenGL has tex coords start in bottom left instead of top left
											1 - a_vertexPosition.y,
											0,
											1.0);
    										colour = vec4(a_colour, 1.0);
    tex_coords = a_texcoords;
}
)";

constexpr auto DEFAULT_UI_FRAGMENT_SHADER = R"(
#version 430

out vec4 colour;

layout(location = 0) in vec4 _colour;
layout(location = 1) in vec2 _tex_coords;
layout(binding = 2) uniform sampler2D u_diffuseTexture;

void main() {
    colour = texture(u_diffuseTexture, _tex_coords) * _colour;
}
)";
