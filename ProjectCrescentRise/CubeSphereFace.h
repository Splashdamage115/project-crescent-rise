#pragma once

#include "Library.h"
#include "ShapeGenerator.h"

class CubeSphereFace
{
public:
	static void generateFace(std::vector<float>& vertices, std::vector<unsigned int>& indices, int resolution, glm::vec3 up, int faceNum, ShapeGenerator gen);
private:

};

