#include "Game.h"

float slideValue = 100.0f;

Game::Game(unsigned int screenWidth, unsigned int screenHeight, float gravity) 
	:m_Width(screenWidth), m_Height(screenHeight), m_Gravity(gravity)
{
	m_SpriteRenderer = nullptr;
	m_Slider = nullptr;
}

Game::~Game() {
	delete m_SpriteRenderer;
	m_SpriteRenderer = nullptr;

	m_Camera = nullptr;

	delete m_Slider;
	m_Slider = nullptr;
}

void Game::Init() {
	EngineInit();
	
	ResourceManager::LoadTexture("Textures/SliderTest.png", "Slide");

	m_Slider = new Slider(ResourceManager::Get<Texture>("Slide"), glm::vec2(100.0f), glm::vec2(300.0f, 100.0f));
}

void Game::ProcessInput(float deltaTime) {
	//Manage input
}

void Game::Update(float deltaTime) {
	EngineUpdate();

	m_Slider->SetPercentage(slideValue);

	if (m_Slider->getPercentage() == 0.0f) {
		std::cout << "Out of health!" << std::endl;
	}

	CheckCollisions();
}

void Game::Render() {
	ImGui::Begin("ImGui");
	ImGui::SliderFloat("Slide Value", &slideValue, 0.0f, 100.0f);
	ImGui::End();

	m_SpriteRenderer->DrawSprite(*ResourceManager::Get<Texture>("Slide"), glm::vec2(100.0f), glm::vec2(300.0f, 100.0f));
	m_Slider->Draw(*m_SpriteRenderer, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Game::CheckCollisions() {
	//Manage Collision
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