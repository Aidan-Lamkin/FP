//
// Created by Aidan Lamkin on 11/16/22.
//

#include "Bullet.h"
#include <glm/gtc/matrix_transform.hpp>

#include <CSCI441/objects.hpp>
#include <CSCI441/OpenGLUtils.hpp>

Bullet::Bullet(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation,
               GLint materialColorUniformLocation, glm::vec3 position, glm::vec3 direction, GLfloat angle, int shotBy) {
    _shaderProgramHandle = shaderProgramHandle;
    _shaderProgramUniformLocations.mvpMtx = mvpMtxUniformLocation;
    _shaderProgramUniformLocations.normalMtx = normalMtxUniformLocation;
    _shaderProgramUniformLocations.materialColor = materialColorUniformLocation;

    _angle = angle;
    this->shotBy = shotBy;

    _direction = direction;
    _position = position;

    _colorBullet = glm::vec3(.688, .394, 0);
    _movementSpeed = 1;

}



glm::vec3 Bullet::getPosition() {
    return _position;
}

void Bullet::moveForward() {
    _position += _direction * _movementSpeed;
}

GLfloat Bullet::getDamage() {
    return _damage;
}

void Bullet::drawBullet(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    glUseProgram(_shaderProgramHandle);
    modelMtx = glm::rotate(modelMtx, _angle, CSCI441::Y_AXIS);
    modelMtx = glm::translate(modelMtx, _offset);
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    glUniform3fv(_shaderProgramUniformLocations.materialColor,1,&_colorBullet[0]);
    CSCI441::drawSolidSphere(.05,10,10);
}

void Bullet::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.mvpMtx, 1, GL_FALSE, &mvpMtx[0][0] );
    glm::mat3 normalMtx = glm::mat3( glm::transpose( glm::inverse( modelMtx )));
    glProgramUniformMatrix3fv( _shaderProgramHandle, _shaderProgramUniformLocations.normalMtx, 1, GL_FALSE, &normalMtx[0][0] );
}

bool Bullet::checkBounds(GLfloat worldSize) {
    return !(_position.x > worldSize || _position.x < -worldSize || _position.z > worldSize ||
             _position.z < -worldSize);
}
