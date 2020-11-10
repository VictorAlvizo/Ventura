#include "Game.h"

Game::Game(unsigned int screenWidth, unsigned int screenHeight) 
	:m_Width(screenWidth), m_Height(screenHeight)
{
	m_State = GameState::ACTIVE; //TODO: Change to menu towards the end
	m_SpriteRenderer = nullptr;
	testEnt = nullptr;
	testEnt2 = nullptr;
}

Game::~Game() {
	delete m_SpriteRenderer;
	delete testEnt;
	delete testEnt2;
}

void Game::Init() {
	ResourceManager::LoadShader("src/Shaders/SpriteVertex.glsl", "src/Shaders/SpriteFragment.glsl", "sprite");
	ResourceManager::LoadShader("src/Shaders/TextVertex.glsl", "src/Shaders/TextFragment.glsl", "text");

	ResourceManager::LoadTexture("Textures/knight.png", "knight");
	ResourceManager::LoadTexture("Textures/map.jpg", "map");
	ResourceManager::LoadTexture("Textures/boat.jpg", "boat");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);

	ResourceManager::Get<Shader>("sprite")->Bind();
	ResourceManager::Get<Shader>("sprite")->SetMat4("u_Projection", projection);

	m_SpriteRenderer = new SpriteRenderer(ResourceManager::Get<Shader>("sprite"));

	testEnt = new Entity(*ResourceManager::Get<Texture>("map"), glm::vec2(300.0f), glm::vec2(200.0f));
	testEnt2 = new Entity(*ResourceManager::Get<Texture>("boat"), glm::vec2(50.0f), glm::vec2(200.0f));
}

void Game::ProcessInput(float deltaTime) {
	if (m_MouseButtons[GLFW_MOUSE_BUTTON_2]) {
		testEnt2->Move(m_MousePos);
	}

	if (m_MouseButtons[GLFW_MOUSE_BUTTON_4]) {
		std::cout << "Mouse Pos (" << m_MousePos.x << "," << m_MousePos.y << ")" << std::endl;
	}
}

void Game::Update(float deltaTime) {
	CheckCollisions();
}

void Game::Render() {

	ImGui::Begin("ImGui");
	ImGui::SliderFloat("Rotation", &testEnt->m_Rotation, 0.0f, 360.0f);
	ImGui::End();

	if (m_State == GameState::MENU) {
		//holder
	}

	if (m_State == GameState::ACTIVE) {
		testEnt->Draw(*m_SpriteRenderer);
		testEnt2->Draw(*m_SpriteRenderer);
	}
}

void Game::CheckCollisions() {
	if (CollisionHandler::CollideRadius(*testEnt, *testEnt2)) {
		std::cout << "Objects have collided" << std::endl;
	}
}
