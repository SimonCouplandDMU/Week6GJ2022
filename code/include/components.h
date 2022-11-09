#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "texture.h"
#include <box2d/box2d.h>
#include <entt/entt.hpp>


struct TransformComponent
{
	TransformComponent(const glm::vec2& p_halfExtents, const glm::vec2& p_position, float p_angle, bool degrees = true)
	{
		halfExtents = p_halfExtents;
		position = p_position;
		if (degrees) angle = glm::radians(p_angle);
		else angle = p_angle;
	}

	glm::vec2 position;
	glm::vec2 halfExtents;
	float angle;
};

struct RenderComponent
{
	RenderComponent(const std::shared_ptr<Texture>& tex, const glm::vec4& colour)
	{
		texture = tex;
		tint = colour;
	}

	std::shared_ptr<Texture> texture = nullptr;
	glm::vec4 tint = glm::vec4(1.f);
};

struct CameraComponent
{
	CameraComponent(entt::entity e);
	void onUpdate(float timestep);

	glm::mat4 view = glm::inverse(glm::mat4(1.f));
	glm::mat4 proj = glm::mat4(1.f);
	entt::entity entity;
};

struct CameraControllerComponent
{
	CameraControllerComponent() {}
	CameraControllerComponent(const glm::vec2& speed, float rotationSpeed, float zoom) 
		: linearSpeed(speed), angularSpeed(rotationSpeed), zoomSpeed(zoom) {}
	glm::vec2 linearSpeed = glm::vec2(1.f, 1.f);
	float angularSpeed = 0.1f;
	float zoomSpeed = 1.1f;
};

struct CollisionData
{
	CollisionData(entt::entity e) : other(e) {}
	entt::entity other;
};

class NativeScript
{
public:
	NativeScript();
	NativeScript(entt::entity& entity);
	virtual ~NativeScript() = default;
	virtual void onCreate() {};
	virtual void onDestroy() {};
	virtual void onUpdate(float timestep) {};
	virtual void onCollision(CollisionData& collisionData) {};
	bool isRegisteredForDeletion() { return m_isRegisteredForDeletion; }
protected:
	entt::entity m_entity;
	entt::registry& m_registry;
	bool m_isRegisteredForDeletion = false;
};

class NativeScriptComponent
{
public:
	NativeScriptComponent() = default;
	~NativeScriptComponent() = default;
	template <typename G, typename ...Args> void create(Args&& ... args)
	{
		G* ptr = new G(std::forward<Args>(args) ...);
		m_instance.reset(static_cast<NativeScript*>(ptr));
		m_instance->onCreate();
	};
	void onUpdate(float timestep) { m_instance->onUpdate(timestep); };
	bool isRegisteredForDeletion() { return m_instance->isRegisteredForDeletion(); }
	void onCollision(CollisionData& collisionData) { m_instance->onCollision(collisionData); }
protected:
	std::shared_ptr<NativeScript> m_instance = nullptr;
};

#pragma region physics

enum class RigidBodyType { dynamic, _static, kinematic };

struct PhysicsMaterial
{
	float friction = 0.2f;
	float restitution = 0.4f;
	float density = 1.0f;
	bool isSensor = false;
};

struct RigidBodyComponent
{
	RigidBodyComponent(entt::entity entity, RigidBodyType p_bodyType = RigidBodyType::_static); 	// Assumes we already have a rendercomponent and transform component
	void setBodyType(RigidBodyType bodyType)
	{
		if (body)
		{
			switch (bodyType)
			{
			case RigidBodyType::dynamic:
				body->SetType(b2BodyType::b2_dynamicBody);
				bodyType = RigidBodyType::dynamic;
				break;
			case RigidBodyType::_static:
				body->SetType(b2BodyType::b2_staticBody);
				bodyType = RigidBodyType::_static;
				break;
			case RigidBodyType::kinematic:
				body->SetType(b2BodyType::b2_kinematicBody);
				bodyType = RigidBodyType::kinematic;
				break;
			}
		}
	}

	b2Body* body = nullptr;
	RigidBodyType bodyType = RigidBodyType::_static;
};


struct BoxColliderComponent
{
	BoxColliderComponent(entt::entity entity, const PhysicsMaterial& material = PhysicsMaterial())
		: BoxColliderComponent(entity, glm::vec2(0.f, 0.f), material) {}


	BoxColliderComponent(entt::entity entity, const glm::vec2& p_offset, const PhysicsMaterial& material = PhysicsMaterial());
	BoxColliderComponent(entt::entity entity, const glm::vec2& p_offset, const glm::vec2& p_halfExtents, const PhysicsMaterial& material = PhysicsMaterial());
		
	b2Fixture* collider;
	glm::vec2 halfExtents = glm::vec2(0.f);
	glm::vec2 offset = glm::vec2(0.F);
};

struct CircleColliderComponent
{
	CircleColliderComponent(entt::entity entity, float radius, const PhysicsMaterial& material = PhysicsMaterial())
		: CircleColliderComponent(entity, radius, glm::vec2(0.f, 0.f), material) {}

	CircleColliderComponent(entt::entity entity, float p_radius, const glm::vec2& p_offset, const PhysicsMaterial& material = PhysicsMaterial());
	float radius;
	glm::vec2 offset = glm::vec2(0.f);
	b2Fixture* collider;
};

struct ColliderGroupComponent
{
	ColliderGroupComponent(entt::entity entity, const PhysicsMaterial& material = PhysicsMaterial())
	{
		boxColliders.emplace_back(entity, material);
	}

	ColliderGroupComponent(entt::entity entity, const glm::vec2& offset, const PhysicsMaterial& material = PhysicsMaterial())
	{
		boxColliders.emplace_back(entity, offset, material);
	}

	ColliderGroupComponent(entt::entity entity, const glm::vec2& offset, const glm::vec2& halfExtents, const PhysicsMaterial& material = PhysicsMaterial())
	{
		boxColliders.emplace_back(entity, offset, halfExtents, material);
	}

	

	void addBoxCollider(entt::entity entity, const PhysicsMaterial& material = PhysicsMaterial()) { boxColliders.emplace_back(entity, material); }
	void addBoxCollider(entt::entity entity, const glm::vec2& offset, const PhysicsMaterial& material = PhysicsMaterial()) { boxColliders.emplace_back(entity, offset, material); }
	void addBoxCollider(entt::entity entity, const glm::vec2& offset, const glm::vec2& halfExtents, const PhysicsMaterial& material = PhysicsMaterial()) { boxColliders.emplace_back(entity, offset, halfExtents, material); }
	
	std::vector<BoxColliderComponent> boxColliders;
};



class CollisionListener : public b2ContactListener
{
public:
	CollisionListener();
	virtual void BeginContact(b2Contact* contact) override
	{
		entt::entity entityA = static_cast<entt::entity>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
		entt::entity entityB = static_cast<entt::entity>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

		if (m_registry.any_of<NativeScriptComponent>(entityA))
		{
			auto& nsc = m_registry.get<NativeScriptComponent>(entityA);
			nsc.onCollision(CollisionData(entityB));
		}

		if (m_registry.any_of<NativeScriptComponent>(entityB))
		{
			auto& nsc = m_registry.get<NativeScriptComponent>(entityB);
			nsc.onCollision(CollisionData(entityA));
		}
	}
private:
	entt::registry& m_registry;
};

#pragma endregion