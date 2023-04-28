#include "shader.hpp"

#include <cassert>
#include <iostream>
#include <fstream>
#include <string>

//Forward definition for error checking functions
std::string checkForShaderCompilationErrors(GLuint shader);
std::string checkForLinkingErrors(GLuint program);

bool our::ShaderProgram::attach(const std::string &filename, GLenum type) const {
    // Here, we open the file and read a string from it containing the GLSL code of our shader
    std::ifstream file(filename);
    if(!file){
        std::cerr << "ERROR: Couldn't open shader file: " << filename << std::endl;
        return false;
    }
    std::string sourceString = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const char* sourceCStr = sourceString.c_str();
    file.close();

    //TODO: Complete this function
    //Note: The function "checkForShaderCompilationErrors" checks if there is
    // an error in the given shader. You should use it to check if there is a
    // compilation error and print it so that you can know what is wrong with
    // the shader. The returned string will be empty if there is no errors.

    // ==> glCreateShader: create a shader object
    GLuint shader = glCreateShader(type);    
    
    // ==> glShaderSource: give it the shader object and the source code
    // ==> 1: number of shaders in the array
    // ==> &sourceCStr: pointer to the shader
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    
    // ==> glCompileShader: compile the shader
    glCompileShader(shader);
    
    // ==> checkForShaderCompilationErrors: check if there is a compilation error
    std::string compilation_check = checkForShaderCompilationErrors(shader);

    //We return true if the compilation succeeded
    if(compilation_check.size() == 0) {

        // ==> glAttachShader: attach the shader to the program
        glAttachShader(this->program, shader);

        // ==> glDeleteShader: delete the shader
        glDeleteShader(shader);

        return true;
    }
    
    std::cerr << "Error in shader.cpp in attach" << compilation_check << ", " << filename << '\n';
    return false;
    
}



bool our::ShaderProgram::link() const {
    //TODO: Complete this function
    //Note: The function "checkForLinkingErrors" checks if there is
    // an error in the given program. You should use it to check if there is a
    // linking error and print it so that you can know what is wrong with the
    // program. The returned string will be empty if there is no errors.
    
    // ==> glLinkProgram: link the program
    glLinkProgram(this->program);

    // ==> checkForLinkingErrors: check if there is a linking error
    std::string link_check = checkForLinkingErrors(this->program);
    
    if(link_check.size() > 0) {
        std::cerr << "Error in shader.cpp in link: " << link_check << '\n';
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////
// Function to check for compilation and linking error in shaders //
////////////////////////////////////////////////////////////////////

std::string checkForShaderCompilationErrors(GLuint shader){
     //Check and return any error in the compilation process
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, logStr);
        std::string errorLog(logStr);
        delete[] logStr;
        return errorLog;
    }
    return std::string();
}

std::string checkForLinkingErrors(GLuint program){
     //Check and return any error in the linking process
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetProgramInfoLog(program, length, nullptr, logStr);
        std::string error(logStr);
        delete[] logStr;
        return error;
    }
    return std::string();
}