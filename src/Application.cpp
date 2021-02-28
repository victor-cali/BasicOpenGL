// This code open a window and draw a triangle; using Modern OpenGL
// based on the code of The Cherno OpenGL tutorial

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
/* ENDS LIBRARY DECLARATION */


/* STARTS ERROR DETECTION MACRO AND FUNCTIONS */
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << ":" << line << std::endl;
        return false;
    }
    return true;
}
/* ENDS ERROR DETECTION MACRO AND FUNCTIONS */


/* STARTS FUNCTION TO READ SHADER FILE */
struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};
static ShaderProgramSource ParseShader(const std::string& filepath) {
    /*open the file*/
    std::ifstream stream(filepath);
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::stringstream ss[2];
    std::string line;
    ShaderType type = ShaderType::NONE;
    /* read line by line and find the beginning of each shader */
    while (getline(stream, line)) {
        if (line.find("shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                /* Set mode to vertex */
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                /* Set mode to fragment */
                type = ShaderType::FRAGMENT;
        }
        else {
            if (type != ShaderType::NONE)
                ss[(int)type] << line << '\n';
        }
    }
    return  { ss[0].str(),ss[1].str() };
}
/* ENDS FUNCTION TO READ SHADER FILE */


/* Compiles and creates shader objects and returns the id */
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    /* glCreateShader() creates an empty shader object and
       returns a non-zero [id] value by which it can be referenced */
    unsigned int id = glCreateShader(type);
    /* pointer to the begining of the array as source[0] */
    const char* src = source.c_str();
    /* glShaderSource() replaces the source code from the [id] shader object
       by the source code in the array of strings specified by [src] */
    glShaderSource(id, 1, &src, nullptr);
    /* Here glCompileShader() compiles the source code strings
       that have been stored in the [id] shader object */
    glCompileShader(id);
    /* Starts error handling */
    int result;
    /* Here glGetShaderiv() returns the compile status from the [id] shader */
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        /* Here glGetShaderiv() returns the length of the information log for the [id] shader */
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        /* To identify the error */
        char* message = (char*)malloc(length * sizeof(char));
        /* glGetShaderInfoLog() returns the information log for the [id] shader */
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
        std::cout << message << std::endl;
        /* Here glDeleteShader() frees the memory
           and invalidates the name associated with the [id] shader object*/
        glDeleteShader(id);
        return 0;
    }
    /* Ends error handling */
    return id;
}

/*Creates a program containg vertex and fragment shader*/
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    /* glCreateProgram() creates an empty program object for the shader,
       it goes to CompileShader() and back here, it returns
       a non-zero value by which it can be referenced */
    unsigned int program = glCreateProgram();
    // Here CompileShader() compiles the vertex shader and returns its id
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    // Here CompileShader() compiles the fragment shader and returns its id
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    /* Here glAttachShader() binds the [vs] shader object to the [program] program object */
    glAttachShader(program, vs);
    /* Here glAttachShader() binds the [fs] shader object to the [program] program object */
    glAttachShader(program, fs);
    /* glLinkProgram() links the [program] program
       using the binded shader to build an executable for the GPU */
    glLinkProgram(program);
    /* glValidateProgram() validates a program object,
       checks if the executables contained can execute */
    glValidateProgram(program);

    /* Here glDeleteShader() frees the memory and invalidates the name associated
       with the [vs] shader object to detach the intermediates*/
    glDeleteShader(vs);
    /* Here glDeleteShader() frees the memory and invalidates the name associated
       with the [fs] shader object to detach the intermediates*/
    glDeleteShader(fs);

    return program;
}

/* MAIN FUNCTION */
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Setting OpenGL context profile */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* glfwSwapInterval() synchronizes the animation */
    glfwSwapInterval(1);

    /* 1 */
    //glewInit();
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    /*end 1*/

    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };


    /* STARTS DEFINITION OF VERTEX ARRAY */
    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));
    /* ENDS DEFINITION OF VERTEX ARRAY */


    /* STARTS DEFINITION OF VERTEX BUFFER */

    /* the vertex buffer object id - always unsigned int */
    unsigned int buffer;
    /* glGenBuffers() generates a buffer object name
       pointing to the vertex buffer*/
    GLCall(glGenBuffers(1, &buffer));
    /* glBindBuffer() binds the [buffer] name buffer object
       to the [GL_ARRAY_BUFFER] buffer binding point */
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    /* glBufferData() creates and initializes a new buffer object's data store */
    GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));
    /* glEnableVertexAttribArray() enables the generic vertex attribute array
       specified by the index [0]*/
    GLCall(glEnableVertexAttribArray(0));
    /* glVertexAttribPointer() defines an array of generic vertex attribute data
       specifying the location and data format of the array of generic vertex attributes
       at index [0] to use when rendering.*/
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    /* ENDS DEFINITION OF VERTEX BUFFER */


    /* STARTS DEFINITION OF INDEX BUFFER */

    /* the index buffer object id - always unsigned int */
    unsigned int ibo;
    /* glGenBuffers() generates a buffer object name
       pointing to the Index Buffer */
    GLCall(glGenBuffers(1, &ibo));
    /* glBindBuffer() binds the [ibo] name buffer object
       to the [GL_ELEMENT_ARRAY_BUFFER] buffer binding point */
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    /* glBufferData() creates and initializes a new buffer object's data store */
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    /* ENDS DEFINITION OF INDEX BUFFER */

    /* Gets shader source code from a file */
    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
    /* Creates a program with the vertex and fragment shaders
       that contain the source code defined for each of them right above*/
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    /* glUseProgram() installs the [shader] program object
       as part of current rendering state to run it */
    GLCall(glUseProgram(shader));

    /* STARTS DEFINITION OF UNIFORM */
    /* glGetUniformLocation() specifies the uniform location */
    int location = glGetUniformLocation(shader, "u_Color");
    /* In case uniform could not be find */
    ASSERT(location != -1);
    /* glUniform4f uses the uniform in c++ code */
    GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));
    /* ENDS DEFINITION OF UNIFORM */


    /* STARTS UNBINDING */
    GLCall(glBindVertexArray(0));
    GLCall(glUseProgram(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    /* ENDS UNBINDING */


    /* to create the animation that changes the color */
    float r = 0.0f;
    float increment = 0.05f;

    /* Loop until the user closes the window, to render continusely */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        /* STARTS BINDING */
        GLCall(glUseProgram(shader));
        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
        GLCall(glBindVertexArray(vao));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        /* ENDS BINDING */

        /* Draw call */
        //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr)); //ERROR
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

    /* glDeleteProgram() frees the memory and invalidates the name
       associated with the [shader] program object */
    glDeleteProgram(shader);

    /* glfwTerminate() Destroys all remaining windows and cursors,
       restores any modified gamma ramps and frees
       any other allocated resources */
    glfwTerminate();

    return 0;
}