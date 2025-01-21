constexpr auto DEFAULT_VERTEX_SHADER = R"(

#version 430

layout(location = 0) out vec2 tex_coords;
layout(location = 1) out vec4 colour;

layout(location = 0) in vec3 _vp;
layout(location = 1) in vec3 _colour;
layout(location = 2) in vec3 _normal;
layout(location = 3) in vec2 _tex_coords;

uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * vec4(_vp, 1.0);
    colour = vec4(_colour, 1.0);
    tex_coords = _tex_coords;
}
)";

constexpr auto DEFAULT_FRAGMENT_SHADER = R"(
#version 430

out vec4 colour;

layout(location = 0) in vec2 _tex_coords;
layout(location = 1) in vec4 _colour;

layout(location = 0) uniform sampler2D sampler1;

void main() {
    colour = texture(sampler1, _tex_coords);
	// colour = vec4(1,1,1,1);
}
)";
