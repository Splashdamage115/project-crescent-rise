#pragma once

#include "ScriptObject.h"
class EnemyAbstractState;
class GameObject;

class EnemyStateManager : public ScriptObject
{
public:
	void Start()override;
	void Update()override;

	void EnterNewState(std::shared_ptr<EnemyAbstractState> newState);

	bool checkPlayerVisibility(float t_distance);
	
	glm::vec3 playerPosition = glm::vec3(0.f);


	std::shared_ptr<GameObject> playerObject;
private:
	std::shared_ptr<EnemyAbstractState> m_currentState;

};

