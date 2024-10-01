#version 400

out vec4 frag_colour;

in vec4 to_frag;

void main() {
    frag_colour = to_frag;
}
