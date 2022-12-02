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
            case GLFW_KEY_Q:
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
            default:
                break;
        }
    }
}

void FPEngine::handleMouseButtonEvent(GLint button, GLint action) {
    // if the event is for the left mouse button
    if( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        _bullets.push_back(new Bullet(
                _lightingShaderProgram->getShaderProgramHandle(),
                _lightingShaderUniformLocations.mvpMatrix,
                _lightingShaderUniformLocations.normalMatrix,
                _lightingShaderUniformLocations.materialColor,
                _player1->getPosition(),
                _player1->getDirection(),
                _player1->getAngle()
                ));
    }
}

void FPEngine::handleCursorPositionEvent(glm::vec2 currMousePosition) {
    // if mouse hasn't moved in the window, prevent camera from flipping out
    if(_mousePosition.x == MOUSE_UNINITIALIZED) {
        _mousePosition = currMousePosition;
    }

    //rotate the camera by the distance the mouse moved
    if(abs(currMousePosition.x - 320) > 10) {

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
    _lightingShaderProgram = new CSCI441::ShaderProgram("shaders/flatShader.v.glsl", "shaders/flatShader.f.glsl" );
    _lightingShaderUniformLocations.mvpMatrix = _lightingShaderProgram->getUniformLocation("mvpMatrix");
    _lightingShaderUniformLocations.materialColor = _lightingShaderProgram->getUniformLocation("materialColor");
    _lightingShaderUniformLocations.lightColor = _lightingShaderProgram->getUniformLocation("lightColor");
    _lightingShaderUniformLocations.lightDirection = _lightingShaderProgram->getUniformLocation("lightDirection");
    _lightingShaderUniformLocations.normalMatrix = _lightingShaderProgram->getUniformLocation("normalMatrix");
    _lightingShaderAttributeLocations.vPos = _lightingShaderProgram->getAttributeLocation("vPos");
    _lightingShaderAttributeLocations.vNormal = _lightingShaderProgram->getAttributeLocation("vNormal");

    _skyboxShaderProgram = new CSCI441::ShaderProgram("shaders/skybox.v.glsl", "shaders/skybox.f.glsl");
    _skyboxShaderUniformLocations.projection = _skyboxShaderProgram->getUniformLocation("projection");
    _skyboxShaderUniformLocations.view = _skyboxShaderProgram->getUniformLocation("view");
    _skyboxShaderUniformLocations.skybox = _skyboxShaderProgram->getUniformLocation("skybox");
    _skyboxShaderAttributeLocations.aPos = _skyboxShaderProgram->getAttributeLocation("aPos");
}

//initializes player and two enemies
void FPEngine::_setupBuffers() {
    CSCI441::setVertexAttributeLocations( _lightingShaderAttributeLocations.vPos, _lightingShaderAttributeLocations.vNormal);

    _player1 = new Player(_lightingShaderProgram->getShaderProgramHandle(),
                          _lightingShaderUniformLocations.mvpMatrix,
                          _lightingShaderUniformLocations.normalMatrix,
                          _lightingShaderUniformLocations.materialColor,
                          glm::vec3(25, .1, 25));

    _player2 = new Player(_lightingShaderProgram->getShaderProgramHandle(),
                          _lightingShaderUniformLocations.mvpMatrix,
                          _lightingShaderUniformLocations.normalMatrix,
                          _lightingShaderUniformLocations.materialColor,
                          glm::vec3(-25, .1, -25));

    _createGroundBuffers();
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

    glEnableVertexAttribArray(_lightingShaderAttributeLocations.vPos);
    glVertexAttribPointer(_lightingShaderAttributeLocations.vPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(_lightingShaderAttributeLocations.vNormal);
    glVertexAttribPointer(_lightingShaderAttributeLocations.vNormal, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbods[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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

    glm::vec3 lightColor = glm::vec3(1,1,1);
    glm::vec3 lightDirection = glm::vec3(-1,-1,-1);
    glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(),_lightingShaderUniformLocations.lightColor,1,&lightColor[0]);
    glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(),_lightingShaderUniformLocations.lightDirection,1,&lightDirection[0]);


}

//*************************************************************************************
//
// Engine Cleanup

void FPEngine::_cleanupShaders() {
    fprintf( stdout, "[INFO]: ...deleting Shaders.\n" );
    delete _lightingShaderProgram;
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
    // use our lighting shader program
    _lightingShaderProgram->useProgram();

    // draw the ground plane
    glm::mat4 groundModelMtx = glm::scale( glm::mat4(1.0f), glm::vec3(WORLD_SIZE, 1.0f, WORLD_SIZE));
    _computeAndSendMatrixUniforms(groundModelMtx, viewMtx, projMtx);

    glm::vec3 groundColor(0.3f, 0.8f, 0.2f);
    glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &groundColor[0]);

    glBindVertexArray(_groundVAO);
    glDrawElements(GL_TRIANGLE_STRIP, _numGroundPoints, GL_UNSIGNED_SHORT, (void*)0);

    //draw players
    glm::mat4 modelMtx(1.0f);
    modelMtx = glm::translate(modelMtx, _player1->getPosition());
    _player1->drawPlayer(modelMtx, viewMtx, projMtx);

    glm::mat4 modelMtx2(1.0f);
    modelMtx2 = glm::translate(modelMtx2, _player2->getPosition());
    _player2->drawPlayer(modelMtx2, viewMtx, projMtx);

    //draw bullets
    for(Bullet* b: _bullets){
        glm::mat4 modelMtx(1.0f);
        modelMtx = glm::translate(modelMtx, b->getPosition());
        b->drawBullet(modelMtx, viewMtx, projMtx);
    }

    //draw skybox
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
}

void FPEngine::_updateScene() {
    //WASD arrow and space inputs for moving player and camera
    movePlayersAndCameras();

    //moves bullets forward, if out of bounds will delete
    moveBullets();

    //detects if bullet hit an enemy, deletes the bullet if so and decreases enemy health
    checkBulletCollisions();

}

void FPEngine::movePlayersAndCameras() const {
    //TODO: expand to both players
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

    if(_keys[GLFW_KEY_RIGHT]){
        _freeCamPlayer1->rotate(1 * 0.05f,
                                0);
        _player1->setDirection(_freeCamPlayer1->getTheta());
    }

    if(_keys[GLFW_KEY_LEFT]){
        _freeCamPlayer1->rotate(-1 * 0.05f,
                                0);
        _player1->setDirection(_freeCamPlayer1->getTheta());
    }
    _freeCamPlayer1->setPosition(_player1->getPosition() + glm::vec3(0, 1.85, 0.0) + glm::vec3(.2, 0, .2) * _player1->getDirection());
    _freeCamPlayer1->recomputeOrientation();
}


void FPEngine::run() {
    //  This is our draw loop - all rendering is done here.  We use a loop to keep the window open
    //	until the user decides to close the window and quit the program.  Without a loop, the
    //	window will display once and then the program exits.
    while( !glfwWindowShouldClose(_window) && !_player1->isDead()) {	        // check if the window was instructed to be closed
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
}

//*************************************************************************************
//
// Private Helper FUnctions

void FPEngine::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // precompute the Model-View-Projection matrix on the CPU
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    // then send it to the shader on the GPU to apply to every vertex
    _lightingShaderProgram->setProgramUniform(_lightingShaderUniformLocations.mvpMatrix, mvpMtx);

    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMtx)));
    _lightingShaderProgram->setProgramUniform(_lightingShaderUniformLocations.normalMatrix, normalMatrix);
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
    while(index < _bullets.size()){
        bool hit = false;
        glm::vec3 b = _bullets[index]->getPosition();
        //TODO: check bullet collision with players
        if(!hit){
            index++;
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
