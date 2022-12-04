#include "FPEngine.hpp"

#include <CSCI441/objects.hpp>
#include <iostream>
#include <stb_image.h>

//*************************************************************************************
//
// Helper Functions

#ifndef M_PI
#define M_PI 3.14159265
#endif

GLfloat skyboxVertices[] =
        {
                -1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, -1.0f,
                -1.0f, 1.0f, -1.0f
        };

unsigned int skyboxIndices[] =
        {
                //Right
                1,2,6,6,5,1,
                //left
                0,4,7,7,3,0,
                //top
                4,5,6,6,7,4,
                //bottom
                0,3,2,2,1,0,
                //back
                0,1,5,5,4,0,
                //front
                3,7,6,6,2,3
        };

/// \desc Simple helper function to return a random number between 0.0f and 1.0f.
GLfloat getRand() {
    return (GLfloat)rand() / (GLfloat)RAND_MAX;
}

//*************************************************************************************
//
// Public Interface

FPEngine::FPEngine()
         : CSCI441::OpenGLEngine(4, 1,
                                 1080, 960,
                                 "FP") {

    for(auto& _key : _keys) _key = GL_FALSE;

    _mousePosition = glm::vec2(MOUSE_UNINITIALIZED, MOUSE_UNINITIALIZED );
}

FPEngine::~FPEngine() {
    delete _freeCamPlayer1;
    delete _freeCamPlayer2;
}

void FPEngine::handleKeyEvent(GLint key, GLint action) {
    if(key != GLFW_KEY_UNKNOWN)
        _keys[key] = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));

    if(action == GLFW_PRESS) {
        switch( key ) {
            // quit!
            case GLFW_KEY_ESCAPE:
                setWindowShouldClose();
                break;
            default: break; // suppress CLion warning
        }
    }
    if(action == GLFW_RELEASE){
        switch(key) {
            case GLFW_KEY_S:
                _player1->resetArms();
                return;
            case GLFW_KEY_W:
                _player1->resetArms();
                return;
            case GLFW_KEY_K:
                _player2->resetArms();
                return;
            case GLFW_KEY_I:
                _player2->resetArms();
                return;
            case GLFW_KEY_LEFT_SHIFT:
                player1CanShoot = true;
                return;
            case GLFW_KEY_SLASH:
                player2CanShoot = true;
                return;
            default:
                break;
        }
    }
}

void FPEngine::handleMouseButtonEvent(GLint button, GLint action) {}

void FPEngine::handleCursorPositionEvent(glm::vec2 currMousePosition) {
    // if mouse hasn't moved in the window, prevent camera from flipping out
    if(_mousePosition.x == MOUSE_UNINITIALIZED) {
        _mousePosition = currMousePosition;
    }

    // update the mouse position
    _mousePosition = currMousePosition;
}

//*************************************************************************************
//
// Engine Setup

void FPEngine::_setupGLFW() {
    CSCI441::OpenGLEngine::_setupGLFW();

    // set our callbacks
    glfwSetKeyCallback(_window, a3_engine_keyboard_callback);
    glfwSetMouseButtonCallback(_window, a3_engine_mouse_button_callback);
    glfwSetCursorPosCallback(_window, a3_engine_cursor_callback);
}

void FPEngine::_setupOpenGL() {
    glEnable( GL_DEPTH_TEST );					                        // enable depth testing
    glDepthFunc( GL_LESS );							                // use less than depth test

    glEnable(GL_BLEND);									            // enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	    // use one minus blending equation

    glClearColor( 0.0f, 1.0f, 1.0f, 1.0f );	        // clear the frame buffer to black
}

