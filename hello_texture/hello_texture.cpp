//
// Created by 田一菲 on 2019/2/13.
//

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

#include <cv.h>
#include <highgui.h>

#include "Shader.h"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


const GLuint winWidth = 800, winHeight = 600;

GLfloat mixValue = 0.1f;

void loadTextrue(char* name, GLuint& imageTexture)
{
    cv::Mat image = cv::imread(name);
    if(image.empty()){
        std::cout << "image empty" << std::endl;
    }
    else {
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glGenTextures(1, &imageTexture);
        glBindTexture(GL_TEXTURE_2D, imageTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        cv::cvtColor(image, image, CV_RGB2BGR);
        glTexImage2D(GL_TEXTURE_2D,         // Type of texture
                     0,                   // Pyramid level (for mip-mapping) - 0 is the top level
                     GL_RGB,              // Internal colour format to convert to
                     image.cols,          // Image width  i.e. 640 for Kinect in standard mode
                     image.rows,          // Image height i.e. 480 for Kinect in standard mode
                     0,                   // Border width in pixels (can either be 1 or 0)
                     GL_RGB,              // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
                     GL_UNSIGNED_BYTE,    // Image data type
                     image.ptr());        // The actual image data itself

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // MAC


    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);

    glewExperimental = GL_TRUE;
    glewInit();

    glViewport(0, 0, winWidth, winHeight);


    Shader ourShader("../hello_texture/default.vs", "../hello_texture/default.frag");



    GLfloat vertices[] =
            {
                    // Positions          // Colors           // Texture Coords
                    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
                    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
                    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
                    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
            };

    GLuint indices[] =
            {
                    0, 1, 3,
                    1, 2, 3
            };
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // 纹理属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);


    // 纹理
    GLuint tex1, tex2;
    loadTextrue("../hello_texture/container.jpg",tex1);
    loadTextrue("../hello_texture/awesomeface.png",tex2);

    // Loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        ourShader.Use();

        glActiveTexture(GL_TEXTURE0);  // 激活纹理单元
        glBindTexture(GL_TEXTURE_2D, tex1);
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex2);
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);
        glUniform1f(glGetUniformLocation(ourShader.Program, "viDig"), mixValue);


        // Draw
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

}
