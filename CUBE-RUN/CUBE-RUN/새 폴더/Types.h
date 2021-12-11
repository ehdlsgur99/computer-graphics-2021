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

};
