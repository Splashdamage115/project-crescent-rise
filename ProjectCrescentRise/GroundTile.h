#pragma once

#include "RenderableObject.h"

class GroundTile : public RenderableObject
{
public:
	virtual void Start()override;
	virtual void Render()override;
private:
	void generateVornoi(std::vector<std::vector<float>> &t_heights, int width);

	int pointAmountVornoi = 256;
	float scaleFactor = 0.1f;
	int width = 128;
	float disp = 1.f;
	float tileSize = 0.5f;
	float heightFactor = 0.1f;

	const unsigned int pointLength = 20;
	const unsigned int hop = 3;
	int size = 0;
};

