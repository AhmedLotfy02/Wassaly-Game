#version 330


uniform sampler2D tex;
in vec2 tex_coord;
out vec4 frag_color;


 void main()
 {
    // the center of the fish eye effect
    vec2 center = vec2(0.5f, 0.5f);
    // the radius of the fish eye effect
    float fish_eye_radius = 20.0f;
    // the scale of the fish eye effect
    float fish_eye_scale = -0.5f;

    // the distance from the center of the fish eye effect to the current tex coord being processed 
    vec2 fish_eye_tex_coord = tex_coord;
    float fish_eye_dist = distance(center, tex_coord);
    fish_eye_tex_coord -= center;

    // if the distance is less than the radius, apply the fish eye effect
    if (fish_eye_dist < fish_eye_radius)
    {
        float percent = 1.0 + ((0.5 - fish_eye_dist) / 0.5) * fish_eye_scale;

        fish_eye_tex_coord *= percent;
    }
    // add the center back to the fish eye tex coord to get the correct texture coordinate
    fish_eye_tex_coord += center;

    // sample the texture
    frag_color = texture(tex, fish_eye_tex_coord );

 }