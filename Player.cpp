//
// Created by Aidan Lamkin on 11/14/22.
//

#include "Player.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <CSCI441/objects.hpp>
#include <CSCI441/OpenGLUtils.hpp>
#include <iostream>

Player::Player(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation,
               GLint materialColorUniformLocation, glm::vec3 startPosition) {

    _shaderProgramHandle = shaderProgramHandle;
    _shaderProgramUniformLocations.mvpMtx = mvpMtxUniformLocation;
    //_shaderProgramUniformLocations.normalMtx = normalMtxUniformLocation;
    _shaderProgramUniformLocations.materialColor = materialColorUniformLocation;

    _direction = glm::vec3(0,0,1);
    _position = startPosition;

    _colorHead = glm::vec3(1,1,1);
    _transHead = glm::vec3(0, 2.0, 0);
    _scaleHead = glm::vec3(1,1,1);

    _colorBody = glm::vec3(1,1,1);
    _transBody = glm::vec3(0,1.25,0);
    _scaleBody = glm::vec3(.65, 1, .25);

    _colorArm = glm::vec3(1,1,1);
    _transLeftArm = glm::vec3(0.28,1.6,.3);
    _transRightArm = glm::vec3(-0.4, 1.6, 0.4);
    _scaleArm = glm::vec3(.25, .25, 1);
    _rotateLeftArm = -0.6f;

    _colorLeg = glm::vec3(1,1,1);
    _transLeftLeg = glm::vec3(.2, .35,0);
    _transRightLeg = glm::vec3(-.2, .35,0);
    _scaleLeg = glm::vec3(.25, .9, .25);

    _colorHandle = glm::vec3(.588, .294, 0);
    _scaleHandle = glm::vec3(.1,.25,.1);
    _transHandle = glm::vec3(-.4,1.7,.8);

    _colorBarrel = glm::vec3(0,0,0);
    _scaleBarrel = glm::vec3(.12,.12,.3);
    _transBarrel = glm::vec3(-.4, 1.8, .88);

    _movementSpeed = 0.4f;

    deathCount = 0;
}

void Player::drawPlayer(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    glUseProgram(_shaderProgramHandle);

    modelMtx = glm::rotate(modelMtx, _rotationAngle, CSCI441::Y_AXIS);

    //hierarchical draw methods
    _drawPlayerHead(modelMtx, viewMtx, projMtx);
    _drawPlayerBody(modelMtx, viewMtx, projMtx);
    _drawPlayerArms(modelMtx, viewMtx, projMtx);
    _drawPlayerLegs(modelMtx, viewMtx, projMtx);
    _drawGun(modelMtx, viewMtx, projMtx);
}

glm::vec3 Player::getPosition() {
    return _position;
}

glm::vec3 Player::getDirection() {
    return _direction;
}

void Player::moveForward() {
    _position += _direction * _movementSpeed;
}

void Player::moveBackward() {
    _position -= _direction * _movementSpeed;
}

void Player::moveRight() {
    _position += glm::vec3(-_direction.z, 0 ,_direction.x) * _movementSpeed;

}

void Player::moveLeft() {
    _position -= glm::vec3(-_direction.z, 0 , _direction.x) * _movementSpeed;
}

void Player::setDirection(GLfloat angle) {
    _direction = glm::normalize(glm::vec3(sin(angle),0,-cos(angle)));
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
}

void Player::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.mvpMtx, 1, GL_FALSE, &mvpMtx[0][0] );
    //glm::mat3 normalMtx = glm::mat3( glm::transpose( glm::inverse( modelMtx )));
    //glProgramUniformMatrix3fv( _shaderProgramHandle, _shaderProgramUniformLocations.normalMtx, 1, GL_FALSE, &normalMtx[0][0] );
}

void Player::_drawPlayerHead(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    modelMtx = glm::translate(modelMtx, _transHead);
    modelMtx = glm::scale(modelMtx, _scaleHead);
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorHead[0]);
    CSCI441::drawSolidSphere(.25, 10, 10);
}

void Player::_drawPlayerBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    modelMtx = glm::translate(modelMtx, _transBody);
    modelMtx = glm::scale( modelMtx, _scaleBody );
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorBody[0]);
    CSCI441::drawSolidCube( 1.0 );
}

void Player::_drawPlayerArms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    glm::mat4 modelMtxRight = glm::translate(modelMtx, _transRightArm);
    modelMtxRight = glm::scale( modelMtxRight, _scaleArm );
    _computeAndSendMatrixUniforms(modelMtxRight, viewMtx, projMtx);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorArm[0]);
    CSCI441::drawSolidCube( 1.0 );

    glm::mat4 modelMtxLeft = glm::rotate(modelMtx, _rotateLeftArm, CSCI441::Y_AXIS);
    modelMtxLeft = glm::translate(modelMtxLeft, _transLeftArm);
    modelMtxLeft = glm::scale( modelMtxLeft, _scaleArm );
    _computeAndSendMatrixUniforms(modelMtxLeft, viewMtx, projMtx);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorArm[0]);
    CSCI441::drawSolidCube( 1.0 );
}

void Player::_drawPlayerLegs(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
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

void Player::_drawGun(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    glm::mat4 modelMtxHandle;
    glm::mat4 modelMtxBarrel;

    modelMtxHandle = glm::translate(modelMtx, _transHandle);
    modelMtxHandle = glm::scale(modelMtxHandle, _scaleHandle);
    _computeAndSendMatrixUniforms(modelMtxHandle,viewMtx,projMtx);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorHandle[0]);
    CSCI441::drawSolidCube(1.0);

    modelMtxBarrel = glm::translate(modelMtx, _transBarrel);
    modelMtxBarrel = glm::scale(modelMtxBarrel, _scaleBarrel);
    _computeAndSendMatrixUniforms(modelMtxBarrel,viewMtx,projMtx);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorBarrel[0]);
    CSCI441::drawSolidCube(1.0);
}

void Player::incrementArms() {
    if(_goingUp){
        _armShift += .005f;
        _transRightArm.y += .005f;
        _transLeftArm.y += .005f;
        _transHandle.y += .005f;
        _transBarrel.y += .005f;
        if(_armShift > .05){
            _goingUp = false;
        }
    }
    else{
        _armShift -= .005f;
        _transRightArm.y -= .005f;
        _transLeftArm.y -= .005f;
        _transHandle.y -= .005f;
        _transBarrel.y -= .005f;
        if(_armShift < -.05){
            _goingUp = true;
        }
    }
}

void Player::resetArms() {
    _armShift = 0;
    _transLeftArm.y = 1.6;
    _transRightArm.y = 1.6;
    _transHandle.y = 1.7;
    _transBarrel.y = 1.8;
}

GLfloat Player::getAngle() {
    return _rotationAngle;
}

void Player::checkBounds(GLfloat worldSize) {
    if(_position.x > worldSize || _position.x < -worldSize || _position.z > worldSize || _position.z < -worldSize){
        _outOfBounds = true;
    }
}

bool Player::isOutOfBounds() {
    return _outOfBounds;
}

bool Player::isDead() {
    return _dead;
}

void Player::setPosition(glm::vec3 newPosition) {
    _position = newPosition;
}

void Player::playerDied() {
    deathCount++;
    float x = ((GLfloat)rand() / (GLfloat)RAND_MAX) * 200 - 100;
    float z = ((GLfloat)rand() / (GLfloat)RAND_MAX) * 200 - 100;
    _position = glm::vec3(x,.1,z);
    health = 100;
    _outOfBounds = false;

}
