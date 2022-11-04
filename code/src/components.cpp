#include "components.h"
#include "scene.h"

NativeScript::NativeScript() : m_entity(entt::null), m_registry(Scene::getRegistry()) {};
NativeScript::NativeScript(entt::entity& entity) : m_entity(entity), m_registry(Scene::getRegistry()) {};

CameraComponent::CameraComponent(entt::entity e)
{
	entity = e;
	entt::registry& registry = Scene::getRegistry();
	auto& transform = registry.get<TransformComponent>(entity);
	
	onUpdate(0.f);
}


void CameraComponent::onUpdate(float timestep)
{
	entt::registry& registry = Scene::getRegistry();
	auto& transform = registry.get<TransformComponent>(entity);

	proj = glm::ortho(-transform.halfExtents.x, transform.halfExtents.x, -transform.halfExtents.y, transform.halfExtents.y);

	glm::mat4 transformMatrix = glm::rotate(glm::translate(glm::mat4(1.f), glm::vec3(transform.position, 0.0)), transform.angle, { 0.f, 0.f, 1.f });
	view = glm::inverse(transformMatrix);
};

RigidBodyComponent::RigidBodyComponent(entt::entity entity, RigidBodyType p_bodyType) 	// Assumes we already have a rendercomponent and transform component
{
	entt::registry& registry = Scene::getRegistry();
	auto& tc = registry.get<TransformComponent>(entity);

	b2BodyDef bodyDef;
	bodyDef.angle = tc.angle;
	bodyDef.position = b2Vec2(tc.position.x, tc.position.y);
	switch (p_bodyType)
	{
	case RigidBodyType::dynamic:
		bodyDef.type = b2BodyType::b2_dynamicBody;
		break;
	case RigidBodyType::_static:
		bodyDef.type = b2BodyType::b2_staticBody;
		break;
	case RigidBodyType::kinematic:
		bodyDef.type = b2BodyType::b2_kinematicBody;
		break;
	}
	bodyType = p_bodyType;
	body = Scene::getWorld()->CreateBody(&bodyDef);
	body->GetUserData().pointer = static_cast<uintptr_t>(entity);
}

BoxColliderComponent::BoxColliderComponent(entt::entity entity, const glm::vec2& p_offset, const PhysicsMaterial& material)
	: offset(p_offset)
{
	entt::registry& registry = Scene::getRegistry();
	auto& rbc = registry.get<RigidBodyComponent>(entity);
	auto& tc = registry.get<TransformComponent>(entity);

	b2FixtureDef fixtureDef;
	b2PolygonShape box;
	box.SetAsBox(tc.halfExtents.x, tc.halfExtents.y);
	halfExtents = tc.halfExtents;
	b2Vec2 b2Offset(p_offset.x, p_offset.y);
	box.m_vertices[0] += b2Offset;
	box.m_vertices[1] += b2Offset;
	box.m_vertices[2] += b2Offset;
	box.m_vertices[3] += b2Offset;
	fixtureDef.shape = &box;

	fixtureDef.density = material.density;
	fixtureDef.restitution = material.restitution;
	fixtureDef.friction = material.friction;
	fixtureDef.isSensor = material.isSensor;

	collider = rbc.body->CreateFixture(&fixtureDef);
}

BoxColliderComponent::BoxColliderComponent(entt::entity entity, const glm::vec2& p_offset, const glm::vec2& p_halfExtents, const PhysicsMaterial& material)
	: offset(p_offset), halfExtents(p_halfExtents)
{
	entt::registry& registry = Scene::getRegistry();
	auto& rbc = registry.get<RigidBodyComponent>(entity);

	b2FixtureDef fixtureDef;
	b2PolygonShape box;
	box.SetAsBox(p_halfExtents.x, p_halfExtents.y);
	b2Vec2 b2Offset(offset.x, offset.y);
	box.m_vertices[0] += b2Offset;
	box.m_vertices[1] += b2Offset;
	box.m_vertices[2] += b2Offset;
	box.m_vertices[3] += b2Offset;
	fixtureDef.shape = &box;

	fixtureDef.density = material.density;
	fixtureDef.restitution = material.restitution;
	fixtureDef.friction = material.friction;
	fixtureDef.isSensor = material.isSensor;

	collider = rbc.body->CreateFixture(&fixtureDef);
}

CollisionListener::CollisionListener() : m_registry(Scene::getRegistry()) {}