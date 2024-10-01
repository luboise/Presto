#version 400


out vec4 to_frag;

in vec3 vp;
in vec3 colour;

uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * vec4(vp, 1.0);
	to_frag = vec4(colour, 1.0);
}
