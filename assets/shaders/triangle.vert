#version 330

// This vertex shader should be used to render a triangle whose normalized device coordinates are:
// (-0.5, -0.5, 0.0), ( 0.5, -0.5, 0.0), ( 0.0,  0.5, 0.0)

// And it also should send the vertex color as a varying to the fragment shader where the colors are (in order):
// (1.0, 0.0, 0.0), (0.0, 1.0, 0.0), (0.0, 0.0, 1.0)

// ==> varyings: to send data from the vertex shader to the fragment shader
out Varyings {
    vec3 color;
} vs_out;

// Currently, the triangle is always in the same position, but we don't want that.
// So two uniforms should be added: translation (vec2) and scale (vec2).
// Each vertex "v" should be transformed to be "scale * v + translation".
// The default value for "translation" is (0.0, 0.0) and for "scale" is (1.0, 1.0).

//TODO: (Req 1) Finish this shader

// ==> uniform: to send data from the CPU to the GPU
uniform vec2 translation = vec2(0.0, 0.0);
uniform vec2 scale = vec2(1.0, 1.0);

void main(){

    // ==> positions: the vertices of the triangle
    const vec3 positions[3] = vec3[3] (
        vec3(-0.5, -0.5, 0.0),
        vec3( 0.5, -0.5, 0.0),
        vec3( 0.0,  0.5, 0.0)
    );

    // ==> colors: the colors of the vertices
    const vec3 colors[3] = vec3[3] (
        vec3(1.0, 0.0, 0.0),
        vec3(0.0, 1.0, 0.0),
        vec3(0.0, 0.0, 1.0)
    );


    // ==> scale_3d: the scale in 3D
    // ==> the homogeneous coordinate is 1.0 as we are working with 3D points so we can translate them 
    // ==> if 0.0 then it is a vector and we can't translate vectors
    vec3 scale_3d = vec3(scale, 1.0);
    // ==> translation_3d: the translation in 3D
    vec3 translation_3d = vec3(translation, 1.0);
 
    // new_position is the new position of the vertex   
    vec3 new_position = (positions[gl_VertexID] * scale_3d) + translation_3d;

    gl_Position = vec4(new_position, 1.0);

    // ==> gl_VertexID: the index of the current vertex
    vs_out.color = colors[gl_VertexID];

}