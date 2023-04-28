#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the bottom-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: (Req 1) Finish this shader.

// ==> The size of each tile is (size x size).
uniform int size = 32;

// ==> The colors of the tiles.
uniform vec3 colors[2];

void main(){
    
    // ==> get the current x and y position and divide by size to know where we are on the screen
    int position_x = int(gl_FragCoord.x / size);
    int position_y = int(gl_FragCoord.y / size);
    
    // ==> if the sum of the x and y position is odd, then the color is colors[1]
    if(mod((position_x + position_y) , 2) == 1)
        frag_color = vec4(colors[1], 1.0);
    else
        // ==> bottom left is 0,0 so the color is colors[0]
        frag_color = vec4(colors[0], 1.0);
}