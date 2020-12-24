#include "Game.h"

bool moving = false;
glm::vec2 lastVelocity = glm::vec2(0.0f);

Game::Game(unsigned int screenWidth, unsigned int screenHeight) 
	:m_Width(screenWidth), m_Height(screenHeight)
{
	m_State = GameState::ACTIVE; //TODO: Change to menu towards the end
	m_SpriteRenderer = nullptr;
	m_TestEntity = nullptr;
	m_ParticleGenerator = nullptr;
	m_Camera = nullptr;
	m_Filter = nullptr;
}

Game::~Game() {
	delete m_SpriteRenderer;
	m_SpriteRenderer = nullptr;

	delete m_TestEntity;
	m_TestEntity = nullptr;

	delete m_ParticleGenerator;
	m_ParticleGenerator = nullptr;

	delete m_Filter;
	m_Filter = nullptr;

	m_Camera = nullptr;
}

void Game::Init() {
	m_Camera = new Camera(glm::vec2(0.0f));

	ResourceManager::LoadShader("src/Shaders/SpriteVertex.glsl", "src/Shaders/SpriteFragment.glsl", "sprite");
	ResourceManager::LoadShader("src/Shaders/TextVertex.glsl", "src/Shaders/TextFragment.glsl", "text");
	ResourceManager::LoadShader("src/Shaders/HBOutlineVertex.glsl", "src/Shaders/HBOutlineFragment.glsl", "hboutline");
	ResourceManager::LoadShader("src/Shaders/FilterVertex.glsl", "src/Shaders/FilterFragment.glsl", "filter");

	ResourceManager::LoadTexture("Textures/knight.png", "knight");
	ResourceManager::LoadTexture("Textures/HitboxCircle.png", "hitboxCircle");
	ResourceManager::LoadTexture("Textures/Ball.png", "ball");
	ResourceManager::LoadTexture("Textures/ParticleTexture.png", "particle");
	ResourceManager::LoadTexture("Textures/background.png", "background");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);

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

	m_TestEntity = new Entity(ResourceManager::Get<Texture>("knight"), 64.0f, 64.0f, glm::vec2(250.0f, 150.0f), glm::vec2(250.0f), 0.0f, glm::vec2(80.0f, 100.0f), glm::vec2(90.0f));
	AnimationCycle knightCycle;
	knightCycle.LinearX("Idle", 0, 4, 0, 160);
	knightCycle.LinearX("Walking", 0, 7, 1, 100);
	knightCycle.LinearX("Swing", 0, 2, 5, 150, false);

	m_TestEntity->AddComponent<AnimationCycle>("KnightAnimation", knightCycle);
	m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->Animate("Idle");

	m_Camera->SetEntity(m_TestEntity);

	m_ParticleGenerator = new ParticleGenerator(*ResourceManager::Get<Texture>("particle"), glm::vec2(0.0f, 0.0f), glm::vec4(-5, 5, -5, 5), glm::ivec2(30.0f, 50.0f));

	m_Filter = new Filter(m_Width, m_Height);

	m_Text = new TextRenderer(m_Width, m_Height, false);
	m_Text->LoadFont("Fonts/arial.ttf", 24);
}

void Game::ProcessInput(float deltaTime) {
	if (m_Keys[GLFW_KEY_W]) {
		m_TestEntity->Translate(glm::vec2(0.0f, -200.0f), deltaTime);
		m_TestEntity->SetRotation(270.0f);
		m_TestEntity->Flip(false);
		m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->Animate("Walking");
		moving = true;
		m_ParticleGenerator->m_SpawnPos = glm::vec2(m_TestEntity->getPos().x + (m_TestEntity->getSize().x / 2), m_TestEntity->getPos().y + (m_TestEntity->getSize().y / 2));
	}

	if (m_Keys[GLFW_KEY_S]) {
		m_TestEntity->Translate(glm::vec2(0.0f, 200.0f), deltaTime);
		m_TestEntity->SetRotation(90.0f);
		m_TestEntity->Flip(false);
		m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->Animate("Walking");
		moving = true;
		m_ParticleGenerator->m_SpawnPos = glm::vec2(m_TestEntity->getPos().x + (m_TestEntity->getSize().x / 2) - 25.0f, m_TestEntity->getPos().y + (m_TestEntity->getSize().y / 2) - 25.0f);
	}

	if (m_Keys[GLFW_KEY_A]) {
		m_TestEntity->Translate(glm::vec2(-200.0f, 0.0f), deltaTime);
		m_TestEntity->SetRotation(0.0f);
		m_TestEntity->Flip(true);
		m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->Animate("Walking");
		moving = true;
		m_ParticleGenerator->m_SpawnPos = glm::vec2(m_TestEntity->getPos().x + (m_TestEntity->getSize().x / 2) - 10.0f, m_TestEntity->getPos().y + (m_TestEntity->getSize().y / 2) - 10.0f);
	}

	if (m_Keys[GLFW_KEY_D]) {
		m_TestEntity->Translate(glm::vec2(200.0f, 0.0f), deltaTime);
		m_TestEntity->SetRotation(0.0f);
		m_TestEntity->Flip(false);
		m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->Animate("Walking");
		moving = true;
		m_ParticleGenerator->m_SpawnPos = glm::vec2(m_TestEntity->getPos().x + (m_TestEntity->getSize().x / 2), m_TestEntity->getPos().y + (m_TestEntity->getSize().y / 2) - 10.0f);
	}

	if (m_Keys[GLFW_KEY_SPACE]) {
		m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->Animate("Swing");
		m_Camera->Disconnect();
	}

	if (!m_Keys[GLFW_KEY_W] && !m_Keys[GLFW_KEY_A] && !m_Keys[GLFW_KEY_S] && !m_Keys[GLFW_KEY_D]) {
		m_TestEntity->SetRotation(0.0f);
		m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->Animate("Idle");
		moving = false;
	}
}

void Game::Update(float deltaTime) {
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

	//If not moving don't spawn any more particles
	m_ParticleGenerator->Update(deltaTime, 1, true, (moving) ? true : false);

	CheckCollisions();
}

void Game::Render() {
	ImGui::Begin("ImGui");
	//ImGui Code
	ImGui::End();

	if (m_State == GameState::MENU) {
		//holder
	}

	if (m_State == GameState::ACTIVE) {
		m_Filter->BeginFilter();

		//Have to draw the background first otherwise anything drawn before it will be overlayed by the background
		m_SpriteRenderer->DrawSprite(*ResourceManager::Get<Texture>("background"), glm::vec2(0.0f), glm::vec2(m_Width, m_Height));

		m_ParticleGenerator->Draw(*m_SpriteRenderer);
		m_Text->Text("Hello Ventura", 200, 200, 1, glm::vec3(0.0f, 1.0f, 0.0f));
		m_TestEntity->Draw(*m_SpriteRenderer, m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->getSpritePos());

		m_Filter->EndFilter();
		m_Filter->FilterRender(glfwGetTime());
	}
}

void Game::CheckCollisions() {
	//holder
}