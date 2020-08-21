#pragma once
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0F;
const float SPEED = 2.5F;
const float SENSITIVITY = 0.1F;
const float FOV = 45.0F;

class Camera {

private:
	float _fov = 45.0f;
	float pitch = 0.0f;
	float yaw = -90.0f;
	float roll = 0.0f;

	float movementSpeed = 2.5f;
	float mouseSensitivity = 0.1f;

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right;
	glm::vec3 worldUp = up;

public:

	Camera(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW,
		float pitch = PITCH
		)
		: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), _fov(FOV),
		position(position), worldUp(up), yaw(yaw), pitch(pitch) {

	}

	float fov() {
		return _fov;
	}

	glm::mat4 transform() {
		return glm::lookAt(position, position + front, up);
	}

	void processKeyboard(CameraMovement direction, float deltaTime) {

		float velocity = movementSpeed * deltaTime;
		if (direction == FORWARD)
			position += front * velocity;
		if (direction == BACKWARD)
			position -= front * velocity;
		if (direction == LEFT)
			position -= right * velocity;
		if (direction == RIGHT)
			position += right * velocity;
	}

    void setPosition(glm::vec3& pos) {
        this->position = pos;
    }

	void processMouseMovement(float xOffset, float yOffset) {

		xOffset *= mouseSensitivity;
		yOffset *= mouseSensitivity;

		yaw += xOffset;
		pitch += yOffset;

		if (pitch > +89.0f) pitch = +89.0f;
		if (pitch < -89.0f) pitch = -89.0f;

		updateVectors();
	}

	void processScroll(float yOffset) {

		if (_fov > 1.0f && _fov < 45.0f)
			_fov -= yOffset;
		else if (_fov <= 1.0f)
			_fov = 1.0f;
		else if (_fov >= 45.0f)
			_fov = 45.0f;
	}

private:

	void updateVectors() {
		front = glm::normalize(glm::vec3{
				cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
				sin(glm::radians(pitch)),
				sin(glm::radians(yaw)) * cos(glm::radians(pitch))
			});
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}

};