#include <glad/glad.h>
#include <glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "Game.h"
#include "Vendor/imgui/imgui.h"
#include "Vendor/imgui/imgui_impl_glfw.h"
#include "Vendor/imgui/imgui_impl_opengl3.h"

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

Game ventura(SCREEN_WIDTH, SCREEN_WIDTH);

void FramebufferSizeCallback(GLFWwindow * window, int width, int height) {
    glViewport(0, 0, width, height);
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
        std::cout << "Error: " << "Failed to initialize GLFW" << std::endl;
    }

    //TODO: Figure out how to let user choose their moniter
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ventura", nullptr, nullptr);

    if (!window) {
        std::cout << "Error: " << "Failed to create the window" << std::endl;
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

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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
    //TODO: May make a "are you sure?" option when doing this or a pause menu
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    //TODO: Implment logic so keys aren't detected mutiple times in 1 key press
    if (key >= 0 && key <= 1024) {
        if (action == GLFW_PRESS) {
            ventura.m_Keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            ventura.m_Keys[key] = false;
        }
    }
}