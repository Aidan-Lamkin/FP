//
// Created by Aidan Lamkin on 11/16/22.
//

#ifndef A5_BULLET_H
#define A5_BULLET_H
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <vector>

class Bullet {
public:
    //constructor and draw
    Bullet(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint materialColorUniformLocation, glm::vec3 position, glm::vec3 direction, GLfloat angle, int shotBy);
    void drawBullet(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx);

    //move methods
    void moveForward();
    bool checkBounds(GLfloat worldSize);

    //getters
    glm::vec3 getPosition();
    GLfloat getDamage();

    int shotBy;
private:
    glm::vec3 _position;
    GLfloat _movementSpeed;
    glm::vec3 _direction;
    glm::vec3 _offset  = glm::vec3(-.4, 1.8, 1);
    GLfloat _angle;
    GLfloat _damage = 10;

    //shader stuff
    GLuint _shaderProgramHandle;

    struct ShaderProgramUniformLocations {
        GLint mvpMtx;
        GLint materialColor;
    } _shaderProgramUniformLocations;

    glm::vec3 _colorBullet;

    void _computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;

};


#endif //A5_BULLET_H
