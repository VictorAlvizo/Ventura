#include "Game.h"

int index = 0; //NOTE: TESTING VARIBLE, REMOVE LATER

Game::Game(unsigned int screenWidth, unsigned int screenHeight) 
	:m_Width(screenWidth), m_Height(screenHeight)
{
	m_State = GameState::ACTIVE; //TODO: Change to menu towards the end
	m_SpriteRenderer = nullptr;
	m_SSR = nullptr; //NOTE: TESTING VARIBLE, REMOVE LATER AND ALL INSTANCES
}

Game::~Game() {
	delete m_SpriteRenderer;
}

void Game::Init() {
	ResourceManager::LoadShader("src/Shaders/SpriteVertex.glsl", "src/Shaders/SpriteFragment.glsl", "sprite");
	ResourceManager::LoadTexture("Textures/knight.png", "knight");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);

	ResourceManager::Get<Shader>("sprite")->Bind();
	ResourceManager::Get<Shader>("sprite")->SetMat4("u_Projection", projection);

	m_SpriteRenderer = new SpriteRenderer(ResourceManager::Get<Shader>("sprite"));

	m_SSR = new SpriteSheetReader(ResourceManager::Get<Texture>("knight"), glm::vec2(65.0f));
}

void Game::ProcessInput(float deltaTime) {
	//holder
}

void Game::Update(float deltaTime) {
	//holder
}

void Game::Render() {

	ImGui::Begin("ImGui");
	ImGui::SliderInt("Index Slider", &index, 0, 72);
	ImGui::End();

	if (m_State == GameState::MENU) {
		//holder
	}

	if (m_State == GameState::ACTIVE) {
		//TODO: 2D array system to retrive sprites
		m_SpriteRenderer->DrawSprite(ResourceManager::Get<Texture>("knight"), glm::vec2(280.0f, 300.0f), glm::vec2(200.0f), 0.0f, glm::vec3(1.0f), m_SSR->getTexUV(index));
	}
}
