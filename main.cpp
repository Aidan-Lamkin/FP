/*
 *  CSCI 441, Computer Graphics, Fall 2022
 *
 *  Project: lab04
 *  File: main.cpp
 *
 *  Description:
 *      This file contains the basic setup to work with GLSL shaders.
 *
 *  Author: Dr. Paone, Colorado School of Mines, 2022
 *
 */

#include "FPEngine.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

///*****************************************************************************
//
// Our main function
int main() {

    auto a5Engine = new FPEngine();
    a5Engine->initialize();
    if (a5Engine->getError() == CSCI441::OpenGLEngine::OPENGL_ENGINE_ERROR_NO_ERROR) {
        a5Engine->run();
    }
    a5Engine->shutdown();
    delete a5Engine;

	return EXIT_SUCCESS;
}
