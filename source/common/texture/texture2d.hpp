#pragma once

#include <glad/gl.h>

namespace our
{

    // This class defined an OpenGL texture which will be used as a GL_TEXTURE_2D
    class Texture2D
    {
       // The OpenGL object name of this texture
        GLuint name = 0;

    public:
        // This constructor creates an OpenGL texture and saves its object name in the member variable "name"
        Texture2D()
        {
            // TODO: (Req 5) Complete this function

            glGenTextures(1, &name);
            /***************************************************************************************************************
             glGenTextures is a function that generates a specified number of texture names and returns them in an array.

            -- The function takes two parameters:
                1) 1 is the number of texture names to generate. In this case, we are generating only one texture name.
                2) &name is a pointer to a variable that will store the generated texture name.

            -- Therefore, the glGenTextures(1, &name) call generates one texture name and stores it in the memory location pointed to by the name variable.
            ****************************************************************************************************************/
        };

        // This deconstructor deletes the underlying OpenGL texture
        ~Texture2D()
        {
            // TODO: (Req 5) Complete this function
            glDeleteTextures(1, &name);
            /***************************************************************************************************************
             glDeleteTextures is a function that deletes a specified number of texture names.

             -- The function takes two parameters:
                1) 1  is the number of texture names to delete. In this case, we are deleting only one texture name.
                2) &name is a pointer to a variable that stores the texture name to be deleted.

             -- Therefore, the glDeleteTextures(1, &name) call deletes the texture with the name stored in the memory location pointed to by the name variable.
            ****************************************************************************************************************/
        }

        // Get the internal OpenGL name of the texture which is useful for use with framebuffers
        GLuint getOpenGLName()
        {
            return name;
        }

        // This method binds this texture to GL_TEXTURE_2D
        void bind() const
        {
            // TODO: (Req 5) Complete this function
            glBindTexture(GL_TEXTURE_2D, name);
            /***************************************************************************************************************
             glBindTexture is a function that binds a named texture to a texture target.

             -- The function takes two parameters:
                1) 'GL_TEXTURE_2D' is the texture target to which the texture is bound. In this case, we are binding the texture to a 2D texture target.
                2) is the name of the texture to be bound. This is the same name that was generated using 'glGenTextures' and stored in the name variable.

             -- Therefore, the glBindTexture(GL_TEXTURE_2D, name) call binds the texture with the name stored in the name variable to the 2D texture target.
            ****************************************************************************************************************/
        }

        // This static method ensures that no texture is bound to GL_TEXTURE_2D
        static void unbind()
        {
            // TODO: (Req 5) Complete this function
            glBindTexture(GL_TEXTURE_2D, 0);
            /********************************************************************************************************************
             The function glBindTexture(GL_TEXTURE_2D, 0) binds the texture object with ID 0 to the current context's GL_TEXTURE_2D target.
             This effectively unbinds any previously bound texture object from the GL_TEXTURE_2D target.
             In other words, it sets the active texture to be the default texture (which is a single white pixel) for the GL_TEXTURE_2D target.
            *********************************************************************************************************************/
                }

        Texture2D(const Texture2D &) = delete;
        Texture2D &operator=(const Texture2D &) = delete;
    };

}