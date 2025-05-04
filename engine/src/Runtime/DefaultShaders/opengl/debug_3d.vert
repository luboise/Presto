#version 430

layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec4 a_colour;

layout(location = 0) out vec4 colour;

layout(std140, binding = 0) uniform GlobalUniforms {
    mat4 view;
    mat4 projection;
};

layout(std140, binding = 1) uniform ObjectUniforms {
    mat4 model;
};

void main() {
    gl_Position = projection * view * model * vec4(a_vertexPosition, 1.0);
    colour = a_colour;
}
