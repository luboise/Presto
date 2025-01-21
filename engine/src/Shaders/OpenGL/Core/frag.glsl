#version 400

out vec4 colour;

in vec2 _tex_coords;
in vec4 _colour;

uniform sampler2D sampler1;

void main() {
    // colour = texture(sampler1, _tex_coords);
	colour = vec4(1,1,1,1);
}
