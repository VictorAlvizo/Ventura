#include "Game.h"

Game::Game(unsigned int screenWidth, unsigned int screenHeight) 
	:m_Width(screenWidth), m_Height(screenHeight)
{
	m_State = GameState::ACTIVE; //TODO: Change to menu towards the end
	m_SpriteRenderer = nullptr;
	testEnt = nullptr;
}

Game::~Game() {
	delete m_SpriteRenderer;
	delete testEnt;
}

void Game::Init() {
	ResourceManager::LoadShader("src/Shaders/SpriteVertex.glsl", "src/Shaders/SpriteFragment.glsl", "sprite");
	ResourceManager::LoadShader("src/Shaders/TextVertex.glsl", "src/Shaders/TextFragment.glsl", "text");

	ResourceManager::LoadTexture("Textures/knight.png", "knight");
	ResourceManager::LoadTexture("Textures/map.jpg", "map");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);

	ResourceManager::Get<Shader>("sprite")->Bind();
	ResourceManager::Get<Shader>("sprite")->SetMat4("u_Projection", projection);

	m_SpriteRenderer = new SpriteRenderer(ResourceManager::Get<Shader>("sprite"));

	testEnt = new Entity(*ResourceManager::Get<Texture>("map"), glm::vec2(0.0f), glm::vec2(200.0f));
}

void Game::ProcessInput(float deltaTime) {
	//FIXME: Is the mouse pos correct????
	if (m_MouseButtons[GLFW_MOUSE_BUTTON_2]) {
		std::cout << "Mouse Pos" << "(" << m_MouseX << "," << m_MouseY << ")" << std::endl;
	}

	if (m_MouseButtons[GLFW_MOUSE_BUTTON_1]) {
		testEnt->Move(glm::vec2(m_MouseX, m_MouseY)); //Note: Ent y ends at 800 when it should end at 600
	}
}

void Game::Update(float deltaTime) {
	CheckCollisions();
}

void Game::Render() {

	ImGui::Begin("ImGui");
	ImGui::SliderFloat("Rotation", &testEnt->m_Rotation, 0.0f, 360.0f);
	ImGui::End();

	std::vector<glm::vec2> corners = testEnt->GetCorners();

	for (glm::vec2 corner : corners) {
		std::cout << "(" << corner.x << "," << corner.y << ")" << std::endl;
	}

	std::cout << std::endl;

	if (m_State == GameState::MENU) {
		//holder
	}

	if (m_State == GameState::ACTIVE) {
		testEnt->Draw(*m_SpriteRenderer);
	}
}

void Game::CheckCollisions() {
	//holder
}
