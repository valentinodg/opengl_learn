//pre-processing
#include "../lib/glad/glad.h"
#include </usr/include/GLFW/glfw3.h>

#include "../lib/glm/glm.hpp"
#include "../lib/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/gtc/type_ptr.hpp"

#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"

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
Camera camera(glm::vec3(0.0f, 0.0f, 55.0f));
float lastX = (float)SCREEN_WIDTH / 2.0;
float lastY = (float)SCREEN_HEIGHT / 2.0;
bool firstMouse = true;

//timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
    glEnable(GL_DEPTH_TEST);

    //build and compile the shader program
    Shader shader("18.1.instancing.vertex", "18.1.instancing.fragment");

    //load models
    Model rock("./model/rock.obj");
    Model planet("./model/planet.obj");

    unsigned int amount = 1000;
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];
    //initialize random seed
    srand(glfwGetTime());
    float radius = 50.0;
    float offset = 2.5f;
    for (unsigned int i = 0; i < amount; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        //1. traslation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        //keep height of asteroid field smaller compared to width of x and z
        float y = displacement * 0.4f;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        //2. scale: scale between 0.05f and 0.25f
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));

        //3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
        
        //4. now add to list of matrices
        modelMatrices[i] = model;
    }

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

        //configure transformation matrices
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        //draw planet
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        shader.setMat4("model", model);
        planet.Draw(shader);

        //draw meteorites
        for (unsigned int i = 0; i < amount; i++) {
            shader.setMat4("model", modelMatrices[i]);
            rock.Draw(shader);
        }

        //buffers swap and io events poll
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

//functions definition
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}