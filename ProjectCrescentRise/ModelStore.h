#pragma once

#include "Library.h"
#include "ufbx.h"
#include "RenderableObject.h"

struct meshStorePair
{
	std::string first;
	renderObject second;
};

class MeshStore
{
public:
	static renderObject getMesh(std::string meshLocation);
private:
	static renderObject loadMesh(const ufbx_mesh* mesh_fbx, const ufbx_scene* scene);
	static std::vector<meshStorePair> m_meshStore;
};