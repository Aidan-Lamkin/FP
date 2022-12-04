#ifndef LAB05_LAB05_ENGINE_HPP
#define LAB05_LAB05_ENGINE_HPP

#include <CSCI441/FreeCam.hpp>
#include <CSCI441/OpenGLEngine.hpp>
#include <CSCI441/ShaderProgram.hpp>

#include "Player.hpp"
#include "Enemy.h"
#include "Bullet.h"

#include <vector>

class FPEngine : public CSCI441::OpenGLEngine {
public:
    FPEngine();
    ~FPEngine();

    void run() final;

    /// \desc handle any key events inside the engine
    /// \param key key as represented by GLFW_KEY_ macros
    /// \param action key event action as represented by GLFW_ macros
    void handleKeyEvent(GLint key, GLint action);

    /// \desc handle any mouse button events inside the engine
    /// \param button mouse button as represented by GLFW_MOUSE_BUTTON_ macros
    /// \param action mouse event as represented by GLFW_ macros
    void handleMouseButtonEvent(GLint button, GLint action);

    /// \desc handle any cursor movement events inside the engine
    /// \param currMousePosition the current cursor position
    void handleCursorPositionEvent(glm::vec2 currMousePosition);

    /// \desc value off-screen to represent mouse has not begun interacting with window yet
    static constexpr GLfloat MOUSE_UNINITIALIZED = -9999.0f;

private:
    void _setupGLFW() final;
    void _setupOpenGL() final;
    void _setupShaders() final;
    void _setupBuffers() final;
    void _setupScene() final;

    void _cleanupBuffers() final;
    void _cleanupShaders() final;

    /// \desc draws everything to the scene from a particular point of view
    /// \param viewMtx the current view matrix for our camera
    /// \param projMtx the current projection matrix for our camera
    void _renderScene(glm::mat4 viewMtx, glm::mat4 projMtx) const;
    /// \desc handles moving our FreeCam as determined by keyboard input
    void _updateScene();

    /// \desc tracks the number of different keys that can be present as determined by GLFW
    static constexpr GLuint NUM_KEYS = GLFW_KEY_LAST;
    /// \desc boolean array tracking each key state.  if true, then the key is in a pressed or held
    /// down state.  if false, then the key is in a released state and not being interacted with
    GLboolean _keys[NUM_KEYS];

    /// \desc last location of the mouse in window coordinates
    glm::vec2 _mousePosition;

    CSCI441::FreeCam* _freeCamPlayer1;
    CSCI441::FreeCam* _freeCamPlayer2;


    Player* _player1;
    Player* _player2;

    std::vector<Bullet*> _bullets = std::vector<Bullet*>();

    /// \desc the size of the world (controls the ground size and locations of buildings)
    static constexpr GLfloat WORLD_SIZE = 100.0f;
    /// \desc VAO for our ground
    GLuint _groundVAO;
    /// \desc the number of points that make up our ground object
    GLsizei _numGroundPoints;

    /// \desc creates the ground VAO
    void _createGroundBuffers();
    GLuint _grassVAO;
    GLsizei _numGrassPoints;
    void _createGrassBuffers();

    float time = 0;

    bool player1CanShoot = true;
    bool player2CanShoot = true;

    /// \desc shader program that performs lighting
    CSCI441::ShaderProgram* _groundShaderProgram = nullptr;   // the wrapper for our shader program
    /// \desc stores the locations of all of our shader uniforms
    struct groundShaderUniformLocations {
        GLint mvpMatrix;
        GLint model;
        GLint player1LightColor;
        GLint player1LightPosition;
        GLint player2LightColor;
        GLint player2LightPosition;
        GLint groundTexture;
        GLint viewPos;
    } _groundShaderUniformLocations;

    /// \desc stores the locations of all of our shader attributes
    struct groundShaderAttributeLocations {
        /// \desc vertex position location
        GLint vPos;
        GLint normal;
    } _groundShaderAttributeLocations;

    /// \desc shader program that performs lighting
    CSCI441::ShaderProgram* _grassShaderProgram = nullptr;   // the wrapper for our shader program
    /// \desc stores the locations of all of our shader uniforms
    struct grassShaderUniformLocations {
        GLint mvpMatrix;
        GLint model;
        GLint player1LightColor;
        GLint player1LightPosition;
        GLint player2LightColor;
        GLint player2LightPosition;
        GLint grassTexture;
        GLint viewPos;
        GLint time;
    } _grassShaderUniformLocations;

    /// \desc stores the locations of all of our shader attributes
    struct grassShaderAttributeLocations {
        /// \desc vertex position location
        GLint vPos;
        GLint normal;
    } _grassShaderAttributeLocations;

    std::vector<glm::vec3> _grassPositions;

    void generateGrassPositions();

    /// \desc shader program that performs lighting
    CSCI441::ShaderProgram* _playerShaderProgram = nullptr;   // the wrapper for our shader program
    /// \desc stores the locations of all of our shader uniforms
    struct playerShaderUniformLocations {
        GLint mvpMatrix;
        GLint model;
        GLint player1LightColor;
        GLint player1LightPosition;
        GLint player2LightColor;
        GLint player2LightPosition;
        GLint viewPos;
        GLint materialColor;
    } _playerShaderUniformLocations;

    /// \desc stores the locations of all of our shader attributes
    struct playerShaderAttributeLocations {
        /// \desc vertex position location
        GLint vPos;
        GLint normal;
    } _playerShaderAttributeLocations;



    unsigned int skyBoxVAO, skyboxVBO, skyboxEBO;
    unsigned int cubemapTexture;

    CSCI441::ShaderProgram* _skyboxShaderProgram = nullptr;
    struct SkyboxShaderUniformLocations{
        GLint projection;
        GLint view;
        GLint skybox;

    }_skyboxShaderUniformLocations;
    struct SkyboxShaderAttributeLocations{
        GLint aPos;
    }_skyboxShaderAttributeLocations;


    /// \desc precomputes the matrix uniforms CPU-side and then sends them
    /// to the GPU to be used in the shader for each vertex.  It is more efficient
    /// to calculate these once and then use the resultant product in the shader.
    /// \param modelMtx model transformation matrix
    /// \param viewMtx camera view matrix
    /// \param projMtx camera projection matrix
    void _computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;

    GLuint _loadAndRegisterTexture(const char* FILENAME);

    /// \desc total number of textures in our scene
    static constexpr GLuint NUM_TEXTURES = 2;
    /// \desc used to index through our texture array to give named access
    enum TEXTURE_ID {
        /// \desc metal texture
        GROUND = 0,
        GRASS = 1
    };
    /// \desc texture handles for our textures
    GLuint _texHandles[NUM_TEXTURES];

    //refactored methods for update scene
    void movePlayersAndCameras();
    void moveBullets();
    void checkBulletCollisions();
    void updateLights();

    //skybox method
    void _createSkybox();
};

void a3_engine_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods );
void a3_engine_cursor_callback(GLFWwindow *window, double x, double y );
void a3_engine_mouse_button_callback(GLFWwindow *window, int button, int action, int mods );

#endif // LAB05_LAB05_ENGINE_HPP
