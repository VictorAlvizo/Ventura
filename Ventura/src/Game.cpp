#include "Game.h"

Game::Game(unsigned int screenWidth, unsigned int screenHeight) 
	:m_Width(screenWidth), m_Height(screenHeight)
{
	m_State = GameState::ACTIVE; //TODO: Change to menu towards the end
	m_SpriteRenderer = nullptr;
	m_Entity = nullptr;
	m_Entity2 = nullptr;
}

Game::~Game() {
	delete m_SpriteRenderer;
	delete m_Entity;
	delete m_Entity2;
}

void Game::Init() {
	ResourceManager::LoadShader("src/Shaders/SpriteVertex.glsl", "src/Shaders/SpriteFragment.glsl", "sprite");
	ResourceManager::LoadShader("src/Shaders/TextVertex.glsl", "src/Shaders/TextFragment.glsl", "text");

	ResourceManager::LoadTexture("Textures/knight.png", "knight");
	ResourceManager::LoadTexture("Textures/Tex1.jpg", "tex1");
	ResourceManager::LoadTexture("Textures/Tex2.jpg", "tex2");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);

	ResourceManager::Get<Shader>("sprite")->Bind();
	ResourceManager::Get<Shader>("sprite")->SetMat4("u_Projection", projection);

	m_SpriteRenderer = new SpriteRenderer(ResourceManager::Get<Shader>("sprite"));

	m_Entity = new Entity(*ResourceManager::Get<Texture>("tex1"), glm::vec2(0.0f), glm::vec2(200.0f));
	m_Entity2 = new Entity(*ResourceManager::Get<Texture>("tex2"), glm::vec2(500.0f, 300.0f), glm::vec2(200.0f));
}

void Game::ProcessInput(float deltaTime) {
	if (m_MouseButtons[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_PRESS) {
		m_Entity->Move(glm::vec2(m_MouseX, m_MouseY));
	}
}

void Game::Update(float deltaTime) {
	CheckCollisions();
}

void Game::Render() {

	ImGui::Begin("ImGui");
	ImGui::SliderFloat("Rotation", &m_Entity2->m_Rotation, 0.0f, 360.0f);
	ImGui::End();

	if (m_State == GameState::MENU) {
		//holder
	}

	if (m_State == GameState::ACTIVE) {
		m_Entity->Draw(*m_SpriteRenderer);
		m_Entity2->Draw(*m_SpriteRenderer);
	}
}

void Game::CheckCollisions() {
	if (CollisionHandler::CollideSAT(*m_Entity, *m_Entity2)) {
		std::cout << "Objects have collided" << std::endl;
	}
}
