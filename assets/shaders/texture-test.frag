#version 330 core

in Varyings {
    vec3 position;
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
} fs_in;

out vec4 frag_color;

uniform sampler2D tex;

void main(){
    //TODO: (Req 5) Change the following line to read the fragment color
    // from the texture at the received texture coordinates
    frag_color = texture(tex,fs_in.tex_coord);

    /***************************************************************************************************************
      'texture' is a built-in function in GLSL that retrieves the color data of a texture at a specified texture coordinate.

     -- The function takes two parameters:
         1) 'tex' is the texture sampler object to sample from.
         2) 'fs_in.tex_coord' is the texture coordinate vector that specifies the location in the texture to sample from.

     -- Therefore, the texture(tex, fs_in.tex_coord) call retrieves the color data of the texture bound to the sampler object tex
        at the texture coordinate fs_in.tex_coord.
         
     -- This function is typically used in shader programs to fetch the color data of a texture and use it to calculate the color of a pixel or a fragment.
     ****************************************************************************************************************/
}