void FPEngine::_setupShaders() {
    _groundShaderProgram = new CSCI441::ShaderProgram("shaders/groundShader.v.glsl", "shaders/groundShader.f.glsl" );
    _groundShaderUniformLocations.mvpMatrix = _groundShaderProgram->getUniformLocation("mvpMatrix");
    _groundShaderUniformLocations.model = _groundShaderProgram->getUniformLocation("model");
    _groundShaderUniformLocations.player1LightColor = _groundShaderProgram->getUniformLocation("player1LightColor");
    _groundShaderUniformLocations.player1LightPosition = _groundShaderProgram->getUniformLocation("player1LightPosition");
    _groundShaderUniformLocations.player2LightColor = _groundShaderProgram->getUniformLocation("player2LightColor");
    _groundShaderUniformLocations.player2LightPosition = _groundShaderProgram->getUniformLocation("player2LightPosition");
    _groundShaderUniformLocations.groundTexture = _groundShaderProgram->getUniformLocation("groundTexture");
    _groundShaderUniformLocations.viewPos = _groundShaderProgram->getUniformLocation("viewPos");

    _groundShaderAttributeLocations.vPos = _groundShaderProgram->getAttributeLocation("vPos");
    _groundShaderAttributeLocations.normal = _groundShaderProgram->getAttributeLocation("normal");

    _grassShaderProgram = new CSCI441::ShaderProgram("shaders/grassShader.v.glsl", "shaders/grassShader.f.glsl");
    _grassShaderUniformLocations.mvpMatrix = _grassShaderProgram->getUniformLocation("mvpMatrix");
    _grassShaderUniformLocations.model = _grassShaderProgram->getUniformLocation("model");
    _grassShaderUniformLocations.player1LightColor = _grassShaderProgram->getUniformLocation("player1LightColor");
    _grassShaderUniformLocations.player1LightPosition = _grassShaderProgram->getUniformLocation("player1LightPosition");
    _grassShaderUniformLocations.player2LightColor = _grassShaderProgram->getUniformLocation("player2LightColor");
    _grassShaderUniformLocations.player2LightPosition = _grassShaderProgram->getUniformLocation("player2LightPosition");
    _grassShaderUniformLocations.grassTexture = _grassShaderProgram->getUniformLocation("grassTexture");
    _grassShaderUniformLocations.viewPos = _grassShaderProgram->getUniformLocation("viewPos");
    _grassShaderUniformLocations.time = _grassShaderProgram->getUniformLocation("time");

    _grassShaderAttributeLocations.vPos = _grassShaderProgram->getAttributeLocation("vPos");
    _grassShaderAttributeLocations.normal = _grassShaderProgram->getAttributeLocation("normal");

    _playerShaderProgram = new CSCI441::ShaderProgram("shaders/playerShader.v.glsl","shaders/playerShader.f.glsl");
    _playerShaderUniformLocations.mvpMatrix = _playerShaderProgram->getUniformLocation("mvpMatrix");
    _playerShaderUniformLocations.model = _playerShaderProgram->getUniformLocation("model");
    _playerShaderUniformLocations.player1LightColor = _playerShaderProgram->getUniformLocation("player1LightColor");
    _playerShaderUniformLocations.player1LightPosition = _playerShaderProgram->getUniformLocation("player1LightPosition");
    _playerShaderUniformLocations.player2LightColor = _playerShaderProgram->getUniformLocation("player2LightColor");
    _playerShaderUniformLocations.player2LightPosition = _playerShaderProgram->getUniformLocation("player2LightPosition");
    _playerShaderUniformLocations.viewPos = _playerShaderProgram->getUniformLocation("viewPos");
    _playerShaderUniformLocations.materialColor = _playerShaderProgram->getUniformLocation("materialColor");

    _playerShaderAttributeLocations.vPos = _playerShaderProgram->getAttributeLocation("vPos");
    _playerShaderAttributeLocations.normal = _playerShaderProgram->getAttributeLocation("normal");

    _skyboxShaderProgram = new CSCI441::ShaderProgram("shaders/skybox.v.glsl", "shaders/skybox.f.glsl");
    _skyboxShaderUniformLocations.projection = _skyboxShaderProgram->getUniformLocation("projection");
    _skyboxShaderUniformLocations.view = _skyboxShaderProgram->getUniformLocation("view");
    _skyboxShaderUniformLocations.skybox = _skyboxShaderProgram->getUniformLocation("skybox");
    _skyboxShaderAttributeLocations.aPos = _skyboxShaderProgram->getAttributeLocation("aPos");
}

