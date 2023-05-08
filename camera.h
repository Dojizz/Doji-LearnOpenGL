#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

extern int screenWidth, screenHeight;

class Camera {
private:
	float fov, near, far, pitch, yaw, speed;
	int width, height;
	glm::vec3 position, front;

public:
	enum Direction { FORWARD, BACK, LEFT, RIGHT };
	const static glm::vec3 upVector;
	const static float minFov, maxFov;
	Camera(float fov, int width, int height);
	// set
	void SetSpeed(float newSpeed);
	void SetPitchYaw(float newPitch, float newYaw);
	void SetFov(float newFov);
	// get
	const glm::vec3 GetPos();
	const void GetPitchYaw(float& p, float& y);
	const glm::vec3 GetFront();
	const float GetFov();
	const glm::mat4 GetViewMatrix();
	const glm::mat4 GetProjMatrix();
	//move
	void MoveCam(Direction d, float dT);
};

const glm::vec3 Camera::upVector = glm::vec3(0.0f, 1.0f, 0.0f);
const float Camera::minFov = 1.0f;
const float Camera::maxFov = 1.0f;

Camera::Camera(float fov = 45.0f, int width = 800, int height = 600){

	if (width != screenWidth || height != screenHeight) {
		std::cout << "WARNING::CAMERA::CONSTRUCTION::WIDTH_OR_HEIGHT_NOT_MATCH_WITH_SCREEN" << std::endl;
	}

	this->fov = fov;
	this->width = width;
	this->height = height;
	this->near = 0.1f;
	this->far = 100.f;
	this->pitch = 0.0f;
	this->yaw = 0.0f;
	this->speed = 5.0f;
	this->position = glm::vec3(0.0f, 0.0f, 3.0f);
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);
}

void Camera::SetSpeed(float newSpeed) {
	this->speed = newSpeed;
}

void Camera::SetPitchYaw(float newPitch, float newYaw) {
	this->pitch = newPitch;
	if (newPitch > 89.0f)
		this->pitch = 89.0f;
	if (newPitch < -89.0f)
		this->pitch = -89.0f;
	this->yaw = newYaw;

	// update camera front
	float camY = sin(glm::radians(pitch));
	float camX = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	float camZ = cos(glm::radians(pitch)) * -cos(glm::radians(yaw));
	this->front = glm::normalize(glm::vec3(camX, camY, camZ));
}

void Camera::SetFov(float newFov) {
	if (newFov >= minFov && newFov <= maxFov)
		this->fov = newFov;
}

const glm::vec3 Camera::GetPos() {
	return position;
}

const float Camera::GetFov() {
	return fov;
}

const void Camera::GetPitchYaw(float& p, float& y) {
	p = pitch;
	y = yaw;
}

const glm::vec3 Camera::GetFront() {
	return this->front;
}

const glm::mat4 Camera::GetViewMatrix() {
	/*glm::mat4 translate;
	for (int i = 0; i <= 2; i++) {
		translate[i][3] -= position[i];
	}
	glm::mat4 rotation;
	glm::vec3 vectorZ = glm::normalize(-front);
	glm::vec3 vectorX = glm::normalize(glm::cross(upVector, vectorZ));
	glm::vec3 vectorY = glm::normalize(glm::cross(vectorZ, vectorX));
	for (int i = 0; i <= 2; i++) {
		rotation[0][i] = vectorX[i];
		rotation[1][i] = vectorY[i];
		rotation[2][i] = vectorZ[i];
	}
	return rotation * translate;*/
	return glm::lookAt(position, position + front, upVector);
}

const glm::mat4 Camera::GetProjMatrix() {
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(fov), width / (float)height, near, far);
	return projection;
}


glm::vec3 DropY(glm::vec3 v) {
	v.y = 0;
	return glm::normalize(v);
}

void Camera::MoveCam(Camera::Direction d, float dT) {
	float dist = dT * speed;
	// FPS style move
	/*switch (d) {
	case Camera::FORWARD:
		position += DropY(front) * dist;
		break;
	case Camera::BACK:
		position -= DropY(front) * dist;
		break;
	case Camera::LEFT:
		position += DropY(glm::cross(upVector, front)) * dist;
		break;
	case Camera::RIGHT:
		position -= DropY(glm::cross(upVector, front)) * dist;
		break;
	}*/

	switch (d) {
	case Camera::FORWARD:
		position += glm::normalize(front) * dist;
		break;
	case Camera::BACK:
		position -= glm::normalize(front) * dist;
		break;
	case Camera::LEFT:
		position += glm::normalize(glm::cross(upVector, front)) * dist;
		break;
	case Camera::RIGHT:
		position -= glm::normalize(glm::cross(upVector, front)) * dist;
		break;
	}
}


#endif // !CAMERA_H


