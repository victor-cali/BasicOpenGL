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
    /* Here glEnable() enables depth */
    glEnable(GL_DEPTH_TEST);

    std::cout << glGetString(GL_VERSION) << std::endl;
    {

        /* Square positions along the texture positions to be mapped */
        GLfloat positions[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };

        /*
        unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3 
        };
        */

        /* Defines how openGL is going to blend alpha */
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));  //src alpha = 0; dest = 1 - 0 = 0

        VertexArray va;
        VertexBuffer vb(positions, sizeof(positions)); // expand the buffer to 4 elements per vertex

        VertexBufferLayout layout;
        layout.Push<float>(3);  // Texture positions size
        layout.Push<float>(2);  // Texture positions size
        va.AddBuffer(vb, layout);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

        va.Unbind();
        vb.Unbind();    //GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        shader.UnBind();  //GLCall(glUseProgram(0));

        Renderer renderer;

        /* to create the animation of color change */
        float r = 0.0f;
        float incrementC = 0.03f;
        /* to create the animation of horizontal movement */
        float x = 0.0f;
        float incrementX = 0.03f;
        /* to create the animation of Scalement */
        float s = 1.0f;
        float incrementS = 0.03f;

        glm::mat4 projection;
        projection = glm::perspective(45.0f, (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);;

        /* Loop until the user closes the window, to render continusely */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();  //GLCall(glClear(GL_COLOR_BUFFER_BIT));

            /* Transformations */
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 size;
            model = glm::rotate(model, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.5f, 1.0f, 0.0f));
            view = glm::translate(view, glm::vec3(x, 0.0f, -3.0f));
            size = glm::scale(size, glm::vec3(s, s, s));
            glm::mat4 transformations = projection * view * model* size;

            // use the shader and bind the buffer and ibo each time in case that the buffer change
            shader.Bind();   //GLCall(glUseProgram(shader));
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);   //GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

            shader.SetUniformMat4f("transformations", transformations);

            Texture texture("res/textures/rainbow.png");
            texture.Bind();
            shader.SetUniform1i("u_Texture", 0);  //the slot is 0

            renderer.Draw(va, shader);

            /* Color change animation */
            if (r > 1.0f)
                incrementC = -0.05f;
            else if (r < 0.0f)
                incrementC =  0.05f;
            r += incrementC;
            /* Horizontal movement animation */
            if (x > 1.0f)
                incrementX = -0.03f;
            else if (x < -1.0f)
                incrementX =  0.03f;
            x += incrementX;
            /* Horizontal movement animation */
            if (s > 1.0f)
                incrementS = -0.01f;
            else if (s < 0.5f)
                incrementS =  0.01f;
            s += incrementS;

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