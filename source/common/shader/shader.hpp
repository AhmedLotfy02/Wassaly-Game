#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {

    class ShaderProgram {

    private:
        //Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram(){
            //TODO: (Req 1) Create A shader program
            program = glCreateProgram();
        }
        ~ShaderProgram(){
            //TODO: (Req 1) Delete a shader program
            glDeleteProgram(program);
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() { 
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name) {
            //TODO: (Req 1) Return the location of the uniform with the given name
            
            // ==> glGetUniformLocation: Returns the location of the uniform variable name in the program object program
            // for example: time = glGetUniformLocation(program, "time");
            return glGetUniformLocation(program, &name[0]);
        }

        void set(const std::string &uniform, GLfloat value) {
            //TODO: (Req 1) Send the given float value to the given uniform
            
            // ==> glUniform1f: give it the location and the value (1 float) that it will sent to the uniform
            // ==> uniform: to send from CPU to GPU
            glUniform1f(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLuint value) {
            //TODO: (Req 1) Send the given unsigned integer value to the given uniform
            
            // ==> glUniform1ui: give it the location and the value (1 unsigned integer) that it will sent to the uniform
            // ==> uniform: to send from CPU to GPU
            glUniform1ui(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLint value) {
            //TODO: (Req 1) Send the given integer value to the given uniform
            
            // ==> glUniform1i: give it the location and the value (1 integer) that it will sent to the uniform
            // ==> uniform: to send from CPU to GPU
            glUniform1i(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, glm::vec2 value) {
            //TODO: (Req 1) Send the given 2D vector value to the given uniform

            // ==> glUniform2f: give it the location and the value (2 floats ==> x and y) that it will sent to the uniform
            // ==> uniform: to send from CPU to GPU
            glUniform2f(getUniformLocation(uniform), value.x, value.y);
        }

        void set(const std::string &uniform, glm::vec3 value) {
            //TODO: (Req 1) Send the given 3D vector value to the given uniform
            
            // ==> glUniform3f: give it the location and the value (3 floats ==> x, y, and z) that it will sent to the uniform
            // ==> uniform: to send from CPU to GPU
            glUniform3f(getUniformLocation(uniform), value.x, value.y, value.z);
        }

        void set(const std::string &uniform, glm::vec4 value) {
            //TODO: (Req 1) Send the given 4D vector value to the given uniform
            
            // ==> glUniform4f: give it the location and the value (4 floats ==> x, y,z, and w) that it will sent to the uniform
            // ==> uniform: to send from CPU to GPU
            glUniform4f(getUniformLocation(uniform), value.x, value.y, value.z, value.w);
        }

        void set(const std::string &uniform, glm::mat4 matrix) {
            //TODO: (Req 1) Send the given matrix 4x4 value to the given uniform

            // ==> glUniformMatrix4fv: give it the location and the value ( one 4x4 matrix) that it will sent to the uniform
            // ==> uniform: to send from CPU to GPU
            
            // parameters: 
            // ==> getUniformLocation(uniform): the location of the uniform
            // ==> count: the number of matrices to be sent (1)
            // ==> false: to transpose the matrix or not
            // ==> &matrix[0][0]: the first element of the matrix
            glUniformMatrix4fv(getUniformLocation(uniform), 1, false,  &matrix[0][0]);
        }

        //TODO: (Req 1) Delete the copy constructor and assignment operator.
        //Question: Why do we delete the copy constructor and assignment operator?

        // To achieve Resource Acquisition Is Initialization (prevent other instances from accessing the same program, which might be deleted)
        // Resources are acquired during initialization, when there is no chance of them being used before they are available, and released with the destruction of the same objects, which is guaranteed to take place even in case of errors.
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

    };

}

#endif