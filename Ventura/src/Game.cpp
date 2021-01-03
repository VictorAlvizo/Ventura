#include "Game.h"

float volume = 0.1f;

Game::Game(unsigned int screenWidth, unsigned int screenHeight, float gravity) 
	:m_Width(screenWidth), m_Height(screenHeight), m_Gravity(gravity)
{
	m_SpriteRenderer = nullptr;
	m_TestEntity = nullptr;
}

Game::~Game() {
	delete m_SpriteRenderer;
	m_SpriteRenderer = nullptr;

	delete m_TestEntity;
	m_TestEntity = nullptr;

	m_Camera = nullptr;
}

void Game::Init() {
	m_Camera = new Camera(glm::vec2(0.0f), glm::vec2(m_Width, m_Height));

	ResourceManager::LoadShader("src/Shaders/SpriteVertex.glsl", "src/Shaders/SpriteFragment.glsl", "sprite");
	ResourceManager::LoadShader("src/Shaders/TextVertex.glsl", "src/Shaders/TextFragment.glsl", "text");
	ResourceManager::LoadShader("src/Shaders/HBOutlineVertex.glsl", "src/Shaders/HBOutlineFragment.glsl", "hboutline");
	ResourceManager::LoadShader("src/Shaders/FilterVertex.glsl", "src/Shaders/FilterFragment.glsl", "filter");

	ResourceManager::LoadTexture("Textures/knight.png", "knight");
	ResourceManager::LoadTexture("Textures/HitboxCircle.png", "hitboxCircle");
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

	//Weird rule here, MUST SET CONSTUCTER TO FALSE if going to be adding audio as a component
	//Otherwise, don't mess with it, as you will probably cause a memory leak
	Audio audio(false);
	m_TestEntity->AddComponent<Audio>("BackgroundAudio", audio);
	m_TestEntity->GetComponent<Audio>("BackgroundAudio")->AddSound("Song", "Audio/breakout.mp3", true);
	m_TestEntity->GetComponent<Audio>("BackgroundAudio")->PlaySound("Song");

	Audio audioSFX(false);
	m_TestEntity->AddComponent<Audio>("SFX", audioSFX);
	m_TestEntity->GetComponent<Audio>("SFX")->AddSound("Swing", "Audio/SwordSwing.mp3");

	m_GroundFloor = new Hitbox(glm::vec2(0.0f, 550.0f), glm::vec2(800.0f, 50.0f));

 	coolBeans = FileSystem::Retrive<Entity>("EntityInfo.txt", "Knight");
}

void Game::ProcessInput(float deltaTime) {
	if (m_Keys[GLFW_KEY_W]) {
		m_TestEntity->m_Velocity = glm::vec2(0.0f, -200.0f);
		m_TestEntity->Translate(deltaTime);
		m_TestEntity->SetRotation(270.0f);
		m_TestEntity->Flip(false);
		m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->Animate("Walking");
	}

	if (m_Keys[GLFW_KEY_S]) {
		m_TestEntity->m_Velocity = glm::vec2(0.0f, 200.0f);
		m_TestEntity->Translate(deltaTime);
		m_TestEntity->SetRotation(90.0f);
		m_TestEntity->Flip(false);
		m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->Animate("Walking");
	}

	if (m_Keys[GLFW_KEY_A]) {
		m_TestEntity->m_Velocity = glm::vec2(-200.0f, 0.0f);
		m_TestEntity->Translate(deltaTime);
		m_TestEntity->SetRotation(0.0f);
		m_TestEntity->Flip(true);
		m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->Animate("Walking");
	}

	if (m_Keys[GLFW_KEY_D]) {
		m_TestEntity->m_Velocity = glm::vec2(200.0f, 0.0f);
		m_TestEntity->Translate(deltaTime);
		m_TestEntity->SetRotation(0.0f);
		m_TestEntity->Flip(false);
		m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->Animate("Walking");
	}

	//Second part of if statement makes sure space is only detcected once. Purpose of m_KeyAllowement
	if (m_Keys[GLFW_KEY_SPACE] && m_KeyAllowment[GLFW_KEY_SPACE] == 1) {
		m_KeyAllowment[GLFW_KEY_SPACE] = 0; //Key has been processed once, don't register again until pressed again
		m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->Animate("Swing");
		m_TestEntity->GetComponent<Audio>("SFX")->PlaySound("Swing");
	}

	//Example of saving the entity information
	if (m_Keys[GLFW_KEY_K] && m_KeyAllowment[GLFW_KEY_K] == 1) {
		FileSystem::Store<Entity>("EntityInfo.txt", "Knight", *m_TestEntity, true);
	}

	if (!m_Keys[GLFW_KEY_W] && !m_Keys[GLFW_KEY_A] && !m_Keys[GLFW_KEY_S] && !m_Keys[GLFW_KEY_D]) {
		m_TestEntity->SetRotation(0.0f);
		m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->Animate("Idle");
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

	//Objects you want effected by gravity update them with GravityMovement() here in the update loop
	m_TestEntity->GravityMovement(m_Gravity, deltaTime);

	CheckCollisions();
}

void Game::Render() {
	ImGui::Begin("ImGui");
	//ImGui Code
	ImGui::End();

	//Have to draw the background first otherwise anything drawn before it will be overlayed by the background
	m_SpriteRenderer->DrawSprite(*ResourceManager::Get<Texture>("background"), glm::vec2(0.0f), glm::vec2(m_Width, m_Height));
	m_TestEntity->Draw(*m_SpriteRenderer, m_TestEntity->GetComponent<AnimationCycle>("KnightAnimation")->getSpritePos());
	m_GroundFloor->Draw(glm::vec3(0.0f, 0.0f, 1.0f));
	coolBeans.Draw(*m_SpriteRenderer, glm::ivec2(0), glm::vec3(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Game::CheckCollisions() {
	if (CollisionHandler::CollideAABB(*m_TestEntity->getHitbox(), *m_GroundFloor)) {
		std::cout << "Entity is touching the ground" << std::endl;
	}
}