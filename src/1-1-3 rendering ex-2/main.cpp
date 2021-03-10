// pre-processing
#include "../../lib/glad/glad.h"
#include </usr/include/GLFW/glfw3.h>
#include <iostream>

// namespaces
using namespace std;

// functions prototypes
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// macros
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

// vertex shader
const char *vertexShaderSource =
    "#version 330 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

// main
int main() {

        // glfw: initialization
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // glfw: window creation
        GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT,
                                              "gl-window", NULL, NULL);
        if (window == NULL) {
                cout << "failed to create glfw window" << endl;
                glfwTerminate();
                return -1;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // glad: load all opengl function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                cout << "failed to initialize GLAD" << endl;
                return -1;
        }

        // vertex shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        // error check
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        if (!success) {
                glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
                cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                     << infoLog << endl;
        }

        // fragment shader
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        // error check
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

        if (!success) {
                glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
                cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                     << infoLog << endl;
        }

        // shader program
        unsigned int shaderProgram = glCreateProgram();

        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // error check
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
                glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
                cout << "ERROR::SHADER::PROGRAM::LIKING_FAILED\n"
                     << infoLog << endl;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // vertex data & buffer(s) set up and vertex attributes configuration

        float firstTriangle[] = {
            -0.9f,  -0.5f, 0.0f, // left
            -0.0f,  -0.5f, 0.0f, // right
            -0.45f, 0.5f,  0.0f, // top
        };

        float secondTriangle[] = {
            0.0f,  -0.5f, 0.0f, // left
            0.9f,  -0.5f, 0.0f, // right
            0.45f, 0.5f,  0.0f  // top
        };

        unsigned int VBOs[2], VAOs[2];
        glGenVertexArrays(2, VAOs);
        glGenBuffers(2, VBOs);

        // firstTriangle setup
        glBindVertexArray(VAOs[0]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle,
                     GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);

        // secondTriangle setup
        glBindVertexArray(VAOs[1]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle,
                     GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // render loop
        while (!glfwWindowShouldClose(window)) {
                // input
                processInput(window);

                // render
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                // triangle draw
                glUseProgram(shaderProgram);

                glBindVertexArray(VAOs[0]);
                glDrawArrays(GL_TRIANGLES, 0, 3);

                glBindVertexArray(VAOs[1]);
                glDrawArrays(GL_TRIANGLES, 0, 3);

                // buffers swap and io events poll
                glfwSwapBuffers(window);
                glfwPollEvents();
        }

        glDeleteVertexArrays(2, VAOs);
        glDeleteBuffers(2, VBOs);

        glfwTerminate();
        return 0;
}

// functions definition
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);
}
