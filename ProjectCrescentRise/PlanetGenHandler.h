#pragma once

#include "Library.h"

class CubeSphere;

class PlanetGenHandler
{
public:
	void init(std::shared_ptr<CubeSphere>);
	void guiRender();
private:
	std::shared_ptr<CubeSphere> m_planet;
};

