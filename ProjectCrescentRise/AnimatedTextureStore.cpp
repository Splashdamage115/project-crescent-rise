#include "AnimatedTextureStore.h"
#include "TextureStore.h"

std::vector<AnimatedTexture> AnimatedTextureStore::m_animatedTextures;

void AnimatedTextureStore::AddAnimatedTexture(std::string t_textureLocation, glm::vec2 t_frameSize, float t_frameWidth, float frameHeight, int t_frameAmt)
{
	for (unsigned int i = 0; i < m_animatedTextures.size(); i++)
	{
		if (m_animatedTextures.at(i).textureLocation == t_textureLocation)
		{
			return; // texture already exists, no need to add again
		}
	}
	AnimatedTexture newTexture;
	newTexture.textureLocation = t_textureLocation;
	newTexture.textureID = TextureStore::RetrieveTexture(t_textureLocation);
	newTexture.frameSize = t_frameSize;
	newTexture.frameWidth = t_frameWidth;
	newTexture.frameHeight = frameHeight;
	newTexture.frameAmt = t_frameAmt;

	float yPos = 0.f;
	float xPos = 0.f;
	newTexture.framePositions.push_back(glm::vec4(xPos, yPos, newTexture.frameSize.x / newTexture.frameWidth, newTexture.frameSize.y / newTexture.frameHeight));

	for (int i = 1; i < t_frameAmt; i++)
	{
		xPos += t_frameSize.x;

		if (xPos + t_frameSize.x > t_frameWidth)
		{
			xPos = 0.f;
			yPos += t_frameSize.y;
		}


		float xOffset = xPos;
		float yOffset = yPos;
		float width = newTexture.frameSize.x;
		float height = newTexture.frameSize.y;

		float u1 = xOffset / newTexture.frameWidth;
		float v1 = yOffset / newTexture.frameHeight;
		float u2 = (xOffset + width) / newTexture.frameWidth;
		float v2 = (yOffset + height) / newTexture.frameHeight;

		glm::vec4 uv = glm::vec4(u1, v1, u2, v2);
		newTexture.framePositions.push_back(uv);
	}

	m_animatedTextures.emplace_back(newTexture);
}

bool AnimatedTextureStore::RetrieveAnimatedTexture(std::string t_textureLocation, int framePosition, glm::vec4& uv)
{
	for (unsigned int i = 0; i < m_animatedTextures.size(); i++)
	{
		if (m_animatedTextures.at(i).textureLocation == t_textureLocation) 
		{
			glBindTexture(GL_TEXTURE_2D, m_animatedTextures.at(i).textureID);

			int useableFrame = framePosition % m_animatedTextures.at(i).frameAmt;
			if (framePosition < 0)
			{
				useableFrame += m_animatedTextures.at(i).frameAmt;
			}

			uv = m_animatedTextures.at(i).framePositions.at(useableFrame);


			return true;
		}
	}
	return false;
}
