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
                                 640, 480,
                                 "A5: The Battle of Five Armies") {

    for(auto& _key : _keys) _key = GL_FALSE;

    _mousePosition = glm::vec2(MOUSE_UNINITIALIZED, MOUSE_UNINITIALIZED );
}

FPEngine::~FPEngine() {
    delete _freeCam;
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
                _player->resetArms();
                return;
            case GLFW_KEY_W:
                _player->resetArms();
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
                _player->getPosition(),
                _player->getDirection(),
                _player->getAngle()
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

    _player = new Player(_lightingShaderProgram->getShaderProgramHandle(),
                       _lightingShaderUniformLocations.mvpMatrix,
                       _lightingShaderUniformLocations.normalMatrix,
                       _lightingShaderUniformLocations.materialColor);

    Enemy* firstEnemy = new Enemy(_lightingShaderProgram->getShaderProgramHandle(),
                                 _lightingShaderUniformLocations.mvpMatrix,
                                 _lightingShaderUniformLocations.normalMatrix,
                                 _lightingShaderUniformLocations.materialColor,
                                 glm::vec3(20,.1,20),
                                 _health,
                                 _movementSpeed);

    Enemy* secondEnemy = new Enemy(_lightingShaderProgram->getShaderProgramHandle(),
                                   _lightingShaderUniformLocations.mvpMatrix,
                                   _lightingShaderUniformLocations.normalMatrix,
                                   _lightingShaderUniformLocations.materialColor,
                                   glm::vec3(-20,.1,-20),
                                   _health,
                                   _movementSpeed);
    _enemies.push_back(firstEnemy);
    _enemies.push_back(secondEnemy);

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

    _freeCam = new CSCI441::FreeCam();
    _freeCam->setPosition( _player->getPosition() + glm::vec3(0,1.85,0.2));
    _freeCam->setTheta( M_PI);
    _freeCam->setPhi( M_PI_2);
    _freeCam->recomputeOrientation();

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
    delete _player;
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

    //draw player
    glm::mat4 modelMtx(1.0f);
    modelMtx = glm::translate( modelMtx, _player->getPosition());
    _player->drawPlayer(modelMtx, viewMtx, projMtx);

    //draw enemies
    for(Enemy* e: _enemies){
        glm::mat4 modelMtx(1.0f);
        modelMtx = glm::translate(modelMtx, e->getPosition());
        e->drawEnemy(modelMtx, viewMtx, projMtx);
    }
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
    movePlayerAndCamera();

    //updates enemy direction and moves forward
    moveEnemies();

    //moves bullets forward, if out of bounds will delete
    moveBullets();

    //detects if bullet hit an enemy, deletes the bullet if so and decreases enemy health
    checkBulletCollisions();

    //check to see if enemy hit player, if so player dies
    checkEnemyPlayerCollisions();

    //checks to see if enemy hit enemy and they bounce off each other
    checkEnemyCollisions();

    //if enemy dies, spawn new one in with increased health and movement speed
    checkEnemyDeaths();
}

