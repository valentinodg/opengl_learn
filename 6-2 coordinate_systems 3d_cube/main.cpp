//pre-processing
#include "../lib/glad/glad.h"
#include </usr/include/GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

#include "../lib/glm/glm.hpp"
#include "../lib/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/gtc/type_ptr.hpp"

#include "shader.hpp"

#include <iostream>
#include <cmath>

//namespaces
using namespace std;

//functions prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//macros
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

//main
int main()
{

    //glfw: initialization
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //glfw: window creation
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "gl-window", NULL, NULL);
    if(window == NULL)
    {
        cout << "failed to create glfw window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //glad: load all opengl function pointers 
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "failed to initialize GLAD" << endl;
        return -1;
    }

    //configure global opengl state
    //let's enable the depth buffer
    glEnable(GL_DEPTH_TEST);
    
    //build and compile the shader program
    Shader ourShader("6.1.coordinate_systems.vertex", "6.1.coordinate_systems.fragment");

    //vertex data & buffer(s) set up and vertex attributes configuration
    float vertices[] = {
    //positions (x,y,z)     //texture coords  
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,    1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    //texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //load and create a texture
    unsigned int texture1;
    unsigned int texture2;

    //texture1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    //set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //load image, create texture and generate mipmaps
    int width, height, nrChannels;
    //vertical flip (y-axis)
    stbi_set_flip_vertically_on_load(true);
    //texture image path
    unsigned char *data = stbi_load("./textures/container.jpg", &width, &height, &nrChannels, 0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //texture2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    //set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //load image, create texture and generate mipmaps
    data = stbi_load("./textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if(data)
    {
        //png -> alpha channel -> rgba
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //set texture units
    //let's activate the shader
    ourShader.use();
    //set via the texture class
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    //render loop
    while(!glfwWindowShouldClose(window))
    {   
        //input
        processInput(window);

        //render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //clear the depth buffer adding the GL_DEPTH_BUFFER_BIT flag to glClear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        //activate shader
        ourShader.use();
        
        //create transformations
        //first initialize matrix to indentity matrix
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        //matrices generation
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        //retrieve the matrix uniform location
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        //pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        //now we set the projection matrix each frame but since it rarely changes we can set it outside the main loop only once
        ourShader.setMat4("projection", projection);

        //render box
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        //buffers swap and io events poll
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    return 0;
}

//functions definition
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
