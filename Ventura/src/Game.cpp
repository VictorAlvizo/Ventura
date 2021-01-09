#include "Game.h"

Game::Game(unsigned int screenWidth, unsigned int screenHeight, float gravity) 
	:m_Width(screenWidth), m_Height(screenHeight), m_Gravity(gravity)
{
	m_SpriteRenderer = nullptr;

	m_Button = nullptr;
}

Game::~Game() {
	delete m_SpriteRenderer;
	m_SpriteRenderer = nullptr;

	m_Camera = nullptr;

	delete m_Button;
	m_Button = nullptr;
}

void Game::Init() {
	EngineInit();

	ResourceManager::LoadTexture("Textures/ButtonHover.png", "buttonHover");
	ResourceManager::LoadTexture("Textures/ButtonClick.png", "buttonClick");

	m_Button = new Button(m_Width, m_Height, *ResourceManager::Get<Texture>("button"), *ResourceManager::Get<Texture>("buttonHover"), *ResourceManager::Get<Texture>("buttonClick"), glm::vec2(200.0f), glm::vec2(300.0f, 100.0f), "Normal");
}

void Game::ProcessInput(float deltaTime) {
	//Best place to check mouse events would be in the ProcessInput loop
	//These functions HAVE to be called so Status can change, well atleast isClicked() because that calls isHovering()
	if (m_Button->isClicked(m_MousePos, m_MouseButtons[GLFW_MOUSE_BUTTON_1])) {
		m_Button->m_ButtonText = "Clicked!";
	}
	else if(m_Button->isHovering(m_MousePos)){
		m_Button->m_ButtonText = "Hovering!";
	}
	else {
		m_Button->m_ButtonText = "Normal!";
	}

}

void Game::Update(float deltaTime) {
	EngineUpdate();

	CheckCollisions();
}

void Game::Render() {
	ImGui::Begin("ImGui");
	//ImGui Code
	ImGui::End();

	m_Button->Draw(*m_SpriteRenderer, true, glm::vec4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Game::CheckCollisions() {
	//holder
}

void Game::EngineInit() {
	m_Camera = new Camera(glm::vec2(0.0f), glm::vec2(m_Width, m_Height));
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);

	//Load Essential Shaders
	ResourceManager::LoadShader("src/Shaders/SpriteVertex.glsl", "src/Shaders/SpriteFragment.glsl", "sprite");
	ResourceManager::LoadShader("src/Shaders/TextVertex.glsl", "src/Shaders/TextFragment.glsl", "text");
	ResourceManager::LoadShader("src/Shaders/HBOutlineVertex.glsl", "src/Shaders/HBOutlineFragment.glsl", "hboutline");
	ResourceManager::LoadShader("src/Shaders/FilterVertex.glsl", "src/Shaders/FilterFragment.glsl", "filter");

	//Load Essential Textures
	ResourceManager::LoadTexture("Textures/HitboxCircle.png", "hitboxCircle");
	ResourceManager::LoadTexture("Textures/SliderTexture.png", "slider");
	ResourceManager::LoadTexture("Textures/ButtonTexture.png", "button");

	//Set the default view and projection uniforms in the shaders
	ResourceManager::Get<Shader>("sprite")->Bind();
	ResourceManager::Get<Shader>("sprite")->SetMat4("u_Projection", projection);
	ResourceManager::Get<Shader>("sprite")->SetMat4("u_View", m_Camera->UpdateView());
	ResourceManager::Get<Shader>("sprite")->UnBind();

	ResourceManager::Get<Shader>("hboutline")->Bind();
	ResourceManager::Get<Shader>("hboutline")->SetMat4("u_Projection", projection);
	ResourceManager::Get<Shader>("hboutline")->SetMat4("u_View", m_Camera->UpdateView());
	ResourceManager::Get<Shader>("hboutline")->UnBind();

	ResourceManager::Get<Shader>("text")->Bind();
	ResourceManager::Get<Shader>("text")->SetMat4("u_View", m_Camera->UpdateView());
	ResourceManager::Get<Shader>("text")->UnBind();

	m_SpriteRenderer = new SpriteRenderer(ResourceManager::Get<Shader>("sprite"));
}

void Game::EngineUpdate() {
	//Update view matrix
	glm::mat4 cameraView = m_Camera->UpdateView();

	ResourceManager::Get<Shader>("sprite")->Bind();
	ResourceManager::Get<Shader>("sprite")->SetMat4("u_View", cameraView);
	ResourceManager::Get<Shader>("sprite")->UnBind();

	ResourceManager::Get<Shader>("hboutline")->Bind();
	ResourceManager::Get<Shader>("hboutline")->SetMat4("u_View", cameraView);
	ResourceManager::Get<Shader>("hboutline")->UnBind();

	ResourceManager::Get<Shader>("text")->Bind();
	ResourceManager::Get<Shader>("text")->SetMat4("u_View", cameraView);
	ResourceManager::Get<Shader>("text")->UnBind();
}