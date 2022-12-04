#version 410 core

// uniform inputs
uniform vec3 player1LightColor;
uniform vec3 player2LightColor;

uniform vec3 player1LightPosition;
uniform vec3 player2LightPosition;

uniform vec3 viewPos;


// varying inputs
in vec3 vertexPosition;
in vec3 Normal;
in vec3 FragPos;
in vec3 color;

// outputs
out vec4 fragColorOut;                  // color to apply to this fragment

void main() {
    // get material color

    struct Material {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float shininess;
    };

    //setting player objects to be pearl material
    Material material = Material(vec3(0.25, 0.20725, 0.20725), vec3(1,0.829,0.829), vec3(0.296648, 0.296648, 0.296648), .0888);

    vec3 materialColor = color;

    //ambient light calculations for both lights
    float ambientStrength = .1;
    vec3 ambient1 = ambientStrength * materialColor * material.ambient * player1LightColor;
    vec3 ambient2 = ambientStrength * materialColor * material.ambient * player2LightColor;


    //diffuse light calculations for both lights
    vec3 norm = normalize(Normal);
    vec3 player1LightDirection = normalize(player1LightPosition - FragPos);
    vec3 player2LightDirection = normalize(player2LightPosition - FragPos);

    float diff1 = max(dot(norm, player1LightDirection), 0.0);
    float diff2 = max(dot(norm, player2LightDirection), 0.0);
    vec3 diffuse1 = diff1 * player1LightColor * materialColor * material.diffuse;
    vec3 diffuse2 = diff2 * player2LightColor * materialColor * material.diffuse;

    //specular light calculations for both lights
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir1 = reflect(-player1LightDirection, norm);
    vec3 reflectDir2 = reflect(-player2LightDirection, norm);

    float spec1 = pow(max(dot(viewDir, reflectDir1), 0.0), 128 * material.shininess);
    float spec2 = pow(max(dot(viewDir, reflectDir2), 0.0), 128 * material.shininess);

    vec3 specular1 = specularStrength * spec1 * player1LightColor * materialColor * material.specular;
    vec3 specular2 = specularStrength * spec2 * player2LightColor * materialColor * material.specular;

    //calculate attenuation
    float d1 = distance(FragPos, player1LightPosition);
    float d2 = distance(FragPos, player2LightPosition);

    float attenuation1 = clamp( 10.0 / d1, 0.0, 1.0);
    float attenuation2 = clamp( 10.0 / d2, 0.0, 1.0);
    //add up each calculation and for both lights

    //cell shading calculations
    vec4 nonRounded = vec4(attenuation1 * (ambient1 + diffuse1 + specular1) +  attenuation2 * (ambient2 + diffuse2 + specular2), 1);
    float xFloor = floor(nonRounded.x * 10) / 10.0;
    float yFloor = floor(nonRounded.y * 10) / 10.0;
    float zFloor = floor(nonRounded.z * 10) / 10.0;
    fragColorOut = vec4(xFloor, yFloor, zFloor, 1);

}