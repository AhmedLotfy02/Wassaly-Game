#include "texture-utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <iostream>

our::Texture2D *our::texture_utils::empty(GLenum format, glm::ivec2 size)
{
    // TODO: (Req 11) Finish this function to create an empty texture with the given size and format
    our::Texture2D *texture = new our::Texture2D();
    texture->bind();

    if (format == GL_RGBA8)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size[0], size[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    else if (format == GL_DEPTH_COMPONENT24)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, size[0], size[1], 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

    return texture;
}

our::Texture2D *our::texture_utils::loadImage(const std::string &filename, bool generate_mipmap)
{
    glm::ivec2 size;
    int channels;
    // Since OpenGL puts the texture origin at the bottom left while images typically has the origin at the top left,
    // We need to till stb to flip images vertically after loading them
    stbi_set_flip_vertically_on_load(true);
    // Load image data and retrieve width, height and number of channels in the image
    // The last argument is the number of channels we want and it can have the following values:
    //- 0: Keep number of channels the same as in the image file
    //- 1: Grayscale only
    //- 2: Grayscale and Alpha
    //- 3: RGB
    //- 4: RGB and Alpha (RGBA)
    // Note: channels (the 4th argument) always returns the original number of channels in the file
    unsigned char *pixels = stbi_load(filename.c_str(), &size.x, &size.y, &channels, 4);
    if (pixels == nullptr)
    {
        std::cerr << "Failed to load image: " << filename << std::endl;
        return nullptr;
    }
    // Create a texture
    our::Texture2D *texture = new our::Texture2D();

    // Bind the texture such that we upload the image data to its storage
    texture->bind();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    /***************************************************************************************************************
      'glPixelStorei() is a function in OpenGL that sets pixel storage modes.

     -- The function takes two parameters:
         1) 'GL_UNPACK_ALIGNMENT, specifies the parameter being set. In this case, it is the alignment requirement for unpacking pixel data from memory.
         2) '4', sets the alignment requirement to 4 bytes.

     -- This means that when pixel data is being read from memory and unpacked for use in OpenGL, it will be aligned on a 4-byte boundary.
        This can improve performance by allowing the data to be read more efficiently by the graphics hardware.
     ****************************************************************************************************************/

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    /***************************************************************************************************************
      'glTexImage2D' is a function that specifys a two-dimensional texture image.

     -- The function parameters are:
        1) GL_TEXTURE_2D: Specifies the target texture. In this case, it is a two-dimensional texture.
        2) 0: Specifies the level-of-detail number. Level 0 is the base image level.
        3) GL_RGBA8: Specifies the internal format of the texture. In this case, it is RGBA with 8 bits per channel.
        4) size.x and size.y: Specifies the width and height of the texture image in pixels.
        5) 0: Specifies the border width. Must be 0.
        6) GL_RGBA: Specifies the format of the pixel data. In this case, it is RGBA.
        7) GL_UNSIGNED_BYTE: Specifies the data type of the pixel data. In this case, it is unsigned bytes.
        8) pixels: A pointer to the image data in memory.
    ****************************************************************************************************************/

    if (generate_mipmap)
        glGenerateMipmap(GL_TEXTURE_2D);
    /***************************************************************************************************************
      'glGenerateMipmap' is a function in the OpenGL graphics library that automatically generates a complete set of mipmaps for a specified texture object.

     -- The function takes one parameter:
        1) 'GL_TEXTURE_2D' is the texture target for which to generate the mipmaps. In this case, we are generating mipmaps for a 2D texture.

     -- Therefore, the glGenerateMipmap(GL_TEXTURE_2D) call generates a complete set of mipmaps for the currently bound 2D texture.
    ****************************************************************************************************************/
    stbi_image_free(pixels); // Free image data after uploading to GPU
    return texture;
}