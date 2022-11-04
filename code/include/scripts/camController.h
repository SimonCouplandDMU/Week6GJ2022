#pragma once
#include "components.h"
#include "input.h"
#include <iostream>

class CamController : public NativeScript
{
public:
	CamController(entt::entity& entity) : NativeScript(entity) {}

	void onUpdate(float timestep) override 
	{
		// Get data
		auto& cam = m_registry.get<CameraComponent>(m_entity);
		auto& ctrl = m_registry.get<CameraControllerComponent>(m_entity);
		auto& tc = m_registry.get<TransformComponent>(m_entity);
		// Do some control
		if (Input::isKeyPressed(GLFW_KEY_I)) tc.position.y += timestep * ctrl.linearSpeed.y; // Up
		if (Input::isKeyPressed(GLFW_KEY_K)) tc.position.y -= timestep * ctrl.linearSpeed.y; // Down
		if (Input::isKeyPressed(GLFW_KEY_J)) tc.position.x -= timestep * ctrl.linearSpeed.x; // Left
		if (Input::isKeyPressed(GLFW_KEY_L)) tc.position.x += timestep * ctrl.linearSpeed.x; // right
		if (Input::isKeyPressed(GLFW_KEY_U)) tc.angle += timestep * ctrl.angularSpeed; // CCW
		if (Input::isKeyPressed(GLFW_KEY_O)) tc.angle -= timestep * ctrl.angularSpeed; // CW

		if (Input::isKeyPressed(GLFW_KEY_Y)) tc.halfExtents += tc.halfExtents * (timestep * ctrl.zoomSpeed);
		if (Input::isKeyPressed(GLFW_KEY_P)) tc.halfExtents -= tc.halfExtents * (timestep * ctrl.zoomSpeed);

		cam.onUpdate(timestep);
	};

};
