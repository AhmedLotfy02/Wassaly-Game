#version 330

uniform sampler2D tex;

in vec2 tex_coord;
out vec4 frag_color;

void main() {

    vec4 color = texture(tex, tex_coord);
    frag_color = vec4(color.rgb * 0.5, color.a);
}