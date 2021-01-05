#pragma once
#include "SpriteRenderer.h"

class HitboxRenderer : public SpriteRenderer
{
public:
	HitboxRenderer(std::shared_ptr<Shader> shader);
	~HitboxRenderer();

	void DrawOutline(glm::vec2 pos, glm::vec2 size, float rotate = 0.0f, glm::vec3 outlineColor = glm::vec3(1.0f));

private:
	void init() override;
};

