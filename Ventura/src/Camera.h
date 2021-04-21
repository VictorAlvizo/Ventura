#pragma once
#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/matrix_transform.hpp"
#include "Entity.h"

class Camera {
public:
	//Default constructer of the camera, should not use it if planning to use
	Camera();
	//Provide the position, the window size, and an entity to follow (if there is one)
	Camera(glm::vec2 pos, glm::vec2 windowSize, Entity * entity = nullptr);
	~Camera();

	//Move the camera to the position
	void Move(glm::vec2 pos);
	//transPos is the vector of the direction to move, deltaTime keeps it moving in that direction gradually. 
	void Translate(glm::vec2 transPos, float deltaTime);
	//Set an entity for the camera to follow
	void SetEntity(Entity * entity);
	//If there is an entity attached to this camera, stop following it
	void Disconnect();

	//Retrive the view matrix the camera provides
	glm::mat4 UpdateView();
	
	//Get the position of the camera
	inline glm::vec2 getPos() const { return m_Pos; }
	//Get the window size. Needed for when wanting to get the area covered by the camera
	inline glm::vec2 getWindowSize() const { return m_WindowSize; }
	//Returns true if their is currently an entity it's following
	inline bool isEntityConnected() const { return m_FollowEntity; }

private:
	glm::vec2 m_Pos, m_WindowSize;

	Entity * m_FollowEntity;
};

