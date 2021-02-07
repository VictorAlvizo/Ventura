#include "Game.h"

int testingVarible = 0;

Game::Game(unsigned int screenWidth, unsigned int screenHeight, float gravity) 
	:m_Width(screenWidth), m_Height(screenHeight), m_Gravity(gravity)
{
	m_SpriteRenderer = nullptr;
}

Game::~Game() {
	delete m_SpriteRenderer;
	m_SpriteRenderer = nullptr;

	m_Camera = nullptr;

	delete timer;
	timer = nullptr;
}

void Game::Init() {
	EngineInit();

	timer = new TimerSpecialized<int>(1000);

	//& Captures outside varibles by reference, so when I modify testingVarible it changes it
	//= would make of copy of that varible but modifying it inside the lamda would not change it
	//Look up Lamda capture bracketse for more information

	//Every 1 second this lamda (function) is called, once testingVarible reaches 5, stop the timer
	auto lamda = [&]() {
		testingVarible++;

		if (testingVarible >= 5) {
			std::cout << "Reached 5, Stopping the timer!" << std::endl;
			timer->StopTimer();
		}

		std::cout << "Testing Varible: " << testingVarible << std::endl;
		return testingVarible;
	};

	timer->StartTimer(lamda);
}

void Game::ProcessInput(float deltaTime) {
	//holder
}

void Game::Update(float deltaTime) {
	EngineUpdate();

	//holder

	CheckCollisions();
}

void Game::Render() {
	ImGui::Begin("ImGui");
	//ImGui Code
	ImGui::End();

	//holder
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