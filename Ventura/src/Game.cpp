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
	ResourceManager::LoadTexture("Textures/HitboxCircle.png", "hitboxCircle");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);

	ResourceManager::Get<Shader>("sprite")->Bind();
	ResourceManager::Get<Shader>("sprite")->SetMat4("u_Projection", projection);
	ResourceManager::Get<Shader>("sprite")->UnBind();

	ResourceManager::Get<Shader>("hboutline")->Bind();
	ResourceManager::Get<Shader>("hboutline")->SetMat4("u_Projection", projection);
	ResourceManager::Get<Shader>("hboutline")->UnBind();

	m_SpriteRenderer = new SpriteRenderer(ResourceManager::Get<Shader>("sprite"));
	m_TestEntity = new Entity(ResourceManager::Get<Texture>("knight"), 64.0f, 64.0f, glm::vec2(100.0f), glm::vec2(250.0f), glm::vec2(80.0f, 100.0f), glm::vec2(90.0f));
	m_TestEntity->m_ShowHitbox = true;

	AnimationCycle knightCycle;
	knightCycle.LinearX("Idle", 0, 4, 0, 160);
	knightCycle.LinearX("Walking", 0, 7, 1, 100);
	knightCycle.LinearX("Swing", 0, 2, 5, 150, false);

	m_TestEntity->AddComponent<AnimationCycle>("KnightAnimation", knightCycle);
	m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->Animate("Idle");

	m_TestEntity->AddHitbox("test", glm::vec2(100.0f), glm::vec2(200.0f));
}

void Game::ProcessInput(float deltaTime) {
	if (m_Keys[GLFW_KEY_W]) {
		m_TestEntity->Translate(glm::vec2(0.0f, -200.0f), deltaTime);
		m_TestEntity->m_Rotation = 270.0f;
		m_TestEntity->Flip(false);
		m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->Animate("Walking");
	}

	if (m_Keys[GLFW_KEY_S]) {
		m_TestEntity->Translate(glm::vec2(0.0f, 200.0f), deltaTime);
		m_TestEntity->m_Rotation = 90.0f;
		m_TestEntity->Flip(false);
		m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->Animate("Walking");
	}

	if (m_Keys[GLFW_KEY_A]) {
		m_TestEntity->Translate(glm::vec2(-200.0f, 0.0f), deltaTime);
		m_TestEntity->m_Rotation = 0.0f;
		m_TestEntity->Flip(true);
		m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->Animate("Walking");
	}

	if (m_Keys[GLFW_KEY_D]) {
		m_TestEntity->Translate(glm::vec2(200.0f, 0.0f), deltaTime);
		m_TestEntity->m_Rotation = 0.0f;
		m_TestEntity->Flip(false);
		m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->Animate("Walking");
	}

	if (m_Keys[GLFW_KEY_SPACE]) {
		m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->Animate("Swing");
	}

	if (!m_Keys[GLFW_KEY_W] && !m_Keys[GLFW_KEY_A] && !m_Keys[GLFW_KEY_S] && !m_Keys[GLFW_KEY_D]) {
		m_TestEntity->m_Rotation = 0.0f;
		m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->Animate("Idle");
	}
}

void Game::Update(float deltaTime) {
	CheckCollisions();
}

void Game::Render() {

	ImGui::Begin("ImGui");
	//holder
	ImGui::End();

	if (m_State == GameState::MENU) {
		//holder
	}

	if (m_State == GameState::ACTIVE) {
		m_TestEntity->Draw(*m_SpriteRenderer, m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->getSpritePos());
	}
}

void Game::CheckCollisions() {
	//holder
}