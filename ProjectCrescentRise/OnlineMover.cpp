#include "OnlineMover.h"

void OnlineMover::Update()
{
	transform->position = position.load();
}
