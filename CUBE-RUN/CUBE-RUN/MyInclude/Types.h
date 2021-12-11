#pragma once

struct Vertexes {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoordinate;
};

struct CameraVectors {
	glm::vec3 vEYE;
	glm::vec3 vAT;
	glm::vec3 vUP;

public:
	float yaw = 45.0f, pitch = 45.0f;
	float scroll = 5.0f;

public:
	void updatePos(float dx, float dy) {
		yaw -= dx;
		pitch -= dy;

		pitch = int(pitch + 90.0f) % 360 - 90.0f;
		if (pitch > 89.0f) pitch = 89.0f;
		else if (pitch < -89.0f) pitch = -89.0f;

		vEYE.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
		vEYE.y = glm::sin(glm::radians(pitch));
		vEYE.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	}

	glm::mat4 getViewMat() const {
		return glm::lookAt((vEYE  * scroll) + vAT, vAT, vUP);
	}

};