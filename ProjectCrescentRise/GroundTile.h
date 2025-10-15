#pragma once

#include "RenderableObject.h"

class GroundTile : public RenderableObject
{
public:
	virtual void Start()override;
	virtual void Render()override;
private:
	const unsigned int pointLength = 20;
	const unsigned int hop = 3;
};

