//
// Created by Aidan Lamkin on 11/14/22.
//

#ifndef A3_ENEMY_H
#define A3_ENEMY_H
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <vector>

class Enemy {
public:
    //constructor and draw
    Enemy( GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation, glm::vec3 position, GLfloat health, GLfloat movementSpeed);
    void drawEnemy(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx);

    //movement
    void moveForward();
    void moveBackward();

    //death related items
    bool isOutOfBounds();
    bool isDead();
    void enemyDied();

    //destructor
    virtual ~Enemy();

    //getters and setters
    glm::vec3 getPosition();
    GLfloat getAngle();
    void setPosition(glm::vec3 newPosition);

    glm::vec3 getDirection();
    void newDirection(glm::vec3 newDirection, GLfloat angle);

    void checkBounds(GLfloat worldSize);

    void decreaseHealth(GLfloat damage);

    void incrementArms();
    void resetArms();
private:
    GLfloat _health;

    glm::vec3 _position;
    GLfloat _movementSpeed;
    glm::vec3 _direction;
    GLfloat _rotationAngle = 0.0;

    bool _outOfBounds = false;
    bool _dead = false;

    GLfloat _armShift = 0;
    bool _goingUp = true;

    //shader stuff
    GLuint _shaderProgramHandle;

    struct ShaderProgramUniformLocations {
        GLint mvpMtx;
        GLint normalMtx;
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

    glm::vec3 _colorLeg;
    glm::vec3 _scaleLeg;
    glm::vec3 _transLeftLeg;
    glm::vec3 _transRightLeg;

    void _drawPlayerHead(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );

    void _drawPlayerBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );

    void _drawPlayerArms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );

    void _drawPlayerLegs(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );

    void _computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;
};


#endif //A3_ENEMY_H
