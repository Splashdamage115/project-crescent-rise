#pragma once

#include "RenderableObject.h"

class billboard2D : public RenderableObject
{
public:
	virtual void Start()override;
	virtual void Render()override;
private:
	GLint uColourLoc = -1;
};
