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

layout(std140, binding = 1) uniform UIUniforms {
    vec2 u_position;
    vec2 u_offset;
    float u_size;
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
