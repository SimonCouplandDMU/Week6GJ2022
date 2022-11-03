#include "scene.h"

#include "scripts/simpleController.h"

entt::registry Scene::m_registry;
std::shared_ptr<b2World> Scene::m_physicsWorld = nullptr;

Scene::Scene()
{
	m_physicsWorld.reset(new b2World(b2Vec2(0.f, -9.81f)));
	m_physicsWorld->SetContactListener(&m_listener);

	Renderer::setClearColour({ 1.f, 1.f, 1.f, 1.f });

	letterCubeTexture.reset(new Texture("../assets/textures/letterCube.png"));
	unsigned char whitePx[3] = { 255,255,255 };
	plainWhiteTexture.reset(new Texture(1, 1, 3, whitePx, 1));

	m_texturedBlock = m_registry.create(); // Create the entity
	m_registry.emplace<TransformComponent>(m_texturedBlock, glm::vec2(3.f, 2.f), glm::vec2(4.f, 0.5f), -5.f); // Add a transform to the block
	m_registry.emplace<RenderComponent>(m_texturedBlock, letterCubeTexture, glm::vec4(1.f)); // Add a render component
	m_registry.emplace<RigidBodyComponent>(m_texturedBlock, m_texturedBlock); // Add a static rigid body
	m_registry.emplace<BoxColliderComponent>(m_texturedBlock, m_texturedBlock); // Add a box collider with standard physics

	m_fallingBlock = m_registry.create(); 
	m_registry.emplace<TransformComponent>(m_fallingBlock, glm::vec2(0.5f, 0.5f), glm::vec2(4.f, 7.0f), 0.f); // Add a transform to the block
	m_registry.emplace<RenderComponent>(m_fallingBlock, plainWhiteTexture, glm::vec4(1.f, 0.f, 0.f, 1.f)); // Add a render component
	m_registry.emplace<RigidBodyComponent>(m_fallingBlock, m_fallingBlock, RigidBodyType::dynamic); // Add a dyanmic rigid body
	PhysicsMaterial zeroRes;
	zeroRes.restitution = 0.f;
	m_registry.emplace<BoxColliderComponent>(m_fallingBlock, m_fallingBlock, zeroRes); // Add a box collider with 0 resistition
	m_registry.emplace<NativeScriptComponent>(m_fallingBlock);
	m_registry.get<NativeScriptComponent>(m_fallingBlock).create<SimpleController>(m_fallingBlock);

	m_camera = m_registry.create();
	m_registry.emplace<TransformComponent>(m_camera, glm::vec2(5.12f,2.f), glm::vec2(0.f, 0.0f), 0.f); // Add a transform to the block
	m_registry.emplace<CameraComponent>(m_camera, m_registry.get<TransformComponent>(m_camera));
}


void Scene::onUpdate(float timeStep)
{
	// Update any scripts
	auto& scriptView = m_registry.view<NativeScriptComponent>();
	{
		for (auto& entity : scriptView)
		{
			auto& script = m_registry.get<NativeScriptComponent>(entity);
			script.onUpdate(timeStep);
		}
	}

	// update the world
	m_physicsWorld->Step(timeStep, 5, 2);

	// update any transforms
	auto& rbView = m_registry.view<TransformComponent, RigidBodyComponent>();

	for (auto& entity : rbView)
	{
		auto& transform = m_registry.get<TransformComponent>(entity);
		auto& rigidBody = m_registry.get<RigidBodyComponent>(entity);

		if (rigidBody.bodyType != RigidBodyType::_static)
		{
			// Update transform
			auto& b2_pos = rigidBody.body->GetPosition();
			transform.position = glm::vec2(b2_pos.x, b2_pos.y);
			transform.angle = rigidBody.body->GetAngle();
		}
	}
}

void Scene::onRender()
{
	Renderer::clearScreen();

	auto& renderView = m_registry.view<TransformComponent, RenderComponent>();

	for (auto& entity : renderView)
	{
		auto& transform = m_registry.get<TransformComponent>(entity);
		auto& render = m_registry.get<RenderComponent>(entity);

		Renderer::drawQuad(Quad::createCentreHalfExtents(transform.position, transform.halfExtents, transform.angle, false), *render.texture, render.tint);
	}
}
