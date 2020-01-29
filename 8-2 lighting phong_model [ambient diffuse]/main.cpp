//pre-processing
#include "../lib/glad/glad.h"
#include </usr/include/GLFW/glfw3.h>

#include "../lib/glm/glm.hpp"
#include "../lib/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/gtc/type_ptr.hpp"

#include "shader.hpp"
#include "camera.hpp"

#include <iostream>

//namespaces
using namespace std;

//functions prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

//macros
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

//timing
//deltatime -> time between current frame and last frame
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //tell glfw to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    Shader lightingShader("8.1.lighting-colors.vertex", "8.1.lighting-colors.fragment");
    Shader lampShader("8.1.lighting-lamp.vertex", "8.1.lighting-lamp.fragment");

    //vertex data & buffer(s) set up and vertex attributes configuration
    float vertices[] = {
        //positions (x,y,z)       //normal vectors (perpendicular to the surface of a vertex) 
        -0.5f, -0.5f, -0.5f,      0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,      0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,      0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,      0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,      0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,      0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,      0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,      0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,      0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,      0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,      0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,      0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f,      -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,      -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,      -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,      -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,      -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,      -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,      1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,      1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,      1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,      1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,      1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,      1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,      0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,      0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,      0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,      0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,      0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,      0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,      0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,      0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,      0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,      0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,      0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,      0.0f,  1.0f,  0.0f
    };

    //let's configure the cube's VAO and VBO
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
  
    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    //normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //let's configure the light's VAO
    //VBO stays the same, the vertices are the same for the light object which is also a 3d cube
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    //we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    //render loop
    while(!glfwWindowShouldClose(window))
    {   
        //per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        //input
        processInput(window);

        //render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //clear the depth buffer adding the GL_DEPTH_BUFFER_BIT flag to glClear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //activate shader
        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);

        //view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        
        //world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        //render the cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //let's also draw the lamp object
        lampShader.use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        //smaller cube
        model = glm::scale(model, glm::vec3(0.2f));
        lampShader.setMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //buffers swap and io events poll
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
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

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

//glfw -> whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    //reversed since y-coordinates go from bottom to top
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

//glfw -> whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
