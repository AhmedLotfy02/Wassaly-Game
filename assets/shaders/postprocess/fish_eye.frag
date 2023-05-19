#version 330


uniform sampler2D tex;
in vec2 tex_coord;
out vec4 frag_color;


 void main()
 {
    vec2 center = vec2(0.5f, 0.5f);
    float fish_eye_radius = 20.0f;
    float fish_eye_scale = -0.5f;

    vec2 fish_eye_tex_coord = tex_coord;
    float fish_eye_dist = distance(center, tex_coord);
    fish_eye_tex_coord -= center;
    if (fish_eye_dist < fish_eye_radius)
    {
        float percent = 1.0 + ((0.5 - fish_eye_dist) / 0.5) * fish_eye_scale;

        fish_eye_tex_coord *= percent;
    }
    fish_eye_tex_coord += center;

    frag_color = texture(tex, fish_eye_tex_coord );

 }