// pre-processing
#include "../../lib/glad/glad.h"
#include </usr/include/GLFW/glfw3.h>
#include <cmath>
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
// UNIFORM
const char *vertexShaderSource =
    "#version 330 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "uniform vec4 ourColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = ourColor;\n"
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
        float vertices[] = {
            -0.5f, -0.5f, 0.0f, // left
            0.5f,  -0.5f, 0.0f, // right
            0.0f,  0.5f,  0.0f  // top
        };

        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                     GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);

        // render loop
        while (!glfwWindowShouldClose(window)) {
                // input
                processInput(window);

                // render
                glClearColor(0.4f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                // triangle draw
                glUseProgram(shaderProgram);

                float timeValue = glfwGetTime();
                float greenValue = sin(timeValue) / 2.0f + 0.5f;
                int vertexColorLocation =
                    glGetUniformLocation(shaderProgram, "ourColor");
                glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 3);

                // buffers swap and io events poll
                glfwSwapBuffers(window);
                glfwPollEvents();
        }

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

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
