#include "GunAttachment.h"
#include "Window.h"

void GunAttachment::Update()
{
	if (!transform)
	{
		return;
	}

	// Get the view matrix from the camera and invert it to obtain the camera's world-space transform.
	// The view matrix converts world space -> camera space, so its inverse gives us camera space -> world space.
	glm::mat4 view = Window::Get().GetView();
	glm::mat4 invView = glm::inverse(view);

	// Extract the camera's world-space position and orientation from the inverse view matrix.
	// GLM matrices are column-major, so each column represents an axis or translation.
	glm::vec3 camPos = glm::vec3(invView[3]);                  // Column 3: world-space position
	glm::vec3 right = glm::normalize(glm::vec3(invView[0]));   // Column 0: right vector
	glm::vec3 up = glm::normalize(glm::vec3(invView[1]));      // Column 1: up vector
	glm::vec3 forward = glm::normalize(-glm::vec3(invView[2]));// Column 2: forward vector (negated for OpenGL's -Z convention)

	// Build a 3x3 rotation basis matrix from the camera's direction vectors.
	glm::mat3 basis(right, up, forward);

	// Extract individual matrix elements for Euler angle decomposition.
	// Variable names use rROWCOL notation to match standard rotation matrix element indexing.
	// Note: GLM is column-major so basis[col][row], e.g. basis[0][2] = row 2, col 0 = r20.
	float r20 = basis[0][2];
	float r21 = basis[1][2];
	float r22 = basis[2][2];
	float r10 = basis[0][1];
	float r00 = basis[0][0];

	// Decompose the rotation matrix into Euler angles using the standard ZYX decomposition.
	// Note: this decomposition has gimbal lock when rotY approaches +/-90 degrees.
	float rotY = asinf(-r20);        // Yaw   (Y-axis rotation)
	float rotX = atan2f(r21, r22);   // Pitch (X-axis rotation)
	float rotZ = atan2f(r10, r00);   // Roll  (Z-axis rotation)

	// Apply the camera's world-space position and rotation to the gun's transform,
	// making the gun follow the camera exactly (first-person weapon attachment).
	transform->position = camPos;
	transform->rotation = glm::degrees(glm::vec3(rotX, rotY, rotZ));
}
