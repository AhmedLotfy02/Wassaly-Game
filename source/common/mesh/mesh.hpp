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

            // ==> Generate an ID for a vertex array object corresponding to vertex data
            // ==> A Vertex Array Object (VAO) stores all of the state needed to supply vertex data. It stores the format of the vertex data as well as the Buffer Objects providing the vertex data arrays.
            glGenVertexArrays(1, &VAO); 

            // ==> Bind the vertex array object
            // ==> When a Vertex Array Object is bound, the previous binding is automatically broken.
            glBindVertexArray(VAO);     


            // 2. Create a vertex buffer object

            // ==> A vertex buffer object (VBO) provides methods for uploading vertex data (position, color, etc.) to the video device for non-immediate-mode rendering.
            glGenBuffers(1, &VBO);                                                          
            
            // ==> Bind the VBO
            // ==> When a Vertex Buffer Object is bound, the previous binding is automatically broken.
            glBindBuffer(GL_ARRAY_BUFFER, VBO);         

            // Copy the vertex data to the VBO
            // ==> glBufferData() creates a new data store for the buffer object currently bound to target. Any pre-existing data store is deleted.
            // parameters:
            //  ==> target: Specifies the target buffer object.
            //  ==> size: Specifies the size in bytes of the buffer object's new data store.
            //  ==> data: Specifies a pointer to data that will be copied into the data store for initialization, or NULL if no data is to be copied.
            //  ==> usage: Specifies the expected usage pattern of the data store.
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), (void*)vertices.data(), GL_STATIC_DRAW); 
            
            // remember to store the number of elements in "elementCount" since you will need it for drawing
            
            // ==> Store the number of elements in "elementCount" since we will need it for drawing
            elementCount = (GLsizei)elements.size(); 

            // 3.1 Position
            
            // ==> Enable the attribute location for position
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);       
            
            // ==> define an array of generic vertex attribute data
            // parameters:
            //  ==> index: Specifies the index of the generic vertex attribute to be modified
            //  ==> size: Specifies the number of components per generic vertex attribute ==> 3 (x,y,z) 
            //  ==> type: Specifies the data type of each component in the array ==> GL_FLOAT
            //  ==> normalized: Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
            //  ==> stride: Specifies the byte offset between consecutive generic vertex attributes. ==> sizeof(Vertex): means jump to the next vertex
            //  ==> pointer: Specifies a offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target. ==> (void*)0: means start from the beginning of the position
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0); 

            // 3.2 Color

            // Enable the attribute location for color
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);          

            // define an array of generic vertex attribute data
            // parameters:
            //  ==> index: Specifies the index of the generic vertex attribute to be modified
            //  ==> size: 4 (r,g,b,a)
            //  ==> type: GL_UNSIGNED_BYTE
            //  ==> normalized: true
            //  ==> stride: sizeof(Vertex): means jump to the next vertex
            //  ==> pointer: (void*)(offsetof(Vertex, color)): means start from the beginning of the color
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void*)(offsetof(Vertex, color))); 

            // 3.3 Texture Coordinates

            // Enable the attribute location for texture coordinates
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);  
            
            //  define an array of generic vertex attribute data
            // parameters:
            //  ==> index: Specifies the index of the generic vertex attribute to be modified
            //  ==> size: 2 (u,v)
            //  ==> type: GL_FLOAT
            //  ==> normalized: false
            //  ==> stride: sizeof(Vertex): means jump to the next vertex
            //  ==> pointer: (void*)(offsetof(Vertex, tex_coord)): means start from the beginning of the texture coordinates
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, false, sizeof(Vertex), (void*)(offsetof(Vertex, tex_coord))); 

            // 3.4 Normal

            // Enable the attribute location for normal
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);          

            //  define an array of generic vertex attribute data
            // parameters:
            //  ==> index: Specifies the index of the generic vertex attribute to be modified
            //  ==> size: 3 (x,y,z)
            //  ==> type: GL_FLOAT
            //  ==> normalized: false
            //  ==> stride: sizeof(Vertex): means jump to the next vertex
            //  ==> pointer: (void*)(offsetof(Vertex, normal)): means start from the beginning of the normal
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, false, sizeof(Vertex), (void*)(offsetof(Vertex, normal))); 

            // 4. Create an element buffer object

            // An EBO is a buffer, just like a vertex buffer object, that stores indices that OpenGL uses to decide what vertices to draw.
            glGenBuffers(1, &EBO);                      
            // Bind the EBO                                                                           
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
            // glBufferData: create a new data store for a buffer object
            // parameters:
            //  ==> target: Specifies the target buffer object.
            //  ==> size: Specifies the size in bytes of the buffer object's new data store.
            //  ==> data: Specifies a pointer to data that will be copied into the data store for initialization, or NULL if no data is to be copied.
            //  ==> usage: Specifies the expected usage pattern of the data store ==> GL_STATIC_DRAW: the data store contents will be specified once by the application, and used many times as the source for GL drawing commands.
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), (void*)elements.data(), GL_STATIC_DRAW); 

        }

        // this function should render the mesh
        void draw() 
        {
            //TODO: (Req 2) Write this function
            if(!enabled) return;
            
            // Bind the VAO
            glBindVertexArray(VAO);                  

            // ==> render the mesh
            // ==> When glDrawElements is called, it uses count sequential elements from an enabled array,
            // ==> starting at indices to construct a sequence of geometric primitives.
            // ==> mode specifies what kind of primitives are constructed and how the array elements construct these primitives.
            
            // parameters:
            //  ==> mode: Specifies what kind of primitives to render. ==> GL_TRIANGLES: Treats each triplet of vertices as an independent triangle.
            //  ==> count: Specifies the number of elements to be rendered.
            //  ==> type: Specifies the type of the values in indices. ==> GL_UNSIGNED_INT: Specifies the type of the values in indices.
            //  ==> indices: Specifies a pointer to the location where the indices are stored.
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