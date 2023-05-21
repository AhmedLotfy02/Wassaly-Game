#version 330

uniform sampler2D tex;

in vec2 tex_coord;
out vec4 frag_color;

const float blur_size = 0.005; // The size of the blur

void main() {
    vec4 color = vec4(0.0);

    // Apply a horizontal blur
    for (int i = -4; i <= 4; i++) {
        color += texture(tex, vec2(tex_coord.x + float(i) * blur_size, tex_coord.y)) / 9.0;
    }

    // Apply a vertical blur
    for (int i = -4; i <= 4; i++) {
        color += texture(tex, vec2(tex_coord.x, tex_coord.y + float(i) * blur_size)) / 9.0;
    }

    frag_color = color;
}
