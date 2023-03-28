#pragma once

#include <glad/gl.h>
#include "vertex.hpp"
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace our {

    #define ATTRIB_LOC_POSITION 0
    #define ATTRIB_LOC_COLOR    1
    #define ATTRIB_LOC_TEXCOORD 2
    #define ATTRIB_LOC_NORMAL   3

    class Mesh {
        // Here, we store the object names of the 3 main components of a mesh:
        
        // A vertex array object
        unsigned int VAO;
        // A vertex buffer 
        unsigned int VBO;
        // element buffer
        unsigned int EBO;
        
        // We need to remember the number of elements that will be draw by glDrawElements 
        GLsizei elementCount;
    public:
        
        std::string path;
        bool enabled = true;
        std::vector<glm::vec3> verts;

        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering 
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& elements)
        {
            //TODO: (Req 2) Write this function
            // remember to store the number of elements in "elementCount" since you will need it for drawing
            // For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc
            
            // 1. Create a vertex array object

            // Generate a VAO to define how to read the vertex & element buffer during rendering
            glGenVertexArrays(1, &VAO); 
            // Bind the VAO
            glBindVertexArray(VAO);     


            // 2. Create a vertex buffer object
            // The VBO is a buffer that stores the vertex data to store the them on the VRAM
            glGenBuffers(1, &VBO);                                                          
            // Bind the VBO
            glBindBuffer(GL_ARRAY_BUFFER, VBO);         
            // Copy the vertex data to the VBO
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), (void*)vertices.data(), GL_STATIC_DRAW); 
            // remember to store the number of elements in "elementCount" since you will need it for drawing
            // Store the number of elements in "elementCount" since we will need it for drawing
            elementCount = (GLsizei)elements.size(); 

            // 3.1 Position
            // Enable the attribute location for position
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);       
            //  define an array of generic vertex attribute data
            // Teach it how to read the bits inside the buffer
            // index: position_location || size: 3 (x,y,z) || 3*sizeof(float) ==> kol mara jump 3 bytes
            // false: means not normalized || sizeof(Vertex): means jump to the next vertex || (void*)0: means start from the beginning
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0); 

            // 3.2 Color
            // Enable the attribute location for color
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);          
            //  define an array of generic vertex attribute data
            // index: color_location || size: 4 (r,g,b,a) || 4*sizeof(unsigned char) ==> kol mara jump 4 bytes 
            // true: means normalized ||sizeof(Vertex): means jump to the next vertex || (void*)(offsetof(Vertex, color)): means start from the beginning of the color
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void*)(offsetof(Vertex, color))); 

            // 3.3 Texture Coordinates
            // Enable the attribute location for texture coordinates
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);  
            //  define an array of generic vertex attribute data
            // index: texcoord_location || size: 2 (u,v) || 2*sizeof(float) ==> kol mara jump 2 bytes
            // false: means not normalized || sizeof(Vertex): means jump to the next vertex || (void*)(offsetof(Vertex, tex_coord)): means start from the beginning of the tex_coord
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, false, sizeof(Vertex), (void*)(offsetof(Vertex, tex_coord))); 

            // 3.4 Normal
            // Enable the attribute location for normal
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);          
            //  define an array of generic vertex attribute data                                                          
            // index: normal_location || size: 3 (x,y,z) || 3*sizeof(float) ==> kol mara jump 3 bytes
            // false: means not normalized || sizeof(Vertex): means jump to the next vertex || (void*)(offsetof(Vertex, normal)): means start from the beginning of the normal
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, false, sizeof(Vertex), (void*)(offsetof(Vertex, normal))); 

            // 4. Create an element buffer object
            // The EBO is a buffer that stores the element data on the VRAM
            glGenBuffers(1, &EBO);                      
            // Bind the EBO                                                                           
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
            // Copy the element data to the EBO
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), (void*)elements.data(), GL_STATIC_DRAW); 

        }

        // this function should render the mesh
        void draw() 
        {
            //TODO: (Req 2) Write this function
            if(!enabled) return;
            
            // Bind the VAO
            glBindVertexArray(VAO);                  

            // Draw the mesh
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, (void*)0); 
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh(){
            //TODO: (Req 2) Write this function
            
            // Delete the VAO
            glDeleteVertexArrays(1, &VAO); 
            
            // Delete the VBO
            glDeleteBuffers(1, &VBO);      
            
            // Delete the EBO
            glDeleteBuffers(1, &EBO);      
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}