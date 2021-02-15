/* STARTS LIBRARY DECLARATION */

// Glew library to access Modern OpenGL functions
#include <GL/glew.h> 
// GLFW library to get the windowing creation and code management implementation
#include <GLFW/glfw3.h>  
// Iostream library to print possible error messages
#include <iostream>  

/* ENDS LIBRARY DECLARATION */

/*Compiles and creates shader objects and returns the id*/
static unsigned int CompileShader(unsigned int type, const std::string& source) {

    /* glCreateShader() creates an empty shader object and
       returns a non-zero [id] value by which it can be referenced */
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    /* glShaderSource() replaces the source code from the [id] shader object
       by the source code in the array of strings specified by [src]*/
    glShaderSource(id, 1, &src, nullptr);
    /* Here glCompileShader() compiles the source code strings
       that have been stored in the [id] shader object */
    glCompileShader(id);
    int result;
    /* Here glGetShaderiv() returns the compile status from the [id] shader */
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        /* Here glGetShaderiv() returns the length of the information log for the [id] shader */
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length * sizeof(char));
        /* glGetShaderInfoLog() returns the information log for the [id] shader */
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        /* Here glDeleteShader() frees the memory
           and invalidates the name associated with the [id] shader object*/
        glDeleteShader(id);
        return 0;
    }
    return id;
}

/*Creates a program containg vertex and fragment shader*/
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

    /* glCreateProgram() creates an empty program object
       and returns a non-zero value by which it can be referenced */
    unsigned int program = glCreateProgram();
    // Here CompileShader() ...
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    // Here CompileShader() ...
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

    /* Here glDeleteShader() frees the memory
       and invalidates the name associated with the [vs] shader object*/
    glDeleteShader(vs);
    /* Here glDeleteShader() frees the memory
       and invalidates the name associated with the [fs] shader object*/
    glDeleteShader(fs);

    return program;
}

/*Main function*/
int main(void)
{
    GLFWwindow* window;

    /* Initialize GLFW library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "MyOpenGL vacl", NULL, NULL);
    if (!window)
    {
        /* glfwTerminate() Destroys all remaining windows and cursors,
           restores any modified gamma ramps and frees
           any other allocated resources */
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current,
       A valid OpenGL context is created
       from now GLEW can be initialized */
    glfwMakeContextCurrent(window);


    /* Initializing GLEW
       This allow us to use Modern OpenGL*/
    if (glewInit() != GLEW_OK) {
        std::cout << "Error" << std::endl;
        exit(1);
    }

    /*Positions array*/
    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    /*---------STARTS DEFINITION OF VERTEX BUFFER---------*/

    unsigned int buffer;

    /* glGenBuffers() generates a buffer object name */
    glGenBuffers(1, &buffer);

    /* glBindBuffer() binds the [buffer] name buffer object
       to the [GL_ARRAY_BUFFER] buffer binding point */
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    /* glBufferData() creates and initializes a new buffer object's data store */
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    /* glVertexAttribPointer() defines an array of generic vertex attribute data
       specifying the location and data format of the array of generic vertex attributes
       at index [0] to use when rendering.*/
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    /* glEnableVertexAttribArray() enables the generic vertex attribute array
       specified by the index [0]*/
    glEnableVertexAttribArray(0);

    /*---------ENDS DEFINITION OF VERTEX BUFFER---------*/

    /*GLSL vertex shader code*/
    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 positions;"
        "void main()"
        "{\n"
        "   gl_Position = positions;\n"
        "}\n";

    /*GLSL fragment shader code*/
    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;"
        "void main()"
        "{\n"
        "   color = vec4(0.0, 0.0, 1.0, 1.0);\n"
        "}\n";

    /* Creates a program with the vertex and fragment shaders
       that contain the source code defined for each of them right above*/
    unsigned int shader = CreateShader(vertexShader, fragmentShader);

    /* glUseProgram() installs the [shader] program object
       as part of current rendering state */
    glUseProgram(shader);

    /* Loop until the user closes the window to render continusely */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Drawing a Triangle with OpenGL Legacy
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f,-0.5f);
        glVertex2f( 0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();
           -------------------------------------*/

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