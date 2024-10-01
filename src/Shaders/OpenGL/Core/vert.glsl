#version 400

out vec4 to_frag;

in vec3 vp;
in vec3 colour;

void main() {
	gl_Position = vec4(vp, 1.0);
	to_frag = vec4(colour, 1.0);
}
