#version 410 core

// uniform inputs
uniform mat4 mvpMatrix;                 // the precomputed Model-View-Projection Matrix
uniform mat4 model;
uniform vec3 materialColor;

// attribute inputs
in vec3 normal;
in vec3 vPos;      // the position of this specific vertex in object space

// varying outputs
out vec3 Normal;
out vec3 FragPos;
out vec3 color;

void main() {
    // transform & output the vertex in clip space
    gl_Position = mvpMatrix * vec4(vPos, 1.0);
    FragPos = vec3(model * vec4(vPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * normal;
    color = materialColor;
}