void FPEngine::movePlayerAndCamera() const {
    if(_player->isOutOfBounds()){
        _player->setPosition(glm::vec3(_player->getPosition().x, _player->getPosition().y - 1, _player->getPosition().z));
        if(_player->getPosition().y < -50){
            _player->playerDied();
        }
    }

    // move right
    if( _keys[GLFW_KEY_D] ) {
        _player->incrementArms();
        _player->moveRight();
        _player->checkBounds(WORLD_SIZE);
    }
    // move left
    if( _keys[GLFW_KEY_A] ) {
        _player->incrementArms();
        _player->moveLeft();
        _player->checkBounds(WORLD_SIZE);
    }
    // move forward
    if( _keys[GLFW_KEY_W] ) {
        _player->incrementArms();
        _player->moveForward();
        _player->checkBounds(WORLD_SIZE);
    }
    // move backward
    if( _keys[GLFW_KEY_S] ) {
        _player->incrementArms();
        _player->moveBackward();
        _player->checkBounds(WORLD_SIZE);
    }

    //jump
    if( _keys[GLFW_KEY_SPACE]){
        if(!_player->isJump()){
            _player->setJump(true);
        }
    }

    if(_keys[GLFW_KEY_RIGHT]){
        _freeCam->rotate(1 * 0.05f,
                         0);
        _player->setDirection(_freeCam->getTheta());
    }

    if(_keys[GLFW_KEY_LEFT]){
        _freeCam->rotate(-1 * 0.05f,
                         0);
        _player->setDirection(_freeCam->getTheta());
    }

    if(_player->isJump()){
        _freeCam->setPosition(
                _player->getPosition() + glm::vec3(0, 1.85, 0.0) + glm::vec3(0, _player->getCurrentJumpHeight(), 0) + glm::vec3(.2, 0, .2) * _player->getDirection());
    }
    else {
        _freeCam->setPosition(_player->getPosition() + glm::vec3(0, 1.85, 0.0) + glm::vec3(.2, 0, .2) * _player->getDirection());
    }
    _freeCam->recomputeOrientation();
}

void FPEngine::moveEnemies() const {
    glm::vec3 playerPosition = _player->getPosition();
    for(Enemy* e: _enemies){
        e->incrementArms();
        glm::vec3 enemyToPlayer = glm::normalize(e->getPosition() - playerPosition );
        if(e->getDirection().z < enemyToPlayer.z){
            e->newDirection(glm::normalize(glm::vec3(e->getDirection().x,0,e->getDirection().z + .08)), _freeCam->getTheta());
        }
        if(e->getDirection().z > enemyToPlayer.z){
            e->newDirection(glm::normalize(glm::vec3(e->getDirection().x,0,e->getDirection().z - .08)), _freeCam->getTheta());
        }
        if(e->getDirection().x < enemyToPlayer.x){
            e->newDirection(glm::normalize(glm::vec3(e->getDirection().x + .08,0,e->getDirection().z)), _freeCam->getTheta());
        }
        if(e->getDirection().x > enemyToPlayer.x){
            e->newDirection(glm::normalize(glm::vec3(e->getDirection().x - .08,0,e->getDirection().z)), _freeCam->getTheta());
        }

        e->moveForward();
        e->checkBounds(WORLD_SIZE);
        if(e->isOutOfBounds()){
            e->setPosition(glm::vec3(e->getPosition().x, e->getPosition().y - 1, e->getPosition().z));
            if(e->getPosition().y < -50){
                e->enemyDied();
            }
        }
    }
}

void FPEngine::checkEnemyDeaths(){
    int index = 0;
    glm::vec3 p = _player->getPosition();
    while(index < _enemies.size()){
        if(_enemies[index]->isDead()){
            _enemies.erase(_enemies.begin() + index);
            _numberEliminations++;
            _health += 10;
            _movementSpeed += .02f;
            GLfloat x = getRand() * 180 - 90;
            GLfloat z = getRand() * 180 - 90;
            while(sqrt(pow(p.x-x,2) + pow(p.z-z,2)) < 30){
                x = getRand() * 180 - 90;
                z = getRand() * 180 - 90;
            }
            _enemies.push_back(new Enemy(_lightingShaderProgram->getShaderProgramHandle(),
                               _lightingShaderUniformLocations.mvpMatrix,
                               _lightingShaderUniformLocations.normalMatrix,
                               _lightingShaderUniformLocations.materialColor,
                               glm::vec3(x,.1,z),
                               _health,
                               _movementSpeed));
        }
        else{
            index++;
        }
    }
}

