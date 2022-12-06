#version 410 core

// uniform inputs
uniform vec3 player1LightColor;
uniform vec3 player2LightColor;

uniform vec3 player1LightPosition;
uniform vec3 player2LightPosition;

uniform sampler2D grassTexture;

uniform vec3 viewPos;

// varying inputs
in vec2 texelCoords;
in vec3 vertexPosition;
in vec3 Normal;
in vec3 FragPos;

// outputs
out vec4 fragColorOut;                  // color to apply to this fragment


void main() {
    // get material color from corresponding texture coordinate
    vec4 materialColor = texture(grassTexture, texelCoords);
    fragColorOut = materialColor;
}