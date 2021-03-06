#ifndef MESH_H
#define MESH_H

#include "../lib/glad/glad.h"
#include "../lib/glm/glm.hpp"
#include "../lib/glm/gtc/matrix_transform.hpp"

#include "shader.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

struct Vertex
{
    //position
    glm::vec3 Position;
    //normal
    glm::vec3 Normal;
    //texture coordinates
    glm::vec2 TexCoords;
    //tangent
    glm::vec3 Tangent;
    //bitangent
    glm::vec3 Bitangent;
};

struct Texture
{
    unsigned int id;
    string type;
    string path;
};

class Mesh
{
    public:
        //mesh data
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;
        unsigned int VAO;

        //functions
        
        //constructor
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
        {
            this->vertices = vertices;
            this->indices = indices;
            this->textures = textures;

            //now that we have all the required data, set the vertex buffers and its attribute pointers
            setupMesh();   
        }

        //render the mesh
        void Draw(Shader shader)
        {
            //bind the appropriate textures
            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;
            unsigned int normalNr = 1;
            unsigned int heightNr = 1;

            for (unsigned int i = 0; i < textures.size(); i++)
            {
                //active proper texture unit before binding
                glActiveTexture(GL_TEXTURE0 + i);
                
                //retrieve texture number (the N in diffuse_textureN)
                string number;
                string name = textures[i].type;

                if(name == "texture_diffuse")
                    number = std::to_string(diffuseNr++);
                else if(name == "texture_specular")
                    //transfer unsigned int to stream
                    number = std::to_string(specularNr++);
                else if(name == "texture_normal")
                    //transfer unsigned int to stream
                    number = std::to_string(normalNr++);
                else if(name == "texture_height")
                    //transfer unsigned int to stream
                    number = std::to_string(heightNr++);

                //now set the sampler to the correct texture unit
                glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
                //finally bind the texture
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
            }
            
            //draw mesh
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            //always good practice to set everything back to defaults once configured
            glActiveTexture(GL_TEXTURE0);
        }
    
    private:
        //render data
        unsigned int VBO, EBO;

        //functions

        //initialized all the buffer objects/arrays
        void setupMesh()
        {
            //create buffers/arrays
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);
            //load data into vertex buffers
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            //a great thing about structs is that their memory layout is sequential for all its items
            //the effect is that we can simply pass a pointer to the struct adn it translates perfectly to a glm::vec3/2 array which again translates to 3/2 floats which translates to a byte array
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

            //set the vertex attribute pointers
            //vertex positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            //vertex normals
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
            //vertex texture coordinates
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
            //vertex tangent
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
            //vertex bitangent
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
            
            glBindVertexArray(0);
        }
};
#endif

