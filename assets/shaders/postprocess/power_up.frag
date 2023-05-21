#version 330
uniform sampler2D tex;

in vec2 tex_coord;
out vec4 frag_color;

const float saturation = 1.5; // The saturation level of the power-up effect
const float brightness = 0.2; // The brightness level of the power-up effect
// Functions for color space conversions (rgb to hsv and hsv to rgb)

vec3 rgbToHsv(vec3 rgb) {
    float cMax = max(max(rgb.r, rgb.g), rgb.b);
    float cMin = min(min(rgb.r, rgb.g), rgb.b);
    float delta = cMax - cMin;

    float hue = 0.0;
    if (delta > 0.0) {
        if (cMax == rgb.r) {
            hue = mod((rgb.g - rgb.b) / delta, 6.0);
        } else if (cMax == rgb.g) {
            hue = ((rgb.b - rgb.r) / delta) + 2.0;
        } else {
            hue = ((rgb.r - rgb.g) / delta) + 4.0;
        }
        hue /= 6.0;
    }

    float saturation = (cMax > 0.0) ? (delta / cMax) : 0.0;
    float value = cMax;

    return vec3(hue, saturation, value);
}

vec3 hsvToRgb(vec3 hsv) {
    float c = hsv.y * hsv.z;
    float x = c * (1.0 - abs(mod((hsv.x * 6.0), 2.0) - 1.0));
    float m = hsv.z - c;

    vec3 rgb = vec3(0.0);
    if (hsv.x < 1.0 / 6.0) {
        rgb = vec3(c, x, 0.0);
    } else if (hsv.x < 2.0 / 6.0) {
        rgb = vec3(x, c, 0.0);
    } else if (hsv.x < 3.0 / 6.0) {
        rgb = vec3(0.0, c, x);
    } else if (hsv.x < 4.0 / 6.0) {
        rgb = vec3(0.0, x, c);
    } else if (hsv.x < 5.0 / 6.0) {
        rgb = vec3(x, 0.0, c);
    } else {
        rgb = vec3(c, 0.0, x);
    }

    return rgb + m;
}


void main() {
    // Sample the color from the original texture
    vec4 color = texture(tex, tex_coord);

    // Apply the power-up effect by adjusting the saturation and brightness
    vec3 hsv = rgbToHsv(color.rgb);
    hsv.y *= saturation; // Adjust saturation
    hsv.z += brightness; // Adjust brightness
    color.rgb = hsvToRgb(hsv);

    // Output the final color
    frag_color = color;
}

