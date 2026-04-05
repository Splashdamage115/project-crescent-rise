#include "GunAttachment.h"
#include "Window.h"

void GunAttachment::Update()
{
	if (!transform)
	{
		return;
	}


	glm::mat4 view = Window::Get().GetView();
	glm::mat4 invView = glm::inverse(view);

	glm::vec3 camPos = glm::vec3(invView[3]);
	glm::vec3 right = glm::normalize(glm::vec3(invView[0]));
	glm::vec3 up = glm::normalize(glm::vec3(invView[1]));
	glm::vec3 forward = glm::normalize(-glm::vec3(invView[2]));

	glm::mat3 basis(right, up, forward);

	float r20 = basis[0][2];
	float r21 = basis[1][2];
	float r22 = basis[2][2];
	float r10 = basis[0][1];
	float r00 = basis[0][0];

	float rotY = asinf(-r20);
	float rotX = atan2f(r21, r22);
	float rotZ = atan2f(r10, r00);

	transform->position = camPos;
	transform->rotation = glm::degrees(glm::vec3(rotX, rotY, rotZ));
}
