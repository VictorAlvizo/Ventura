#include "Game.h"

Game::Game(unsigned int screenWidth, unsigned int screenHeight, float gravity) 
	:m_Width(screenWidth), m_Height(screenHeight), m_Gravity(gravity), m_CapsActive(false)
{
	m_SpriteRenderer = nullptr;
	m_Camera = nullptr;
}

Game::~Game() {
	delete m_SpriteRenderer;
	m_SpriteRenderer = nullptr;

	delete m_Camera;
	m_Camera = nullptr;
}

void Game::Init() {
	EngineInit();
}

void Game::ProcessInput(float deltaTime) {
	//holder
}

void Game::Update(float deltaTime) {
	EngineUpdate();
	
	//holder

	CheckCollisions();
}

void Game::Render() {
	ImGui::Begin("ImGui");
	//ImGui Code
	ImGui::End();
}

void Game::CheckCollisions() {
	//holder
}

void Game::EngineInit() {
	m_Camera = new Camera(glm::vec2(0.0f), glm::vec2(m_Width, m_Height));
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);

	//Load Essential Shaders
	ResourceManager::LoadShader("src/Shaders/SpriteVertex.glsl", "src/Shaders/SpriteFragment.glsl", "sprite");
	ResourceManager::LoadShader("src/Shaders/TextVertex.glsl", "src/Shaders/TextFragment.glsl", "text");
	ResourceManager::LoadShader("src/Shaders/HBOutlineVertex.glsl", "src/Shaders/HBOutlineFragment.glsl", "hboutline");
	ResourceManager::LoadShader("src/Shaders/FilterVertex.glsl", "src/Shaders/FilterFragment.glsl", "filter");

	//Load Essential Textures
	ResourceManager::LoadTexture("Textures/HitboxCircle.png", "hitboxCircle");
	ResourceManager::LoadTexture("Textures/SliderTexture.png", "slider");
	ResourceManager::LoadTexture("Textures/ButtonTexture.png", "button");
	ResourceManager::LoadTexture("Textures/CheckboxTexture.png", "checkbox");
	ResourceManager::LoadTexture("Textures/CheckTickedTexture.png", "checkboxTicked");
	ResourceManager::LoadTexture("Textures/TextboxTexture.png", "textbox");
	ResourceManager::LoadTexture("Textures/InserationCursor.jpg", "textcursor");

	glGenBuffers(1, &m_UBOVisionBlock);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBOVisionBlock);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	unsigned int spriteUBOVision = glGetUniformBlockIndex(ResourceManager::Get<Shader>("sprite")->getProgram(), "Vision");
	unsigned int textUBOVision = glGetUniformBlockIndex(ResourceManager::Get<Shader>("text")->getProgram(), "Vision");
	unsigned int hbOutlineUBOVision = glGetUniformBlockIndex(ResourceManager::Get<Shader>("hboutline")->getProgram(), "Vision");

	glUniformBlockBinding(ResourceManager::Get<Shader>("sprite")->getProgram(), spriteUBOVision, 0);
	glUniformBlockBinding(ResourceManager::Get<Shader>("text")->getProgram(), textUBOVision, 0);
	glUniformBlockBinding(ResourceManager::Get<Shader>("hboutline")->getProgram(), hbOutlineUBOVision, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_UBOVisionBlock);

	glBindBuffer(GL_UNIFORM_BUFFER, m_UBOVisionBlock);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(m_Camera->UpdateView()));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
	//Set the default view and projection uniforms in the shaders
	ResourceManager::Get<Shader>("sprite")->Bind();
	ResourceManager::Get<Shader>("sprite")->SetMat4("u_Projection", projection);
	ResourceManager::Get<Shader>("sprite")->UnBind();

	ResourceManager::Get<Shader>("hboutline")->Bind();
	ResourceManager::Get<Shader>("hboutline")->SetMat4("u_Projection", projection);
	ResourceManager::Get<Shader>("hboutline")->UnBind();

	m_SpriteRenderer = new SpriteRenderer(ResourceManager::Get<Shader>("sprite"));
}

void Game::EngineUpdate() {
	//Update view matrix
	glm::mat4 cameraView = m_Camera->UpdateView();

	glBindBuffer(GL_UNIFORM_BUFFER, m_UBOVisionBlock);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(cameraView));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}