#pragma once

#include "Library.h"

struct Texture
{
	bool InitialiseTexture(std::string location);

	GLuint texture;
	std::string name;
	bool active = false;
};

struct NormalMap
{
	bool InitialiseNormal(std::string location);

	GLuint normal;
	std::string name;
	bool active = false;
};

class TextureStore
{
public:
	TextureStore() {}

	static GLuint RetrieveTexture(std::string location);
	static GLuint RetrieveNormals(std::string location);
private:
	static std::vector<Texture> textures;
	static std::vector<NormalMap> normals;
};

