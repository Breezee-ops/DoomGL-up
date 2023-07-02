
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>


#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "engine.h"

const int GLSW = 800;
const int GLSH = 600;
const int SW2 = 400;
const int SH2 = 300;

/** TODO
* 1) window init loop done
* 2) shaders write them
* 3) handling point manipulation (do this in vertex shaders)
*       rotation
*       transformation to screen coordinates (send this to gpu for now) I think so?
* 4) write init function
*/

player p;
trig m;

const char* vertexshadersource = "#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
"}\0";

const char* fragmentshadersource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

engine eng;

// init func

void init(sectors *walls) {
    p = { 0, 0, 20, 0, 0 };
    for (int x = 0; x < 360; x++) {
        m.COS[x] = cos(x / 180.0 * 3.1415);
        m.SIN[x] = sin(x / 180.0 * 3.1415);
    }
    walls[0].x1 = 30;
    walls[0].y1 = 10;
    walls[0].z = 40;

    walls[0].x2 = 70;
    walls[0].y2 = 10;

    walls[1].x1 = 70;
    walls[1].y1 = 10;
    walls[1].z = 40;

    walls[1].x2 = 70;
    walls[1].y2 = 40;
}

int main() {
	// Window init start
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	// Window pointer and window start

    GLFWwindow* window = glfwCreateWindow(800, 600, "wonderwindow", NULL, NULL);
    if (window == nullptr) {
        std::cout << "window error" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // check if glad loader loaded all the function pointers use glfw from proc function as loader
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "glad failed to load function pointers" << std::endl;
        return -1;
    }

    // end window handling
    /**TODO
    * shader handling
    * render loop
    */
    // shader link and compile
    // vertex shader
    unsigned int vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexshader, 1, &vertexshadersource, NULL);
    glCompileShader(vertexshader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexshader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // frag shader
    unsigned int fragshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragshader, 1, &fragmentshadersource, NULL);
    glCompileShader(fragshader);
    glGetShaderiv(fragshader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragshader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // linker
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexshader);
    glAttachShader(shaderProgram, fragshader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexshader);
    glDeleteShader(fragshader);

    float points[8] = { 0,0,0,0,0,0,0,0 };
    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3
    };
    sectors walls[2];

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STREAM_DRAW); 
    
    glBindBuffer(GL_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);

    int tick = 0;
    int time = 0;
    int curtime = 0;
    
    init(walls);
    // render loop

    while (!glfwWindowShouldClose(window)) {
        time = (int)glfwGetTime();
        
        eng.keyboardHandle(p, m, window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        eng.render(walls, 2, p, m, points, indices, VAO, VBO, EBO, shaderProgram);

        //std::cout << points[0] << std::endl;
        //std::cout << std::endl;
        glfwSwapBuffers(window);
        glfwPollEvents();
        tick += 1;
        if (time - curtime == 1) {
            std::cout << tick << std::endl;
            tick = 0;
        }
        curtime = time;
    }

    // destructor portion

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}