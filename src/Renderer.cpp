// This code open a window and draw a triangle; using Modern OpenGL
// based on the code of The Cherno OpenGL tutorial

#include <iostream> 
#include "Renderer.h"


/* Error Detection Macro */

/* glGetError() returns the value of the error flag.
   no other errors are recorded until glGetError() is called, 
   the error code is returned, and the flag is reset to GL_NO_ERROR */

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}
bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << ":" << line << std::endl;
        return false;
    }
    return true;
}


void Renderer::Clear() const
{
    /* glClear() clear buffers to preset values */
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    //GLCall(glClear(GL_DEPTH_BUFFER_BIT));
}

/* Draw Call */
void Renderer::Draw(const VertexArray& va, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    /* glDrawElements() render primitives from array data.
       It specifies multiple geometric primitives with very few subroutine calls. */
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
    va.Unbind();
}