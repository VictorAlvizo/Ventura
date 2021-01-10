#include "Game.h"

bool shouldBurst = true;

Game::Game(unsigned int screenWidth, unsigned int screenHeight, float gravity) 
	:m_Width(screenWidth), m_Height(screenHeight), m_Gravity(gravity)
{
	m_SpriteRenderer = nullptr;
}

Game::~Game() {
	delete m_SpriteRenderer;
	m_SpriteRenderer = nullptr;

	m_Camera = nullptr;
}

void Game::Init() {
	EngineInit();

	m_Particles = new ParticleGenerator(*ResourceManager::Get<Texture>("hitboxCircle"), glm::vec2(300.0f), glm::vec2(0.0f, 100.0f), glm::vec4(10.0f), glm::vec2(30.0f));
	m_Particles->AppendVelocityList({ glm::vec2(0.0f, -100.0f), glm::vec2(100.0f, 0.0f), glm::vec2(-100.0f, 0.0f) });
	m_Particles->AppendColors({ glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) });

	//Spawn new particles for half a second every 2 seconds
	m_Particles->BurstMode(true, 0.5f);
}

void Game::ProcessInput(float deltaTime) {
	//Toggle the option for particle bursting, needs to ensure it only proccesses 1 key or else
	//it can glitch during toggling
	if (m_Keys[GLFW_KEY_SPACE] && m_KeyAllowment[GLFW_KEY_SPACE] == 1) {
		m_KeyAllowment[GLFW_KEY_SPACE] = 0;

		shouldBurst = !shouldBurst;
		m_Particles->BurstMode(shouldBurst, 0.5f);
	}
}

void Game::Update(float deltaTime) {
	EngineUpdate();
	CheckCollisions();

	m_Particles->Update(deltaTime, 2, true, true);
}

void Game::Render() {
	ImGui::Begin("ImGui");
	//ImGui Code
	ImGui::End();

	m_Particles->Draw(*m_SpriteRenderer);
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