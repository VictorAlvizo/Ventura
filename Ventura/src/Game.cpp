#include "Game.h"

int x = 0;
int y = 0;
bool flipped = false;

Game::Game(unsigned int screenWidth, unsigned int screenHeight) 
	:m_Width(screenWidth), m_Height(screenHeight)
{
	m_State = GameState::ACTIVE; //TODO: Change to menu towards the end
	m_SpriteRenderer = nullptr;
	m_TestEntity = nullptr;
}

Game::~Game() {
	delete m_SpriteRenderer;
	delete m_TestEntity;
}

void Game::Init() {
	ResourceManager::LoadShader("src/Shaders/SpriteVertex.glsl", "src/Shaders/SpriteFragment.glsl", "sprite");
	ResourceManager::LoadShader("src/Shaders/TextVertex.glsl", "src/Shaders/TextFragment.glsl", "text");

	ResourceManager::LoadTexture("Textures/knight.png", "knight");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);

	ResourceManager::Get<Shader>("sprite")->Bind();
	ResourceManager::Get<Shader>("sprite")->SetMat4("u_Projection", projection);

	m_SpriteRenderer = new SpriteRenderer(ResourceManager::Get<Shader>("sprite"));
	m_TestEntity = new Entity(ResourceManager::Get<Texture>("knight"), 64.0f, 64.0f, glm::vec2(100.0f), glm::vec2(200.0f));
}

void Game::ProcessInput(float deltaTime) {
	//holder
}

void Game::Update(float deltaTime) {
	if (flipped != m_TestEntity->isFlipped()) {
		m_TestEntity->Flip();
	}

	CheckCollisions();
}

void Game::Render() {

	ImGui::Begin("ImGui");
	ImGui::SliderInt("X", &x, 0, 7);
	ImGui::SliderInt("Y", &y, 0, 8);
	ImGui::Checkbox("Flipped", &flipped);
	ImGui::End();

	if (m_State == GameState::MENU) {
		//holder
	}

	if (m_State == GameState::ACTIVE) {
		m_TestEntity->Draw(*m_SpriteRenderer, glm::ivec2(x, y));
	}
}

void Game::CheckCollisions() {
	//holder
}
