#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Texture.h"

class SpriteRenderer
{
public:
	// contructor / destructor
	SpriteRenderer(Shader& shader);
	~SpriteRenderer();

	void DrawSprite(Texture2D& texture, glm::vec2 position, float resize = 1.0f, glm::vec3 color = glm::vec3(1.0f), float rotate = 0.0f);

private:
	// renderer state
	Shader shader;
	GLuint QuadVAO;

	// initialize and configure the quad's buffer and vertex attributes
	void InitRenderData();
};

