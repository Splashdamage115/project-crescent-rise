#include "CubeSphereFace.h"

void CubeSphereFace::generateFace(std::vector<float>& vertices, std::vector<unsigned int>& indices, int resolution, glm::vec3 up, int faceNum, ShapeGenerator& gen)
{
	glm::vec3 axisA = glm::vec3(up.y, up.z, up.x);
	glm::vec3 axisB = glm::cross(up, axisA);

	int triIndex = faceNum * (resolution - 1) * (resolution - 1) * 6;
	//if(multiplierSkip > 0)
	//	triIndex += (multiplierSkip - 1) * ((resolution - 1) * (resolution - 1) * 6);

	int pointAmt = resolution;

	for (int y = 0; y < pointAmt; y++)
	{
		for (int x = 0; x < pointAmt; x++)
		{
			int i = x + y * pointAmt;
			int vertexIndex = i + (faceNum * resolution * resolution);
			//if (multiplierSkip > 0)
			//	vertexIndex += (multiplierSkip - 1) * (faceNum * pointAmt * pointAmt);
			int vertNum = vertexIndex * 8;
			glm::vec2 percent = glm::vec2((float)x / (float)(pointAmt - 1), (float)y / (float)(pointAmt - 1));
			glm::vec3 pointOnUnitCube = up + (percent.x - 0.5f) * 2 * axisA + (percent.y - 0.5f) * 2 * axisB;
			glm::vec3 pointOnUnitSphere = glm::normalize(pointOnUnitCube);
			glm::vec3 displacedPoint = gen.CalcualtePointOnPlanet(pointOnUnitSphere);
			glm::vec3 normal = glm::normalize(displacedPoint);
			vertices.at(vertNum) = displacedPoint.x;
			vertices.at(vertNum + 1) = displacedPoint.y;
			vertices.at(vertNum + 2) = displacedPoint.z;
			vertices.at(vertNum + 3) = percent.x; //     u
			vertices.at(vertNum + 4) = percent.y; //     v
			vertices.at(vertNum + 5) = normal.x;   //     normal x
			vertices.at(vertNum + 6) = normal.y;   //     normal y
			vertices.at(vertNum + 7) = normal.z;   //     normal z

			if (x != resolution - 1 && y != resolution - 1)
			{
				// triangle 1
				indices.at(triIndex) = vertexIndex;
				indices.at(triIndex + 1) = vertexIndex + pointAmt + 1;
				indices.at(triIndex + 2) = vertexIndex + pointAmt;

				// triangle 2
				indices.at(triIndex + 3) = vertexIndex;
				indices.at(triIndex + 4) = vertexIndex + 1;
				indices.at(triIndex + 5) = vertexIndex + pointAmt + 1;

				triIndex += 6;
			}
		}
	}
}

void CubeSphereFace::generateFace(std::vector<float>& vertices, std::vector<unsigned int>& indices, int resolution, glm::vec3 up, int faceNum)
{
	glm::vec3 axisA = glm::vec3(up.y, up.z, up.x);
	glm::vec3 axisB = glm::cross(up, axisA);

	int triIndex = faceNum * (resolution - 1) * (resolution - 1) * 6;

	for (int y = 0; y < resolution; y++)
	{
		for (int x = 0; x < resolution; x++)
		{
			int i = x + y * resolution;
			int vertexIndex = i + (faceNum * resolution * resolution);
			int vertNum = vertexIndex * 8;
			glm::vec2 percent = glm::vec2((float)x / (float)(resolution - 1), (float)y / (float)(resolution - 1));
			glm::vec3 pointOnUnitCube = up + (percent.x - 0.5f) * 2 * axisA + (percent.y - 0.5f) * 2 * axisB;
			glm::vec3 pointOnUnitSphere = glm::normalize(pointOnUnitCube);
			vertices.at(vertNum) = pointOnUnitSphere.x;
			vertices.at(vertNum + 1) = pointOnUnitSphere.y;
			vertices.at(vertNum + 2) = pointOnUnitSphere.z;
			vertices.at(vertNum + 3) = percent.x; //     u
			vertices.at(vertNum + 4) = percent.y; //     v

			if (x != resolution - 1 && y != resolution - 1)
			{
				// triangle 1
				indices.at(triIndex) = vertexIndex;
				indices.at(triIndex + 1) = vertexIndex + resolution + 1;
				indices.at(triIndex + 2) = vertexIndex + resolution;

				// triangle 2
				indices.at(triIndex + 3) = vertexIndex;
				indices.at(triIndex + 4) = vertexIndex + 1;
				indices.at(triIndex + 5) = vertexIndex + resolution + 1;

				triIndex += 6;
			}
		}
	}
}
