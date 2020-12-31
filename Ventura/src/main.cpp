#include <glad/glad.h>
#include <glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "Game.h"

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode);
void MouseButtonCallback(GLFWwindow * window, int button, int action, int mods);
void MousePosCallback(GLFWwindow * window, double xPos, double yPos);

//TODO: Find a way allow to developer to edit window size without having to touch main.cpp
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

Game ventura(SCREEN_WIDTH, SCREEN_HEIGHT, 9.8f);

void FramebufferSizeCallback(GLFWwindow * window, int width, int height) {
    glViewport(0, 0, width, height);
    ventura.m_Width = width;
    ventura.m_Height = height;
}

int main() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef  __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif //  __APPLE__

    GLFWwindow * window;

    if (!glfwInit()) {
        std::cout << "Error: Failed to initialize GLFW" << std::endl;
    }

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ventura", nullptr, nullptr);

    if (!window) {
        std::cout << "Error: Failed to create the window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Error: Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MousePosCallback);

    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ventura.Init();

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float currentTime = 0.0f;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

    while (!glfwWindowShouldClose(window)) {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();

        ventura.ProcessInput(deltaTime);
        ventura.Update(deltaTime);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ventura.Render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode) {
    if (key >= 0 && key <= 1024) {
        if (action == GLFW_PRESS) {
            ventura.m_Keys[key] = true;
            ventura.m_KeyAllowment[key] = 1;
        }
        else if (action == GLFW_RELEASE) {
            ventura.m_Keys[key] = false;
            ventura.m_KeyAllowment[key] = 0;
        }
    }
}

void MousePosCallback(GLFWwindow * window, double xPos, double yPos) {
    ventura.m_MousePos = glm::vec2(xPos, yPos);
}

void MouseButtonCallback(GLFWwindow * window, int button, int action, int mods) {
    ventura.m_MouseButtons[button] = (action == GLFW_PRESS) ? true : false;
}