#pragma once
#ifndef GLFW_FUNCTION_H
#define GLFW_FUNCTION_H

extern float transparent;
extern float cameraSpeed;
extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;
extern float deltaTime;
extern float pitch, yaw, lastX, lastY, fov;
float sensitivity = 0.05f;
bool ifMouseFirst = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (ifMouseFirst) {
        lastX = xpos;
        lastY = ypos;
        ifMouseFirst = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    pitch += sensitivity * yoffset;
    yaw += sensitivity * xoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    float camY = sin(glm::radians(pitch));
    float camX = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    float camZ = cos(glm::radians(pitch)) * (-1) * cos(glm::radians(yaw));
    cameraFront = glm::normalize(glm::vec3(camX, camY, camZ));

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        transparent = fmax(0.0, transparent - 0.0001f);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        transparent = fmin(1.0, transparent + 0.0001f);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += glm::normalize(cameraFront) * cameraSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraUp, cameraFront)) * cameraSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraUp, cameraFront)) * cameraSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= glm::normalize(cameraFront) * cameraSpeed * deltaTime;
    }
        
}



GLFWwindow* initGLFW(int major_version = 3, int minor_version = 3, int width = 800, int height = 600) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    else {
        glfwMakeContextCurrent(window);
    }
    return window;
}

void initGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
}

void setGLFWCallBack(GLFWwindow* window) {
    if (window == NULL) {
        return;
    }
    else{
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
    }
        
}

#endif // !GLFW_FUNCTION_H