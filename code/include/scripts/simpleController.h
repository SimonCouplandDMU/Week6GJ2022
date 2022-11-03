#include "components.h"
#include "input.h"
#include <iostream>

class SimpleController : public NativeScript
{
public:
	SimpleController(entt::entity& entity) : NativeScript(entity) {}
	void onCreate() override 
	{
		auto& rc = m_registry.get<RenderComponent>(m_entity);
		m_colour = rc.tint;
	};

	void onUpdate(float timestep) override 
	{
		// Change colour
		m_colour += glm::vec4(timestep, timestep * 0.5f, timestep * 0.25f, 0.f);
		if (m_colour.x > 0.8f) m_colour.x -= 1.f;
		if (m_colour.y > 0.8f) m_colour.y -= 1.f;
		if (m_colour.z > 0.8f) m_colour.z -= 1.f;

		auto& rc = m_registry.get<RenderComponent>(m_entity);
		rc.tint = m_colour;

		// Do some control
		auto& rb = m_registry.get<RigidBodyComponent>(m_entity);

		if (Input::isKeyPressed(GLFW_KEY_SPACE))
		{
			// Jump
			rb.body->ApplyLinearImpulseToCenter(b2Vec2(0.f, 0.3f), true);
		}
		
	};

	void onCollision(CollisionData& collisionData) override {
		std::cout << "hit\n";
	};
private:
	glm::vec4 m_colour = glm::vec4(1.f);
};