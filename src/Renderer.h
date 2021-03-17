#pragma once

// This code open a window and draw a triangle; using Modern OpenGL
// based on the code of The Cherno OpenGL tutorial

/* Library declaration */
// Glew library to access Modern OpenGL functions
#include <GL/glew.h> 

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

/* STARTS ERROR DETECTION MACRO AND FUNCTIONS */
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
/* ENDS ERROR DETECTION MACRO AND FUNCTIONS */

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const Shader& shader) const;

};