//
// Created by Aidan Lamkin on 11/14/22.
//

#include "Enemy.h"
#include <glm/gtc/matrix_transform.hpp>

#include <CSCI441/objects.hpp>
#include <CSCI441/OpenGLUtils.hpp>
#include <iostream>

Enemy::Enemy(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation,
             GLint materialColorUniformLocation, glm::vec3 position, GLfloat health, GLfloat movementSpeed) {
    _shaderProgramHandle = shaderProgramHandle;
    _shaderProgramUniformLocations.mvpMtx = mvpMtxUniformLocation;
    _shaderProgramUniformLocations.normalMtx = normalMtxUniformLocation;
    _shaderProgramUniformLocations.materialColor = materialColorUniformLocation;

    _health = health;

    _direction = glm::vec3(0,0,1);
    _position = position;

    _colorHead = glm::vec3(.2,1,0);
    _transHead = glm::vec3(0, 2.0, 0);
    _scaleHead = glm::vec3(1,1,1);

    _colorBody = glm::vec3(.5,.2,0);
    _transBody = glm::vec3(0,1.25,0);
    _scaleBody = glm::vec3(.65, 1, .25);

    _colorArm = glm::vec3(.2,1,0);
    _transLeftArm = glm::vec3(0.4,1.6,.4);
    _transRightArm = glm::vec3(-0.4, 1.6, 0.4);
    _scaleArm = glm::vec3(.25, .25, 1);

    _colorLeg = glm::vec3(0,0,1);
    _transLeftLeg = glm::vec3(.2, .35,0);
    _transRightLeg = glm::vec3(-.2, .35,0);
    _scaleLeg = glm::vec3(.25, .9, .25);

    _movementSpeed = movementSpeed;

}

void Enemy::drawEnemy(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    glUseProgram(_shaderProgramHandle);

    modelMtx = glm::rotate(modelMtx, _rotationAngle, CSCI441::Y_AXIS);

    //hierarchical draw methods
    _drawPlayerHead(modelMtx, viewMtx, projMtx);
    _drawPlayerBody(modelMtx, viewMtx, projMtx);
    _drawPlayerArms(modelMtx, viewMtx, projMtx);
    _drawPlayerLegs(modelMtx, viewMtx, projMtx);
}

void Enemy::moveForward() {
    _position += -_direction * _movementSpeed;
}

bool Enemy::isOutOfBounds() {
    return _outOfBounds;
}

bool Enemy::isDead() {
    return _dead;
}

void Enemy::enemyDied() {
    _dead = true;
}

Enemy::~Enemy() {}

glm::vec3 Enemy::getPosition() {
    return _position;
}

void Enemy::setPosition(glm::vec3 newPosition) {
    _position = newPosition;
}

glm::vec3 Enemy::getDirection() {
    return _direction;
}

void Enemy::newDirection(glm::vec3 newDirection, GLfloat angle) {
    _direction = newDirection;

    while(angle > 2 *M_PI){
        angle -= 2 * M_PI;
    }
    while(angle < 0){
        angle += 2 * M_PI;
    }
    if(angle > M_PI_2 && angle < 3 * M_PI_2) {
        _rotationAngle = asin(_direction.x);
    }
    else if(angle < M_PI_2){
        _rotationAngle = acos(_direction.z);
    }
    else if(angle > 3 * M_PI_2){
        _rotationAngle = -acos(_direction.z);
    }
    _rotationAngle += M_PI;
}

void Enemy::checkBounds(GLfloat worldSize) {
    if(_position.x > worldSize || _position.x < -worldSize || _position.z > worldSize || _position.z < -worldSize){
        _outOfBounds = true;
    }
}

void Enemy::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.mvpMtx, 1, GL_FALSE, &mvpMtx[0][0] );
    glm::mat3 normalMtx = glm::mat3( glm::transpose( glm::inverse( modelMtx )));
    glProgramUniformMatrix3fv( _shaderProgramHandle, _shaderProgramUniformLocations.normalMtx, 1, GL_FALSE, &normalMtx[0][0] );
}

void Enemy::_drawPlayerHead(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    modelMtx = glm::translate(modelMtx, _transHead);
    modelMtx = glm::scale(modelMtx, _scaleHead);
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorHead[0]);
    CSCI441::drawSolidSphere(.25, 10, 10);
}

void Enemy::_drawPlayerBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    modelMtx = glm::translate(modelMtx, _transBody);
    modelMtx = glm::scale( modelMtx, _scaleBody );
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorBody[0]);
    CSCI441::drawSolidCube( 1.0 );
}

void Enemy::_drawPlayerArms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    glm::mat4 modelMtxRight = glm::translate(modelMtx, _transRightArm);
    modelMtxRight = glm::scale( modelMtxRight, _scaleArm );
    _computeAndSendMatrixUniforms(modelMtxRight, viewMtx, projMtx);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorArm[0]);
    CSCI441::drawSolidCube( 1.0 );

    glm::mat4 modelMtxLeft = glm::translate(modelMtx, _transLeftArm);
    modelMtxLeft = glm::scale( modelMtxLeft, _scaleArm );
    _computeAndSendMatrixUniforms(modelMtxLeft, viewMtx, projMtx);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorArm[0]);
    CSCI441::drawSolidCube( 1.0 );
}

void Enemy::_drawPlayerLegs(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    glm::mat4 modelMtxRight;
    glm::mat4 modelMtxLeft;
    modelMtxRight = glm::translate(modelMtx, _transRightLeg);
    modelMtxRight = glm::scale( modelMtxRight, _scaleLeg );
    _computeAndSendMatrixUniforms(modelMtxRight, viewMtx, projMtx);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorLeg[0]);
    CSCI441::drawSolidCube( 1.0 );

    modelMtxLeft = glm::translate(modelMtx, _transLeftLeg);
    modelMtxLeft = glm::scale( modelMtxLeft, _scaleLeg );
    _computeAndSendMatrixUniforms(modelMtxLeft, viewMtx, projMtx);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorLeg[0]);
    CSCI441::drawSolidCube( 1.0 );
}

void Enemy::incrementArms() {
    if(_goingUp){
        _armShift += .005f;
        _transRightArm.y += .005f;
        _transLeftArm.y -= .005f;
        if(_armShift > .05){
            _goingUp = false;
        }
    }
    else{
        _armShift -= .005f;
        _transRightArm.y -= .005f;
        _transLeftArm.y += .005f;
        if(_armShift < -.05){
            _goingUp = true;
        }
    }
}

void Enemy::resetArms() {
    _armShift = 0;
    _transLeftArm.y = 1.6;
    _transRightArm.y = 1.6;
}

GLfloat Enemy::getAngle() {
    return _rotationAngle;
}

void Enemy::moveBackward() {
    _position -= -_direction * _movementSpeed;
}

void Enemy::decreaseHealth(GLfloat damage) {
    _health -= damage;
    if(_health < 0){
        enemyDied();
    }
}
