#version 450
layout(location = 0) in vec2 inPosition;  // Input vertex position
layout(location = 1) in vec3 inColor;     // Input vertex color

layout(location = 0) out vec3 fragColor;  // Output color to fragment shader

void main() {
    fragColor = inColor;
    gl_Position = vec4(inPosition, 0.0, 1.0);
}
