#version 410 core

// uniform inputs
uniform mat4 mvpMatrix;                 // the precomputed Model-View-Projection Matrix
uniform mat4 model;
uniform float time;
// attribute inputs
in vec3 normal;
in vec3 vPos;      // the position of this specific vertex in object space

// varying outputs
out vec3 Normal;
out vec3 FragPos;
out vec2 texelCoords;

void main() {
    // transform & output the vertex in clip space
    vec3 newVPos = vPos + vec3(sin(time + vPos.x) * vPos.y * 0.5, 0.0, 0.0);
    gl_Position = mvpMatrix * vec4(newVPos, 1.0);
    FragPos = vec3(model * vec4(newVPos, 1.0));
    texelCoords = vec2(newVPos.x, newVPos.z);
    Normal = mat3(transpose(inverse(model))) * normal;
}