//initializes player and two enemies
void FPEngine::_setupBuffers() {
    CSCI441::setVertexAttributeLocations(_groundShaderAttributeLocations.vPos, _groundShaderAttributeLocations.normal);
    CSCI441::setVertexAttributeLocations(_grassShaderAttributeLocations.vPos, _grassShaderAttributeLocations.normal);


    _player1 = new Player(_playerShaderProgram->getShaderProgramHandle(),
                          _playerShaderUniformLocations.mvpMatrix,
                          _playerShaderUniformLocations.materialColor,
                          glm::vec3(25, .1, 25));

    _player2 = new Player(_playerShaderProgram->getShaderProgramHandle(),
                          _playerShaderUniformLocations.mvpMatrix,
                          _playerShaderUniformLocations.materialColor,
                          glm::vec3(-25, .1, -25));

    _createGroundBuffers();
    _createGrassBuffers();
    _createSkybox();

}

void FPEngine::_createGroundBuffers() {
    struct Vertex {
        GLfloat x, y, z;
        GLfloat normalX, normalY, normalZ;

    };

    Vertex groundQuad[4] = {
            {-1.0f, 0.0f, -1.0f, 0,1,0},
            { 1.0f, 0.0f, -1.0f,0,1,0},
            {-1.0f, 0.0f,  1.0f,0,1,0},
            { 1.0f, 0.0f,  1.0f,0,1,0}
    };

    GLushort indices[4] = {0,1,2,3};

    _numGroundPoints = 4;

    glGenVertexArrays(1, &_groundVAO);
    glBindVertexArray(_groundVAO);

    GLuint vbods[2];       // 0 - VBO, 1 - IBO
    glGenBuffers(2, vbods);
    glBindBuffer(GL_ARRAY_BUFFER, vbods[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundQuad), groundQuad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(_groundShaderAttributeLocations.vPos);
    glVertexAttribPointer(_groundShaderAttributeLocations.vPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(_groundShaderAttributeLocations.normal);
    glVertexAttribPointer(_groundShaderAttributeLocations.normal, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbods[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    _texHandles[TEXTURE_ID::GROUND] = _loadAndRegisterTexture("data/bottom.jpg");

}


void FPEngine::_setupScene() {

    _freeCamPlayer1 = new CSCI441::FreeCam();
    _freeCamPlayer1->setPosition(_player1->getPosition() + glm::vec3(0, 1.85, 0.2));
    _freeCamPlayer1->setTheta(M_PI);
    _freeCamPlayer1->setPhi(M_PI_2);
    _freeCamPlayer1->recomputeOrientation();

    _freeCamPlayer2 = new CSCI441::FreeCam();
    _freeCamPlayer2->setPosition(_player2->getPosition() + glm::vec3(0, 1.85, 0.2));
    _freeCamPlayer2->setTheta(M_PI);
    _freeCamPlayer2->setPhi(M_PI_2);
    _freeCamPlayer2->recomputeOrientation();

    glm::vec3 lightColor1 = glm::vec3(0,1,0);
    glm::vec3 lightPosition1 = glm::vec3(_player1->getPosition().x, _player1->getPosition().y + 5, _player1->getPosition().z);
    glm::vec3 lightColor2 = glm::vec3(0,1,0);
    glm::vec3 lightPosition2 = glm::vec3(_player2->getPosition().x, _player2->getPosition().y + 5, _player2->getPosition().z);

    glProgramUniform3fv(_groundShaderProgram->getShaderProgramHandle(), _groundShaderUniformLocations.player1LightColor, 1, &lightColor1[0]);
    glProgramUniform3fv(_groundShaderProgram->getShaderProgramHandle(), _groundShaderUniformLocations.player1LightPosition, 1, &lightPosition1[0]);
    glProgramUniform3fv(_groundShaderProgram->getShaderProgramHandle(), _groundShaderUniformLocations.player2LightColor, 1, &lightColor2[0]);
    glProgramUniform3fv(_groundShaderProgram->getShaderProgramHandle(), _groundShaderUniformLocations.player2LightPosition, 1, &lightPosition2[0]);

    glProgramUniform3fv(_playerShaderProgram->getShaderProgramHandle(), _playerShaderUniformLocations.player1LightColor, 1, &lightColor1[0]);
    glProgramUniform3fv(_playerShaderProgram->getShaderProgramHandle(), _playerShaderUniformLocations.player1LightPosition, 1, &lightPosition1[0]);
    glProgramUniform3fv(_playerShaderProgram->getShaderProgramHandle(), _playerShaderUniformLocations.player2LightColor, 1, &lightColor2[0]);
    glProgramUniform3fv(_playerShaderProgram->getShaderProgramHandle(), _playerShaderUniformLocations.player2LightPosition, 1, &lightPosition2[0]);

}

//*************************************************************************************
//
// Engine Cleanup

void FPEngine::_cleanupShaders() {
    fprintf( stdout, "[INFO]: ...deleting Shaders.\n" );
    delete _groundShaderProgram;
    delete _skyboxShaderProgram;
    delete _playerShaderProgram;
}

void FPEngine::_cleanupBuffers() {
    fprintf( stdout, "[INFO]: ...deleting VAOs....\n" );
    CSCI441::deleteObjectVAOs();
    glDeleteVertexArrays( 1, &_groundVAO );

    fprintf( stdout, "[INFO]: ...deleting VBOs....\n" );
    CSCI441::deleteObjectVBOs();

    fprintf( stdout, "[INFO]: ...deleting models..\n" );
    delete _player1;
    delete _player2;
}

//*************************************************************************************
//
// Rendering / Drawing Functions - this is where the magic happens!

void FPEngine::_renderScene(glm::mat4 viewMtx, glm::mat4 projMtx) const {
    //draw skybox
    //meets one of the texture requirements
    glDepthFunc(GL_LEQUAL);
    _skyboxShaderProgram->useProgram();
    glm::mat4 v = glm::mat4(glm::mat3(viewMtx));
    glUniformMatrix4fv(_skyboxShaderUniformLocations.view, 1, GL_FALSE, &v[0][0]);
    glUniformMatrix4fv(_skyboxShaderUniformLocations.projection, 1, GL_FALSE, &projMtx[0][0]);
    glBindVertexArray(skyBoxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);

    // use our lighting shader program
    _groundShaderProgram->useProgram();
    // draw the ground plane
    glm::mat4 groundModelMtx = glm::scale( glm::mat4(1.0f), glm::vec3(WORLD_SIZE, 1.0f, WORLD_SIZE));
    _computeAndSendMatrixUniforms(groundModelMtx, viewMtx, projMtx);
    glBindVertexArray(_groundVAO);
    glDrawElements(GL_TRIANGLE_STRIP, _numGroundPoints, GL_UNSIGNED_SHORT, (void*)0);

    //TODO use grass shader to load sprites of grass that sway
    _grassShaderProgram->useProgram();
    glBindTexture(GL_TEXTURE_2D, _texHandles[TEXTURE_ID::GRASS]);
    _grassShaderProgram->setProgramUniform(_grassShaderUniformLocations.time, time);
    if(viewMtx == _freeCamPlayer1->getViewMatrix()){
        _grassShaderProgram->setProgramUniform(_grassShaderUniformLocations.viewPos, _freeCamPlayer1->getPosition());
    }
    else{
        _grassShaderProgram->setProgramUniform(_grassShaderUniformLocations.viewPos, _freeCamPlayer2->getPosition());
    }
    glBindVertexArray(_grassVAO);
    for(int i = 0; i < _grassPositions.size(); i++){
        glm::mat4 model = glm::translate(glm::mat4(1.0f),_grassPositions[i]);
        glm::mat4 mvp = projMtx * viewMtx * model;
        _grassShaderProgram->setProgramUniform(_grassShaderUniformLocations.model, model);
        _grassShaderProgram->setProgramUniform(_grassShaderUniformLocations.mvpMatrix, mvp);
        glDrawElements(GL_TRIANGLE_STRIP, _numGrassPoints, GL_UNSIGNED_SHORT, (void*)0);
    }


    //draw players
    _playerShaderProgram->useProgram();

    glm::mat4 modelMtx(1.0f);
    modelMtx = glm::translate(modelMtx, _player1->getPosition());
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    // then send it to the shader on the GPU to apply to every vertex
    _playerShaderProgram->setProgramUniform(_playerShaderUniformLocations.mvpMatrix, mvpMtx);
    if(viewMtx == _freeCamPlayer1->getViewMatrix()) {
        _playerShaderProgram->setProgramUniform(_playerShaderUniformLocations.viewPos, _freeCamPlayer1->getPosition());
    }
    else{
        _playerShaderProgram->setProgramUniform(_playerShaderUniformLocations.viewPos, _freeCamPlayer2->getPosition());
    }
    _playerShaderProgram->setProgramUniform(_playerShaderUniformLocations.model, modelMtx);
    _player1->drawPlayer(modelMtx, viewMtx, projMtx);

    glm::mat4 modelMtx2(1.0f);
    modelMtx2 = glm::translate(modelMtx2, _player2->getPosition());
    glm::mat4 mvpMtx2 = projMtx * viewMtx * modelMtx2;

    _playerShaderProgram->setProgramUniform(_playerShaderUniformLocations.mvpMatrix, mvpMtx2);
    _playerShaderProgram->setProgramUniform(_playerShaderUniformLocations.model, modelMtx2);
    _player2->drawPlayer(modelMtx2, viewMtx, projMtx);

    //draw bullets
    /*
     */
    for(Bullet* b: _bullets){
        glm::mat4 modelMtx(1.0f);
        modelMtx = glm::translate(modelMtx, b->getPosition());
        glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
        _playerShaderProgram->setProgramUniform(_playerShaderUniformLocations.mvpMatrix, mvpMtx);
        _playerShaderProgram->setProgramUniform(_playerShaderUniformLocations.model, modelMtx);
        b->drawBullet(modelMtx, viewMtx, projMtx);
    }


}

GLuint FPEngine::_loadAndRegisterTexture(const char* FILENAME) {
    // our handle to the GPU
    GLuint textureHandle = 0;

    // enable setting to prevent image from being upside down
    stbi_set_flip_vertically_on_load(true);

    // will hold image parameters after load
    GLint imageWidth, imageHeight, imageChannels;
    // load image from file
    GLubyte* data = stbi_load( FILENAME, &imageWidth, &imageHeight, &imageChannels, 0);

    // if data was read from file
    if( data ) {
        const GLint STORAGE_TYPE = (imageChannels == 4 ? GL_RGBA : GL_RGB);

        glGenTextures(NUM_TEXTURES, &textureHandle);
        glBindTexture(GL_TEXTURE_2D, textureHandle);


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D,0,STORAGE_TYPE, imageWidth, imageHeight, 0, STORAGE_TYPE,GL_UNSIGNED_BYTE, data);

        //this line was causing me to crash -> fprintf( stdout, "[INFO]: %s texture map read in with handle %d\n", FILENAME, textureHandle);

        // release image memory from CPU - it now lives on the GPU
        stbi_image_free(data);
    } else {
        // load failed
        fprintf( stderr, "[ERROR]: Could not load texture map \"%s\"\n", FILENAME );
    }

    // return generated texture handle
    return textureHandle;
}

void FPEngine::_updateScene() {
    movePlayersAndCameras();

    //moves bullets forward, if out of bounds will delete
    moveBullets();

    //detects if bullet hit an enemy, deletes the bullet if so and decreases enemy health
    checkBulletCollisions();

    //TODO: if player has not been hit for awhile start regenerating health
    updateLights();

    time += .005f;
}

void FPEngine::movePlayersAndCameras() {
    if(_player1->isOutOfBounds()){
        _player1->setPosition(glm::vec3(_player1->getPosition().x, _player1->getPosition().y - 1, _player1->getPosition().z));
        if(_player1->getPosition().y < -50){
            _player1->playerDied();
        }
    }

    // move right
    if( _keys[GLFW_KEY_D] ) {
        _player1->incrementArms();
        _player1->moveRight();
        _player1->checkBounds(WORLD_SIZE);
    }
    // move left
    if( _keys[GLFW_KEY_A] ) {
        _player1->incrementArms();
        _player1->moveLeft();
        _player1->checkBounds(WORLD_SIZE);
    }
    // move forward
    if( _keys[GLFW_KEY_W] ) {
        _player1->incrementArms();
        _player1->moveForward();
        _player1->checkBounds(WORLD_SIZE);
    }
    // move backward
    if( _keys[GLFW_KEY_S] ) {
        _player1->incrementArms();
        _player1->moveBackward();
        _player1->checkBounds(WORLD_SIZE);
    }

    if(_keys[GLFW_KEY_E]){
        _freeCamPlayer1->rotate(1 * 0.05f,
                                0);
        _player1->setDirection(_freeCamPlayer1->getTheta());
    }
    //rotate
    if(_keys[GLFW_KEY_Q]){
        _freeCamPlayer1->rotate(-1 * 0.05f,
                                0);
        _player1->setDirection(_freeCamPlayer1->getTheta());
    }
    _freeCamPlayer1->setPosition(_player1->getPosition() + glm::vec3(0, 1.85, 0.0) + glm::vec3(.2, 0, .2) * _player1->getDirection());
    _freeCamPlayer1->recomputeOrientation();

    if(_player2->isOutOfBounds()){
        _player2->setPosition(glm::vec3(_player2->getPosition().x, _player2->getPosition().y - 1, _player2->getPosition().z));
        if(_player2->getPosition().y < -50){
            _player2->playerDied();
        }
    }

    // move right
    if( _keys[GLFW_KEY_L] ) {
        _player2->incrementArms();
        _player2->moveRight();
        _player2->checkBounds(WORLD_SIZE);
    }
    // move left
    if( _keys[GLFW_KEY_J] ) {
        _player2->incrementArms();
        _player2->moveLeft();
        _player2->checkBounds(WORLD_SIZE);
    }
    // move forward
    if( _keys[GLFW_KEY_I] ) {
        _player2->incrementArms();
        _player2->moveForward();
        _player2->checkBounds(WORLD_SIZE);
    }
    // move backward
    if( _keys[GLFW_KEY_K] ) {
        _player2->incrementArms();
        _player2->moveBackward();
        _player2->checkBounds(WORLD_SIZE);
    }
    //rotate
    if(_keys[GLFW_KEY_O]){
        _freeCamPlayer2->rotate(1 * 0.05f,
                                0);
        _player2->setDirection(_freeCamPlayer2->getTheta());
    }

    if(_keys[GLFW_KEY_U]){
        _freeCamPlayer2->rotate(-1 * 0.05f,
                                0);
        _player2->setDirection(_freeCamPlayer2->getTheta());
    }
    _freeCamPlayer2->setPosition(_player2->getPosition() + glm::vec3(0, 1.85, 0.0) + glm::vec3(.2, 0, .2) * _player2->getDirection());
    _freeCamPlayer2->recomputeOrientation();

    //shoot buttons
    if(_keys[GLFW_KEY_LEFT_SHIFT] && player1CanShoot){
        _bullets.push_back(new Bullet(
                _playerShaderProgram->getShaderProgramHandle(),
                _playerShaderUniformLocations.mvpMatrix,
                _playerShaderUniformLocations.materialColor,
                _player1->getPosition(),
                _player1->getDirection(),
                _player1->getAngle(),
                1
        ));
        player1CanShoot = false;
    }
    if(_keys[GLFW_KEY_SLASH] && player2CanShoot){
        _bullets.push_back(new Bullet(
                _playerShaderProgram->getShaderProgramHandle(),
                _playerShaderUniformLocations.mvpMatrix,
                _playerShaderUniformLocations.materialColor,
                _player2->getPosition(),
                _player2->getDirection(),
                _player2->getAngle(),
                2
        ));
        player2CanShoot = false;
    }
}


void FPEngine::run() {
    //  This is our draw loop - all rendering is done here.  We use a loop to keep the window open
    //	until the user decides to close the window and quit the program.  Without a loop, the
    //	window will display once and then the program exits.
    while( !glfwWindowShouldClose(_window) && _player1->deathCount < 3 && _player2->deathCount < 3) {	        // check if the window was instructed to be closed
        glDrawBuffer( GL_BACK );				        // work with our back frame buffer

        GLint framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize( _window, &framebufferWidth, &framebufferHeight );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// clear the current color contents and depth buffer in the window

        glm::mat4 projectionMatrix = glm::perspective( 45.0f, (GLfloat) framebufferWidth / (GLfloat) framebufferHeight, 0.001f, 1000.0f );
        glm::mat4 viewMatrix = _freeCamPlayer1->getViewMatrix();
        glViewport( 0, 0, framebufferWidth, framebufferHeight / (double)2);

        // draw everything to the window
        _renderScene(viewMatrix, projectionMatrix);
        glViewport(0, framebufferHeight / (double)2, framebufferWidth, framebufferHeight / (double)2);
        viewMatrix = _freeCamPlayer2->getViewMatrix();
        _renderScene(viewMatrix, projectionMatrix);

        _updateScene();

        glfwSwapBuffers(_window);                       // flush the OpenGL commands and make sure they get rendered!
        glfwPollEvents();				                // check for any events and signal to redraw screen
    }
    if(_player1->deathCount == 3){
        std::cout << "Player 2 Won" << std::endl;
    }
    else{
        std::cout << "Player 1 Won" << std::endl;
    }
}

//*************************************************************************************
//
// Private Helper FUnctions

void FPEngine::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // precompute the Model-View-Projection matrix on the CPU
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    // then send it to the shader on the GPU to apply to every vertex
    _groundShaderProgram->setProgramUniform(_groundShaderUniformLocations.mvpMatrix, mvpMtx);
    glBindTexture(GL_TEXTURE_2D, _texHandles[TEXTURE_ID::GROUND]);
    if(viewMtx == _freeCamPlayer1->getViewMatrix()) {
        _groundShaderProgram->setProgramUniform(_groundShaderUniformLocations.viewPos, _freeCamPlayer1->getPosition());
    }
    else{
        _groundShaderProgram->setProgramUniform(_groundShaderUniformLocations.viewPos, _freeCamPlayer2->getPosition());
    }
    _groundShaderProgram->setProgramUniform(_groundShaderUniformLocations.model, modelMtx);
}


void FPEngine::moveBullets() {
    for(Bullet* b: _bullets){
        b->moveForward();
    }
    int index = 0;
    while(index < _bullets.size()){
        if(!_bullets[index]->checkBounds(WORLD_SIZE)){
            _bullets.erase(_bullets.begin() + index);
        }
        else{
            index++;
        }
    }
}

void FPEngine::checkBulletCollisions() {
    int index = 0;
    double hitRadius = 1.0;
    while(index < _bullets.size()){
        bool hit = false;
        glm::vec3 b = _bullets[index]->getPosition();

        glm::vec3 p1 = _player1->getPosition();
        glm::vec3 p2 = _player2->getPosition();

        double d1 = sqrt(pow(b.x - p1.x, 2) + pow(b.y - p1.y, 2) + pow(b.z - p1.z, 2));
        double d2 = sqrt(pow(b.x - p2.x, 2) + pow(b.y - p2.y, 2) + pow(b.z - p2.z, 2));
        if(d1 < hitRadius && _bullets[index]->shotBy != 1){
            hit = true;
            _player1->health -= _bullets[index]->getDamage();
            if(_player1->health <= 0){
                _player1->playerDied();
            }
        }
        else if(d2 < hitRadius && _bullets[index]->shotBy != 2){
            hit = true;
            _player2->health -= _bullets[index]->getDamage();
            if(_player2->health <= 0){
                _player2->playerDied();
            }
        }
        if(!hit){
            index++;
        }
        else{
            _bullets.erase(_bullets.begin() + index);
        }
    }
}

void FPEngine::_createSkybox() {
    glGenVertexArrays(1, &skyBoxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);
    glBindVertexArray(skyBoxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    std::string facesCubemap[6] = {
            "data/right.jpg",
            "data/left.jpg",
            "data/top.jpg",
            "data/bottom.jpg",
            "data/front.jpg",
            "data/back.jpg"
    };

    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for(unsigned int i = 0; i < 6; i++){
        int width, height, nrChannels;
        unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
        if(data){
            stbi_set_flip_vertically_on_load(false);
            glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0,
                    GL_RGB,
                    width,
                    height,
                    0,
                    GL_RGB,
                    GL_UNSIGNED_BYTE,
                    data
            );
            stbi_image_free(data);
        }
        else{
            std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
            stbi_image_free(data);
        }
    }
}

void FPEngine::updateLights() {
    int health1 = _player1->health;
    int health2 = _player2->health;

    glm::vec3 lightColor1 = glm::vec3((double)(100.0 - health1) / 100.0, (double)health1 / 100.0,0);
    glm::vec3 lightPosition1 = glm::vec3(_player1->getPosition().x, _player1->getPosition().y + 5, _player1->getPosition().z);
    glm::vec3 lightColor2 = glm::vec3((double)(100.0 - health2) / 100.0, (double)health2 / 100.0,0);
    glm::vec3 lightPosition2 = glm::vec3(_player2->getPosition().x, _player2->getPosition().y + 5, _player2->getPosition().z);

    glProgramUniform3fv(_groundShaderProgram->getShaderProgramHandle(), _groundShaderUniformLocations.player1LightColor, 1, &lightColor1[0]);
    glProgramUniform3fv(_groundShaderProgram->getShaderProgramHandle(), _groundShaderUniformLocations.player1LightPosition, 1, &lightPosition1[0]);
    glProgramUniform3fv(_groundShaderProgram->getShaderProgramHandle(), _groundShaderUniformLocations.player2LightColor, 1, &lightColor2[0]);
    glProgramUniform3fv(_groundShaderProgram->getShaderProgramHandle(), _groundShaderUniformLocations.player2LightPosition, 1, &lightPosition2[0]);

    glProgramUniform3fv(_playerShaderProgram->getShaderProgramHandle(), _playerShaderUniformLocations.player1LightColor, 1, &lightColor1[0]);
    glProgramUniform3fv(_playerShaderProgram->getShaderProgramHandle(), _playerShaderUniformLocations.player1LightPosition, 1, &lightPosition1[0]);
    glProgramUniform3fv(_playerShaderProgram->getShaderProgramHandle(), _playerShaderUniformLocations.player2LightColor, 1, &lightColor2[0]);
    glProgramUniform3fv(_playerShaderProgram->getShaderProgramHandle(), _playerShaderUniformLocations.player2LightPosition, 1, &lightPosition2[0]);

    glProgramUniform3fv(_grassShaderProgram->getShaderProgramHandle(), _grassShaderUniformLocations.player1LightColor, 1, &lightColor1[0]);
    glProgramUniform3fv(_grassShaderProgram->getShaderProgramHandle(), _grassShaderUniformLocations.player1LightPosition, 1, &lightPosition1[0]);
    glProgramUniform3fv(_grassShaderProgram->getShaderProgramHandle(), _grassShaderUniformLocations.player2LightColor, 1, &lightColor2[0]);
    glProgramUniform3fv(_grassShaderProgram->getShaderProgramHandle(), _grassShaderUniformLocations.player2LightPosition, 1, &lightPosition2[0]);
}

void FPEngine::generateGrassPositions() {
    for(int i = 0; i < 500; i++){
        double x = getRand() * 2 * WORLD_SIZE - WORLD_SIZE;
        double z = getRand() * 2 * WORLD_SIZE - WORLD_SIZE;
        _grassPositions.push_back(glm::vec3(x, 0, z));
    }
}

void FPEngine::_createGrassBuffers() {
    struct Vertex {
        GLfloat x, y, z;
        GLfloat normalX, normalY, normalZ;

    };

    Vertex grassQuad[4] = {
            {0.0f, 0.0f, 0.0f, 0,0,1},
            { 0.5f, 0.0f, 0.0f,0,0,1},
            {0.0f, 0.5f,  0.0f,0,0,1},
            { 0.5f, 0.5f,  0.0f,0,0,1}
    };

    GLushort indices[4] = {0,1,2,3};

    _numGrassPoints = 4;

    glGenVertexArrays(1, &_grassVAO);
    glBindVertexArray(_grassVAO);

    GLuint vbods[2];       // 0 - VBO, 1 - IBO
    glGenBuffers(2, vbods);
    glBindBuffer(GL_ARRAY_BUFFER, vbods[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grassQuad), grassQuad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(_grassShaderAttributeLocations.vPos);
    glVertexAttribPointer(_grassShaderAttributeLocations.vPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(_grassShaderAttributeLocations.normal);
    glVertexAttribPointer(_grassShaderAttributeLocations.normal, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbods[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    _texHandles[TEXTURE_ID::GRASS] = _loadAndRegisterTexture("data/grass.png");

    generateGrassPositions();
}

//*************************************************************************************
//
// Callbacks

void a3_engine_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods ) {
    auto engine = (FPEngine*) glfwGetWindowUserPointer(window);

    // pass the key and action through to the engine
    engine->handleKeyEvent(key, action);
}

void a3_engine_cursor_callback(GLFWwindow *window, double x, double y ) {
    auto engine = (FPEngine*) glfwGetWindowUserPointer(window);

    // pass the cursor position through to the engine
    engine->handleCursorPositionEvent(glm::vec2(x, y));
}

void a3_engine_mouse_button_callback(GLFWwindow *window, int button, int action, int mods ) {
    auto engine = (FPEngine*) glfwGetWindowUserPointer(window);

    // pass the mouse button and action through to the engine
    engine->handleMouseButtonEvent(button, action);
}
