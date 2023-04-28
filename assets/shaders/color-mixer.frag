#version 330 core

// This shader is designed to work with "triangle.vert" and it receives an
// interpolated varying which represents the vertex color.

in Varyings {
    vec3 color;
} fs_in;

// The fragment shader outputs a color for each pixel it processes.
out vec4 frag_color;

// We will use 3 uniforms to mix the color channels around.
// uniform: to send data from the CPU to the GPU.

uniform vec4 red= vec4(1.0,0.0,0.0,0.0);
uniform vec4 green= vec4(0.0,1.0,0.0,0.0);
uniform vec4 blue= vec4(0.0,0.0,1.0,0.0);

// currently the shader just returns the interpalated color varying.
// However, we want to mix the color channels around. We can do this using a 
// color matrix which we will send to the shader as 3 uniforms: red, green, blue.
// Each of these 3 variables will be a vec4. To apply the channel mixing for a
// certain channel (e.g. red), we apply this linear transformation:
// frag_color.r = red.r * fs_in.color.r + red.g * fs_in.color.g + red.b * fs_in.color.b + red.a;
// However, this line is too long to write, so we can simplify it using a dot product
// (which is defined in the "dot" function).

//TODO: (Req 1) Finish this shader and apply the channel mixing using the "dot" function.

void main(){
    // ==> make the color of the fragment equal to the color of the vertex in 3D space.
    vec4 fs_in_color_v4 = vec4(fs_in.color, 1.0);
    // ==> apply the channel mixing using the "dot" function for R,G,B.
    frag_color[0] = dot(red, fs_in_color_v4);
    frag_color[1] = dot(green, fs_in_color_v4);
    frag_color[2] = dot(blue, fs_in_color_v4);
    
    // ==> set the alpha channel to 1.0.
    frag_color[3] = 1.0;
}