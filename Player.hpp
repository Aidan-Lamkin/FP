//
// Created by Aidan Lamkin on 11/14/22.
//

#ifndef A3_PLAYER_HPP
#define A3_PLAYER_HPP

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <vector>

class Player {
public:
    //constructor and draw
    Player( GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint materialColorUniformLocation, glm::vec3 startPosition );
    void drawPlayer(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx);

    //movement
    void moveForward();
    void moveBackward();
    void moveRight();
    void moveLeft();

    //death related
    bool isOutOfBounds();
    bool isDead();
    void playerDied();

    //getters and setters
    void setDirection(GLfloat angle);

    GLfloat getAngle();

    GLfloat getCurrentJumpHeight();

    glm::vec3 getPosition();
    void setPosition(glm::vec3 newPosition);

    glm::vec3 getDirection();

    void incrementArms();

    void resetArms();

    void checkBounds(GLfloat worldSize);

    int deathCount;
    int health = 100;
    float timeSinceLastShot = 0.0f;
private:

    //movement related members
    glm::vec3 _position;
    GLfloat _movementSpeed;
    glm::vec3 _direction;
    GLfloat _rotationAngle = 0.0;

    GLfloat _armShift = 0;
    bool _goingUp = true;

    //Life bools
    bool _outOfBounds = false;
    bool _dead = false;


    //shader stuff
    GLuint _shaderProgramHandle;

    struct ShaderProgramUniformLocations {
        GLint mvpMtx;
        //GLint normalMtx;
        GLint materialColor;
    } _shaderProgramUniformLocations;

    //colors, scales, and offsets for body parts

    glm::vec3 _colorBody;
    glm::vec3 _scaleBody;
    glm::vec3 _transBody;

    glm::vec3 _colorHead;
    glm::vec3 _scaleHead;
    glm::vec3 _transHead;

    glm::vec3 _colorArm;
    glm::vec3 _scaleArm;
    glm::vec3 _transLeftArm;
    glm::vec3 _transRightArm;
    GLfloat _rotateLeftArm;

    glm::vec3 _colorLeg;
    glm::vec3 _scaleLeg;
    glm::vec3 _transLeftLeg;
    glm::vec3 _transRightLeg;

    glm::vec3 _colorHandle;
    glm::vec3 _scaleHandle;
    glm::vec3 _transHandle;
    glm::vec3 _colorBarrel;
    glm::vec3 _scaleBarrel;
    glm::vec3 _transBarrel;



    //draw methods

    void _drawPlayerHead(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );

    void _drawPlayerBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );

    void _drawPlayerArms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );

    void _drawPlayerLegs(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );

    void _drawGun(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );

    void _computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;
};


#endif //A3_PLAYER_HPP
