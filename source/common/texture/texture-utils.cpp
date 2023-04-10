#include "texture-utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <iostream>

our::Texture2D *our::texture_utils::empty(GLenum format, glm::ivec2 size)
{
    // TODO: (Req 11) Finish this function to create an empty texture with the given size and format
    our::Texture2D *texture = new our::Texture2D();
    // DONE: (Req 10) Finish this function to create an empty texture with the given size and format
    int levels = 1; // Default value --> Especially for the depth texture
    if (format != GL_DEPTH_COMPONENT24)
        levels = (int)glm::floor(glm::log2((float)glm::max(size.x, size.y))) + 1;
    texture->bind();                                               // Bind the texture to be able to use it later on.
    glTexStorage2D(GL_TEXTURE_2D, levels, format, size.x, size.y); // Assign the needed storage space.
    return texture;
}
// go ahead

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
    // This line sets the pixel storage mode, which affects how pixel data is unpacked from CPU memory into GPU memory. In this case,
    // it sets the alignment to 4 bytes per row, which is the default value and works well for most cases.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    // This line uploads the texture image data to the GPU. It specifies the target (2D texture), the level of detail (0 for base level),
    // the internal format (RGBA8, which means 8 bits per channel), the texture dimensions (size.x and size.y), the border width (0), the pixel format (RGBA),
    // the data type (unsigned byte), and a pointer to the pixel data.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    // This line generates a series of mipmaps for the texture, which are smaller versions of the texture that can be used for mipmapping filtering.
    // If the generate_mipmap flag is set to true, it will generate mipmaps for the texture.
    if (generate_mipmap)
        glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(pixels); // Free image data after uploading to GPU
    return texture;
}