void FPEngine::run() {
    //  This is our draw loop - all rendering is done here.  We use a loop to keep the window open
    //	until the user decides to close the window and quit the program.  Without a loop, the
    //	window will display once and then the program exits.
    while( !glfwWindowShouldClose(_window) && !_player->isDead()) {	        // check if the window was instructed to be closed
        glDrawBuffer( GL_BACK );				        // work with our back frame buffer
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// clear the current color contents and depth buffer in the window

        // Get the size of our framebuffer.  Ideally this should be the same dimensions as our window, but
        // when using a Retina display the actual window can be larger than the requested window.  Therefore,
        // query what the actual size of the window we are rendering to is.
        GLint framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize( _window, &framebufferWidth, &framebufferHeight );

        // update the viewport - tell OpenGL we want to render to the whole window
        glViewport( 0, 0, framebufferWidth, framebufferHeight );

        // set the projection matrix based on the window size
        // use a perspective projection that ranges
        // with a FOV of 45 degrees, for our current aspect ratio, and Z ranges from [0.001, 1000].
        glm::mat4 projectionMatrix = glm::perspective( 45.0f, (GLfloat) framebufferWidth / (GLfloat) framebufferHeight, 0.001f, 1000.0f );

        // set up our look at matrix to position our camera
        glm::mat4 viewMatrix = _freeCam->getViewMatrix();

        // draw everything to the window
        _renderScene(viewMatrix, projectionMatrix);

        _updateScene();

        glfwSwapBuffers(_window);                       // flush the OpenGL commands and make sure they get rendered!
        glfwPollEvents();				                // check for any events and signal to redraw screen
    }
    //display how many enemies killed
    std::cout << "Thanks for playing, you eliminated: " << _numberEliminations << " enemies" << std::endl;
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

void FPEngine::checkEnemyPlayerCollisions() {
    glm::vec3 p = _player->getPosition();
    for(Enemy* enemy: _enemies){
        glm::vec3 e = enemy->getPosition();
        GLfloat d = sqrt(pow(p.x - e.x, 2) + pow(p.y-e.y,2) + pow(p.z - e.z,2));
        if(d < .5){
            _player->playerDied();
        }
    }
}

void FPEngine::checkEnemyCollisions() {
    for(int i = 0; i < _enemies.size(); i++){
        glm::vec3 e1 = _enemies[i]->getPosition();
        for (int j = 0; j < _enemies.size(); j++) {
            if(i != j) {
                glm::vec3 e2 = _enemies[j]->getPosition();
                GLfloat d = sqrt(pow(e1.x - e2.x, 2) + pow(e1.y - e2.y, 2) + pow(e1.z - e2.z, 2));
                if (d < 1) {
                    _enemies[i]->moveBackward();
                    _enemies[j]->moveBackward();

                    glm::vec3 ni = glm::normalize(e1 - e2);
                    glm::vec3 nj = glm::normalize(e2 - e1);

                    glm::vec3 newDirection = dot( _enemies[i]->getDirection() , ni ) * ni;
                    newDirection = glm::vec3(newDirection.x * 2, newDirection.y * 2, newDirection.z * 2);
                    newDirection = _enemies[i]->getDirection() - newDirection;

                    _enemies[i]->newDirection(newDirection, _freeCam->getTheta());

                    newDirection = dot( _enemies[j]->getDirection() , nj ) * nj;
                    newDirection = glm::vec3(newDirection.x * 2, newDirection.y * 2, newDirection.z * 2);
                    newDirection = _enemies[j]->getDirection() - newDirection;

                    _enemies[j]->newDirection(newDirection, _freeCam->getTheta());

                    _enemies[i]->moveForward();
                    _enemies[j]->moveForward();
                }
            }
        }
    }
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
        for(int j = 0; j < _enemies.size(); j++){
            glm::vec3 e = _enemies[j]->getPosition();
            GLfloat d = sqrt(pow(b.x-e.x,2) + pow(b.y-e.y,2) + pow(b.z-e.z,2));
            if(d < 1){
                GLfloat damage = _bullets[index]->getDamage();
                _bullets.erase(_bullets.begin() + index);
                hit = true;
                _enemies[j]->decreaseHealth(damage);
                break;
            }
        }
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
