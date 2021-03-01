#include "Shader.h"
#include <iostream>
#include <fstream>  
#include <string>
#include <sstream>
#include "Renderer.h"

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

/* Function to read from a shader file */
ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    /*open the file*/
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    /* read line by line and find the beginning of each shader */
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                /* Set mode to vertex */
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                /* Set mode to fragment */
                type = ShaderType::FRAGMENT;
        }
        else
        {
            if (type != ShaderType::NONE)
                ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

/* Compiles and creates shader objects and returns the id */
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
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


void Shader::Bind() const
{
    /* glUseProgram() installs the [m_RendererID] program object
       as part of current rendering state to run it */
    GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
    /* glUseProgram(0) sets to [null] the current rendering state */
    GLCall(glUseProgram(0));
}

/* Set Uniforms */
/* glUniform__() Specifies the value of a uniform variable for the current program object */

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

/* GetUniformLocation() returns an integer that represents the location
   of a specific uniform variable within a program object */
unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(unsigned int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform ' " << name << " ' doesn't exist " << std::endl;
    
    m_UniformLocationCache[name] = location;
    return location; 
}