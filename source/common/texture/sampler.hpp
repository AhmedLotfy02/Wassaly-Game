#pragma once

#include <glad/gl.h>
#include <json/json.hpp>
#include <glm/vec4.hpp>

namespace our
{

    // This class defined an OpenGL sampler
    class Sampler
    {
        // The OpenGL object name of this sampler
        GLuint name;

    public:
        // This constructor creates an OpenGL sampler and saves its object name in the member variable "name"
        Sampler()
        {
            // TODO: (Req 6) Complete this function
            glGenSamplers(1, &name);
            /***************************************************************************************************************
             'glGenSamplers()' is a function that generates a specified number of sampler objects.
              A sampler object is used to control the way in which textures are sampled, such as specifying how they are filtered and wrapped.

            -- The function takes two parameters:
               1) '1': specifies the number of sampler objects to be generated. In this case, only one sampler object will be generated.
               2) '&name': a pointer to a GLuint variable that will receive the name of the first sampler object that is generated.
           ****************************************************************************************************************/
        };

        // This deconstructor deletes the underlying OpenGL sampler
        ~Sampler()
        {
            // TODO: (Req 6) Complete this function
            glDeleteSamplers(1, &name);
            /***************************************************************************************************************
             'glDeleteSamplers' is a function that deletes a specified number of sampler objects from memory.
              A sampler object is used to control the way in which textures are sampled, such as specifying how they are filtered and wrapped.

             -- The function takes two parameters:
               1) '1': specifies the number of sampler objects to be deleted. In this case, only one sampler object will be deleted.
               2) '&name': a pointer to a GLuint variable that will receive the name of the first sampler object that is generated.
            ***************************************************************************************************************/
        }

        // This method binds this sampler to the given texture unit
        void bind(GLuint textureUnit) const
        {
            // TODO: (Req 6) Complete this function
            glBindSampler(textureUnit, name);
            /***************************************************************************************************************
             'glBindSampler' is a function that binds a sampler object to a texture unit.

              -- The function takes two parameters:
                 1) 'textureUnit' specifies the texture unit to which the sampler object will be bound.
                                  In this case, it is set to textureUnit, which is an integer value that specifies the texture unit index.
                                  Texture units are used in OpenGL to specify which texture will be used by a particular draw call,
                                  and each texture unit has its own set of texture state, including a bound sampler object.

                 2) 'name': a GLuint variable that contains the name of the sampler object to be bound to the specified texture unit.
            ***************************************************************************************************************/
        }

        // This static method ensures that no sampler is bound to the given texture unit
        static void unbind(GLuint textureUnit)
        {
            // TODO: (Req 6) Complete this function
            glBindSampler(textureUnit, NULL);
            /***************************************************************************************************************
              -'glBindSampler' is a function that binds a sampler object to a texture unit. allowing you to specify the way textures are sampled when they are used in rendering.
              - In this case, the second parameter is NULL, which means that no sampler object is being bound to the specified texture unit.
              - When a sampler object is not bound to a texture unit, the default sampler object is used, which applies default values to texture
                sampling parameters such as filtering and wrapping modes. 
            ***************************************************************************************************************/
        }

        // This function sets a sampler paramter where the value is of type "GLint"
        // This can be used to set the filtering and wrapping parameters
        void set(GLenum parameter, GLint value) const
        {
            // TODO: (Req 6) Complete this function
            glSamplerParameteri(name, parameter, value);
            /***************************************************************************************************************
              'glSamplerParameteri' is a function that sets a parameter value for a sampler object.

               -- The function takes three parameters:
                  1) 'name' the name of the sampler object for which the parameter value will be set.
                  2) 'parameter' an integer value that specifies the parameter to be set.
                  3) 'value' the value to be set for the specified parameter. The type of value required depends on the parameter being set.

               -- Example:
                  if you wanted to set the filtering mode of a sampler object named mySampler to linear, you could call the function as follows:
                  glSamplerParameteri(mySampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                  In this case, GL_TEXTURE_MIN_FILTER is a constant that specifies the parameter being set,
                  and GL_LINEAR is the value to which the parameter is being set.
                  This would cause textures that use mySampler to be sampled with linear filtering, which produces smoother results than the default nearest-neighbor filtering. 
            ***************************************************************************************************************/
        }

        // This function sets a sampler paramter where the value is of type "GLfloat"
        // This can be used to set the "GL_TEXTURE_MAX_ANISOTROPY_EXT" parameter
        void set(GLenum parameter, GLfloat value) const
            // TODO: (Req 6) Complete this function
        {
            glSamplerParameterf(name, parameter, value);
            /***************************************************************************************************************
              'glSamplerParameteri' is a function that sets a floating-point parameter value for a sampler object.

               -- The function takes three parameters:
                  1) 'name' the name of the sampler object for which the parameter value will be set.
                  2) 'parameter' an integer value that specifies the parameter to be set.
                  3) 'value' the floating-point value to be set for the specified parameter.

               -- Example:
                  if you wanted to set the anisotropic filtering level of a sampler object named mySampler to 4.0, you could call the function as follows:
                  glSamplerParameterf(mySampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0);
                  In this case, GL_TEXTURE_MAX_ANISOTROPY_EXT is a constant that specifies the anisotropic filtering parameter being set,
                   and 4.0 is the value to which the parameter is being set.
            ***************************************************************************************************************/
        }

        // This function sets a sampler paramter where the value is of type "GLfloat[4]"
        // This can be used to set the "GL_TEXTURE_BORDER_COLOR" parameter
        void set(GLenum parameter, glm::vec4 value) const
        {
            glSamplerParameterfv(name, parameter, &(value.r));
            /***************************************************************************************************************
             'glSamplerParameteri' is a function that sets an array of floating-point parameter values for a sampler object.

             -- The function takes three parameters:
                1) 'name' the name of the sampler object for which the parameter value will be set.
                2) 'parameter' an integer value that specifies the parameter to be set.
                3) '&(value.r)' a pointer to a floating-point value or an array of floating-point values to be set for the specified parameter. The number and type of values required depend on the parameter being set.
            ***************************************************************************************************************/
        }

        // Given a json object, this function deserializes the sampler state
        void deserialize(const nlohmann::json &data);

        Sampler(const Sampler &) = delete;
        Sampler &operator=(const Sampler &) = delete;
    };

}