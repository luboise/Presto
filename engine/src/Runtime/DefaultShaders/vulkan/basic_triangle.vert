#version 450

layout(set = 0, binding = 0) uniform VertexShaderMatrices {
	mat4 modelView;
	mat4 projection;
} mats;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

void main() {
	gl_Position = mats.projection * mats.modelView * vec4(inPosition, 1.0);
    // fragColor = inColor;
	fragColor = vec3(inColor.x / inPosition.x + 0.1, inColor.y / inPosition.y + 0.1, inColor.z / inPosition.z + 0.1);
}
