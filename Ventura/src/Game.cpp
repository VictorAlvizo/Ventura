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

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);

	ResourceManager::Get<Shader>("sprite")->Bind();
	ResourceManager::Get<Shader>("sprite")->SetMat4("u_Projection", projection);
	ResourceManager::Get<Shader>("sprite")->UnBind();

	ResourceManager::Get<Shader>("hboutline")->Bind();
	ResourceManager::Get<Shader>("hboutline")->SetMat4("u_Projection", projection);
	ResourceManager::Get<Shader>("hboutline")->UnBind();

	m_SpriteRenderer = new SpriteRenderer(ResourceManager::Get<Shader>("sprite"));

	m_TestEntity = new Entity(ResourceManager::Get<Texture>("knight"), 64.0f, 64.0f, glm::vec2(300.0f, 200.0f), glm::vec2(200.0f), glm::vec3(1.0f));
	m_TestEntity->m_ShowHitbox = true;

	AnimationCycle cycle;
	cycle.LinearX("Idle", 0, 4, 0, 150);
	cycle.LinearX("Walking", 0, 7, 1, 80);
	cycle.LinearX("Swing", 0, 2, 5, 140);

	m_TestEntity->AddComponent<AnimationCycle>("AnimCycle", cycle);
	m_TestEntity->GetComponent<AnimationCycle>("AnimCycle")->Animate("Idle");
}

void Game::ProcessInput(float deltaTime) {
	if (m_Keys[GLFW_KEY_W]) {
		m_TestEntity->Translate(glm::vec2(0.0f, -200.0f), deltaTime);
		m_TestEntity->GetComponent<AnimationCycle>("AnimCycle")->Animate("Walking");
		m_TestEntity->m_Rotation = 90.0f;
		m_TestEntity->Flip(true);
	}
	else if (m_Keys[GLFW_KEY_S]) {
		m_TestEntity->Translate(glm::vec2(0.0f, 200.0f), deltaTime);
		m_TestEntity->GetComponent<AnimationCycle>("AnimCycle")->Animate("Walking");
		m_TestEntity->m_Rotation = 90.0f;
		m_TestEntity->Flip(false);
	}
	else if (m_Keys[GLFW_KEY_A]) {
		m_TestEntity->Translate(glm::vec2(-200.0f, 0.0f), deltaTime);
		m_TestEntity->GetComponent<AnimationCycle>("AnimCycle")->Animate("Walking");
		m_TestEntity->m_Rotation = 0.0f;
		m_TestEntity->Flip(true);
	}
	else if (m_Keys[GLFW_KEY_D]) {
		m_TestEntity->Translate(glm::vec2(200.0f, 0.0f), deltaTime);
		m_TestEntity->GetComponent<AnimationCycle>("AnimCycle")->Animate("Walking");
		m_TestEntity->m_Rotation = 0.0f;
		m_TestEntity->Flip(false);
	}
	else if (m_Keys[GLFW_KEY_SPACE]) {
		//FIXME: Cant see the swing animation as it will get overidden by idle below haha
		m_TestEntity->GetComponent<AnimationCycle>("AnimCycle")->Animate("Swing");
		m_TestEntity->m_Rotation = 0.0f;
	}

	if (!m_Keys[GLFW_KEY_W] && !m_Keys[GLFW_KEY_A] && !m_Keys[GLFW_KEY_S] && !m_Keys[GLFW_KEY_D]) {
		m_TestEntity->GetComponent<AnimationCycle>("AnimCycle")->Animate("Idle");
		m_TestEntity->m_Rotation = 0.0f;
	}
}

void Game::Update(float deltaTime) {
	CheckCollisions();
}

void Game::Render() {

	ImGui::Begin("ImGui");
	//ImGui code here
	ImGui::End();

	if (m_State == GameState::MENU) {
		//holder
	}

	if (m_State == GameState::ACTIVE) {
		m_TestEntity->Draw(*m_SpriteRenderer, m_TestEntity->GetComponent<AnimationCycle>("AnimCycle")->getSpritePos());
	}
}

void Game::CheckCollisions() {
	//holder
}