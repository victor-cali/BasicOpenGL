/*This code uses Modern OpenGL to render geometry,
    and uses an abstraction of the VertexBuffer, VertexArray, and Shader.
  The code is based on The Cherno OpenGL tutorial in https://youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
*/

/* STARTS LIBRARY DECLARATION */
// Glew library to access Modern OpenGL functions
#include <GL/glew.h> 
// GLFW library to get the windowing creation and code management implementation
#include <GLFW/glfw3.h>  
// Standard Input/Output Streams Library, used to print possible error messages
#include <iostream>  
// Input/output stream class to operate on files, used to manage the shaders' files
#include <fstream>
// This header introduces string types, character traits and a set of converting functions
#include <string>
// Header providing string stream classes,
#include <sstream>
// Classes abstraction header files of the program
#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
// OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
/* ENDS LIBRARY DECLARATION */




/* Window Dimensions */
const GLint WIDTH = 800, HEIGHT = 600;


/* MAIN FUNCTION */
int main(void)
{
    /* Create a GLFWwindow object for GLFW's functions */
    GLFWwindow* window;
    /* Initialize GLFW library */
    if (!glfwInit())
        return -1;
    /* Setting OpenGL context profile to create the vertex array */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    /* Create a windowed mode windowand its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Report 3 OpenGL VACL 162328", nullptr, nullptr);
    /*glfwGetFramebufferSize() gets the actual width
      of the screen window itself, relative to the density of the screen*/
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    /* Chechks if window was correctly created*/
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    /* glfwSwapInterval() synchronizes the animation */
    glfwSwapInterval(1);
    /* Set GLEW to modern approach usage*/
    glewExperimental = GL_TRUE;
    //glewInit();
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;
    /* Define viewpoint dimensions */
    glViewport(0, 0, screenWidth, screenHeight);

    std::cout << glGetString(GL_VERSION) << std::endl;
    {

        /* Square positions along the texture positions to be mapped */
        GLfloat positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,      // 0 the bottom left
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,      // 1 the bottom right side
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,      // 1 the top right
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f       // 2 the top left
        };

        unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3 
        };

        /* Defines how openGL is going to blend alpha */
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));  //src alpha = 0; dest = 1 - 0 = 0

        VertexArray va;
        VertexBuffer vb(positions, sizeof(positions)); // expand the buffer to 4 elements per vertex

        VertexBufferLayout layout;
        layout.Push<float>(3);  // Texture positions size
        layout.Push<float>(2);  // Texture positions size
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

        Texture texture("res/textures/gold.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);  //the slot is 0

        va.Unbind();
        vb.Unbind();    //GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        ib.Unbind();    //GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        shader.UnBind();  //GLCall(glUseProgram(0));

        Renderer renderer;

        /* to create the animation that changes the color */
        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window, to render continusely */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();  //GLCall(glClear(GL_COLOR_BUFFER_BIT));

            // use the shader and bind the buffer and ibo each time in case that the buffer change
            shader.Bind();   //GLCall(glUseProgram(shader));

            /* Create transformations */
            glm::mat4 transform;
            transform = glm::rotate(transform, (GLfloat)glfwGetTime() * -1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
            /* Get matrix's uniform location and set matrix */
            GLint transformLocation = glGetUniformLocation(shader.m_RendererID,"transform");
            glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));


            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);   //GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

            renderer.Draw(va, ib, shader);

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            /* Part of the animation */
            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    /* glfwTerminate() Destroys all remaining windows and cursors,
       restores any modified gamma ramps and frees
       any other allocated resources */
    glfwTerminate();
    return 0;
}