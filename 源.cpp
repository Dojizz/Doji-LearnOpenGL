#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "data.h"
#include "glfw_function.h"
#include "gl_function.h"
#include "shader.h"
#include "stb_image.h"
#include "camera.h"
#include "texture.h"
#include "mesh.h"
#include "model.h"


float transparent = 0.5;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float currentFrame = 0.0f;
float lastX = 400, lastY = 300;
glm::vec4 backColor(0.98f, 0.55f, 0.21f, 1.0f);
Camera mainCamera;
float lightScale = 0.2;
glm::vec3 lightPos(0.f, 0.f, 4.f);

void render(GLFWwindow *window) {
    if (window == NULL)
        return;
    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_DEPTH_TEST);

    //// model import
    //Model nanosuit("resources/models/nanosuit/nanosuit.obj");
    //ShaderProgram nanoShader("nanosuitShader.vert", "nanosuitShader.frag");
    
    glm::vec4 testVec(0.f, 0.f, -100.f, 1.f);
    testVec = mainCamera.GetProjMatrix() * testVec;
    std::cout << testVec.z;

    //while (!glfwWindowShouldClose(window))
    //{
    //    glfwPollEvents();
    //    
    //    currentFrame = glfwGetTime();
    //    deltaTime = currentFrame - lastFrame;
    //    lastFrame = currentFrame;
    //    // input
    //    processInput(window);
    //    glClearColor(backColor.x, backColor.y, backColor.z, backColor.w);
    //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //    // draw
    //    nanoShader.Use();
    //    nanoShader.SetMat4fv("model", glm::mat4(1.0f));
    //    nanoShader.SetMat4fv("view", mainCamera.GetViewMatrix());
    //    nanoShader.SetMat4fv("projection", mainCamera.GetProjMatrix());
    //    nanosuit.Draw(nanoShader);

    //    // swap
    //    glfwSwapBuffers(window);
    //}

    glfwTerminate();
}



int main()
{
    GLFWwindow* window = initGLFW();
    initGLAD();
    setGLFWCallBack(window);

    render(window);
    return 0;
}


