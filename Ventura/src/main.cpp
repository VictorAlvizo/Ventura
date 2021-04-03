#include <glad/glad.h>
#include <glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "Game.h"

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode);
void MouseButtonCallback(GLFWwindow * window, int button, int action, int mods);
void MousePosCallback(GLFWwindow * window, double xPos, double yPos);

//Note: If planning to not use full screen, declare initial size here
float windowWidth = 800, windowHeight = 600;
Game * ventura;

void FramebufferSizeCallback(GLFWwindow * window, int width, int height) {
    glViewport(0, 0, width, height);
    ventura->m_Width = width;
    ventura->m_Height = height;
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
        return -1;
    }

    /* Uncomment if you want to use fullscreen
    int moniterCount;
    GLFWmonitor ** moniters = glfwGetMonitors(&moniterCount);
    //Note: Replace the 0 with whatever moniter you want the window to pop out on, moniterCount
    //Provides the # of moniters avalible. could make a simple C++ script beforehand to
    //provide the user control of the display
    const GLFWvidmode * currentMode = glfwGetVideoMode(moniters[0]);

    windowWidth = currentMode->width;
    windowHeight = currentMode->height;
    window = glfwCreateWindow(windowWidth, windowHeight, "Ventura", moniters[0], nullptr); */

    //Note: Use this line if not using full screen mode, comment this out if using full screen
    window = glfwCreateWindow(windowWidth, windowHeight, "Ventura", nullptr, nullptr);

    //3rd parameter is the initial gravity value
    ventura = new Game(windowWidth, windowHeight, 50.0f);

    GLFWimage images[1];
    //Note: Insert your own favicon image here by replacing "ProductImages..." with your own image path
    images[0].pixels = stbi_load("ProductImages/VenturaLogo.png", &images[0].width, &images[0].height, 0, 4);
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);

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
    glViewport(0, 0, windowWidth, windowHeight);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ventura->Init();

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

        ventura->ProcessInput(deltaTime);
        ventura->Update(deltaTime);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ventura->Render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

        if (ventura->m_CloseWindow) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    delete ventura;
    ventura = nullptr;

    return 0;
}

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode) {
    if (key >= 0 && key <= 1024) {
        if (action == GLFW_PRESS) {
            ventura->m_Keys[key] = true;
            ventura->m_KeyAllowment[key] = 1;

            //To ensure that keyallowment for caps stays 0, as otherwise it would set itself to 1
            if (key == GLFW_KEY_CAPS_LOCK && ventura->m_KeyAllowment[GLFW_KEY_CAPS_LOCK] == 0) {
                ventura->m_KeyAllowment[key] = 0;
            }

            //Note: Assumes that caps lock is off when the user launches the application
            if (key == GLFW_KEY_CAPS_LOCK && ventura->m_KeyAllowment[GLFW_KEY_CAPS_LOCK] == 1) {
                ventura->m_CapsActive = !ventura->m_CapsActive;
                ventura->m_KeyAllowment[GLFW_KEY_CAPS_LOCK] = 0;
            }
        }
        else if (action == GLFW_RELEASE) {
            ventura->m_Keys[key] = false;
            ventura->m_KeyAllowment[key] = 0;
        }
    }
}

void MousePosCallback(GLFWwindow * window, double xPos, double yPos) {
    ventura->m_MousePos = glm::vec2(xPos, yPos);
}

void MouseButtonCallback(GLFWwindow * window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        ventura->m_MouseButtons[button] = true;
        ventura->m_MouseAllowment[button] = 1;
    }
    else if (action == GLFW_RELEASE) {
        ventura->m_MouseButtons[button] = false;
        ventura->m_MouseAllowment[button] = 0;
    }
}