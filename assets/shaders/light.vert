#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;

// Now we need to the surface normal to compute the light so we will send it as an attribute.
layout(location = 3) in vec3 normal;

// We will need to do the light processing in the world space so we will break our transformations into 2 stages:
// 1- Object to World.
uniform mat4 object_to_world; // M
uniform mat4 object_to_world_inv_transpose; // M_IT // The inverse transpose will be used to transform the surface normal.

// 2- World to Homogenous Clipspace.
uniform mat4 view_projection; // VP

// The camera position will be used for specular computation.
uniform vec3 camera_position; // eye

out Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} vsout;


void main() {
    // First we compute the world position.
    vsout.world = (object_to_world * vec4(position, 1.0f)).xyz;

    // Then we compute the view vector (vertex to eye vector in the world space) to be used for specular computation later.
    vsout.view = normalize(camera_position - vsout.world);

    // Then we compute normal in the world space (Note that w=0 since this is a vector).
    vsout.normal = normalize((object_to_world_inv_transpose * vec4(normal, 0.0f)).xyz);

    // Finally, we compute the position in the homogenous clip space and send the rest of the data.
    gl_Position = view_projection * vec4(vsout.world, 1.0);
    vsout.color = color;
    vsout.tex_coord = tex_coord;
}