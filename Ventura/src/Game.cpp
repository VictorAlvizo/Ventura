#include "Game.h"

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
	ResourceManager::LoadShader("src/Shaders/HBOutlineVertex.glsl", "src/Shaders/HBOutlineFragment.glsl", "hboutline");

	ResourceManager::LoadTexture("Textures/knight.png", "knight");
	ResourceManager::LoadTexture("Textures/Mario.jpg", "mario");
	ResourceManager::LoadTexture("Textures/Ball.png", "circle");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);

	ResourceManager::Get<Shader>("sprite")->Bind();
	ResourceManager::Get<Shader>("sprite")->SetMat4("u_Projection", projection);
	ResourceManager::Get<Shader>("sprite")->UnBind();

	ResourceManager::Get<Shader>("hboutline")->Bind();
	ResourceManager::Get<Shader>("hboutline")->SetMat4("u_Projection", projection);
	ResourceManager::Get<Shader>("hboutline")->UnBind();

	m_SpriteRenderer = new SpriteRenderer(ResourceManager::Get<Shader>("sprite"));

	m_TestEntity = new Circle(ResourceManager::Get<Texture>("circle"), glm::vec2(100.0f), 50.0f);
	m_TestEntity->m_ShowHitbox = true;

	m_ColEnt = new Entity(ResourceManager::Get<Texture>("mario"), glm::vec2(400.0f, 200.0f), glm::vec2(200.0f));
	m_ColEnt->m_ShowHitbox = true;
}

void Game::ProcessInput(float deltaTime) {
	if (m_MouseButtons[GLFW_MOUSE_BUTTON_4]) {
		std::cout << "Mouse: " << m_MousePos.x << "," << m_MousePos.y << std::endl;
	}

	if (m_Keys[GLFW_KEY_W]) {
		m_TestEntity->Translate(glm::vec2(0.0f, -200.0f), deltaTime);
	}
	else if (m_Keys[GLFW_KEY_S]) {
		m_TestEntity->Translate(glm::vec2(0.0f, 200.0f), deltaTime);
	}
	else if (m_Keys[GLFW_KEY_A]) {
		m_TestEntity->Translate(glm::vec2(-200.0f, 0.0f), deltaTime);
	}
	else if (m_Keys[GLFW_KEY_D]) {
		m_TestEntity->Translate(glm::vec2(200.0f, 0.0f), deltaTime);
	}
	else if (m_Keys[GLFW_KEY_SPACE]) {
		//holder
	}

	if (!m_Keys[GLFW_KEY_W] && !m_Keys[GLFW_KEY_A] && !m_Keys[GLFW_KEY_S] && !m_Keys[GLFW_KEY_D]) {
		//holder
	}
}

void Game::Update(float deltaTime) {
	CheckCollisions();
}

void Game::Render() {

	ImGui::Begin("ImGui");
	ImGui::SliderFloat("Rotation", &m_ColEnt->m_Rotation, 0.0f, 360.0f);
	ImGui::End();

	if (m_State == GameState::MENU) {
		//holder
	}

	if (m_State == GameState::ACTIVE) {
		m_TestEntity->Draw(*m_SpriteRenderer);
		m_ColEnt->Draw(*m_SpriteRenderer);
	}
}

void Game::CheckCollisions() {
	if (CollisionHandler::CollideCircleAABB(dynamic_cast<Circle *>(m_TestEntity), *m_ColEnt)) {
		std::cout << "Objects have collided" << std::endl;
	}
}