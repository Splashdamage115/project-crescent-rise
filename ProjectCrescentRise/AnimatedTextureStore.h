#pragma once

#include "Library.h"

struct AnimatedTexture
{
	std::string textureLocation;
	GLuint textureID;
	glm::vec2 frameSize;
	std::vector<glm::vec4> framePositions;
	float frameWidth;
	float frameHeight;
	int frameAmt;
};

class AnimatedTextureStore
{
public:
	static void AddAnimatedTexture(std::string t_textureLocation, glm::vec2 t_frameSize, float t_frameWidth, float t_frameHeight, int t_frameAmt);
	static bool RetrieveAnimatedTexture(std::string t_textureLocation, int framePosition, glm::vec4& uv);
private:
	static std::vector<AnimatedTexture> m_animatedTextures;
};

