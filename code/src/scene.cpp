#include "scene.h"

#include "scripts/simpleController.h"
#include "scripts/camController.h"

entt::registry Scene::m_registry;
std::shared_ptr<b2World> Scene::m_physicsWorld = nullptr;

Scene::Scene()
{
	m_physicsWorld.reset(new b2World(b2Vec2(0.f, -9.81f)));
	m_physicsWorld->SetContactListener(&m_listener);

	Renderer::setClearColour({ 0.2f, 0.f, 0.4f, 1.f });
	Renderer::enableStandardBlend();

	letterCubeTexture.reset(new Texture("../assets/textures/letterCube.png"));
	unsigned char whitePx[3] = { 255,255,255 };
	plainWhiteTexture.reset(new Texture(1, 1, 3, whitePx, 1));
	moonTexture.reset(new Texture("../assets/textures/moon.png"));

	m_character = m_registry.create(); // Create the entity
	m_registry.emplace<TransformComponent>(m_character, glm::vec2(0.2f, 0.2f), glm::vec2(0.4f, 6.5f), 0.f); // Add a transform to the block
	m_registry.emplace<RenderComponent>(m_character, moonTexture, glm::vec4(1.f)); // Add a render component
	m_registry.emplace<RigidBodyComponent>(m_character, m_character, RigidBodyType::dynamic);
	PhysicsMaterial highRes;
	highRes.restitution = 0.6f;
	m_registry.emplace<CircleColliderComponent>(m_character, m_character, 0.2f, glm::vec2(0.f), highRes); 
	{
		auto& rb = m_registry.get<RigidBodyComponent>(m_character);
		rb.body->SetSleepingAllowed(false);
	}

	m_middle = m_registry.create(); // Create the entity
	m_registry.emplace<TransformComponent>(m_middle, glm::vec2(0.2f, 0.2f), glm::vec2(0.f, 0.0f), 0.f); // Add a transform to the block
	m_registry.emplace<RenderComponent>(m_middle, moonTexture, glm::vec4(1.f)); // Add a render component
	m_registry.emplace<RigidBodyComponent>(m_middle, m_middle, RigidBodyType::dynamic);
	m_registry.emplace<CircleColliderComponent>(m_middle, m_middle, 0.2f, glm::vec2(0.f), highRes);
	{
		auto& rb = m_registry.get<RigidBodyComponent>(m_middle);
		rb.body->SetSleepingAllowed(false);
	}

	m_circleEntities.reserve(4096);
	size_t blockCount = 90;
	float radius = 7.f;
	float angle = 0;
	float deltaAngle = glm::two_pi<float>() / (float)blockCount;
	glm::vec2 he(0.1f, 0.25f);
	glm::vec4 red(1.f, 0.f, 0.f, 1.f);
	glm::vec4 blue(0.f, 0.f, 1.f, 1.f);
	glm::vec4 colour = red;

	for (size_t i = 0;i < blockCount;i++)
	{
		m_circleEntities.push_back(m_registry.create());
		glm::vec2 pos;
		pos.x = cos(angle) * radius;
		pos.y = sin(angle) * radius;
		m_registry.emplace<TransformComponent>(m_circleEntities.back(), he, pos, angle, false); // Add a transform to the block
		colour = i % 2 ? red : blue;
		m_registry.emplace<RenderComponent>(m_circleEntities.back(), plainWhiteTexture, colour); // Add a render component
		m_registry.emplace<RigidBodyComponent>(m_circleEntities.back(), m_circleEntities.back());
		m_registry.emplace<BoxColliderComponent>(m_circleEntities.back(), m_circleEntities.back());
		angle += deltaAngle;
	}

	radius = 5.5f;
	he.y = 0.2f;
	for (size_t i = 0;i < blockCount;i++)
	{
		m_circleEntities.push_back(m_registry.create());
		glm::vec2 pos;
		pos.x = cos(angle) * radius;
		pos.y = sin(angle) * radius;
		m_registry.emplace<TransformComponent>(m_circleEntities.back(), he, pos, angle, false); // Add a transform to the block
		colour = i % 2 ? red : blue;
		m_registry.emplace<RenderComponent>(m_circleEntities.back(), plainWhiteTexture, colour); // Add a render component
		m_registry.emplace<RigidBodyComponent>(m_circleEntities.back(), m_circleEntities.back());
		m_registry.emplace<BoxColliderComponent>(m_circleEntities.back(), m_circleEntities.back());
		angle += deltaAngle;
	}

	radius = 4.f;
	he.y = 0.16f;
	for (size_t i = 0;i < blockCount;i++)
	{
		m_circleEntities.push_back(m_registry.create());
		glm::vec2 pos;
		pos.x = cos(angle) * radius;
		pos.y = sin(angle) * radius;
		m_registry.emplace<TransformComponent>(m_circleEntities.back(), he, pos, angle, false); // Add a transform to the block
		colour = i % 2 ? red : blue;
		m_registry.emplace<RenderComponent>(m_circleEntities.back(), plainWhiteTexture, colour); // Add a render component
		m_registry.emplace<RigidBodyComponent>(m_circleEntities.back(), m_circleEntities.back());
		m_registry.emplace<BoxColliderComponent>(m_circleEntities.back(), m_circleEntities.back());
		angle += deltaAngle;
	}

	radius = 2.5f;
	he.y = 0.1f;
	for (size_t i = 0;i < blockCount;i++)
	{
		m_circleEntities.push_back(m_registry.create());
		glm::vec2 pos;
		pos.x = cos(angle) * radius;
		pos.y = sin(angle) * radius;
		m_registry.emplace<TransformComponent>(m_circleEntities.back(), he, pos, angle, false); // Add a transform to the block
		colour = i % 2 ? red : blue;
		m_registry.emplace<RenderComponent>(m_circleEntities.back(), plainWhiteTexture, colour); // Add a render component
		m_registry.emplace<RigidBodyComponent>(m_circleEntities.back(), m_circleEntities.back());
		m_registry.emplace<BoxColliderComponent>(m_circleEntities.back(), m_circleEntities.back());
		angle += deltaAngle;
	}

	radius = 1.f;
	he.y = 0.04f;
	for (size_t i = 0;i < blockCount;i++)
	{
		m_circleEntities.push_back(m_registry.create());
		glm::vec2 pos;
		pos.x = cos(angle) * radius;
		pos.y = sin(angle) * radius;
		m_registry.emplace<TransformComponent>(m_circleEntities.back(), he, pos, angle, false); // Add a transform to the block
		colour = i % 2 ? red : blue;
		m_registry.emplace<RenderComponent>(m_circleEntities.back(), plainWhiteTexture, colour); // Add a render component
		m_registry.emplace<RigidBodyComponent>(m_circleEntities.back(), m_circleEntities.back());
		m_registry.emplace<BoxColliderComponent>(m_circleEntities.back(), m_circleEntities.back());
		angle += deltaAngle;
	}

	colour = glm::vec4(0.f, 1.f, 0.f, 1.f);
	he.y = 0.1;
	he.x = 0.75f;

	size_t segmentCount = 6;
	deltaAngle = glm::two_pi<float>() / (float)segmentCount;

	radius = 6.25;
	angle = deltaAngle * 0.5f;
	for (size_t i = 0; i < segmentCount; i++)
	{
		m_circleEntities.push_back(m_registry.create());
		glm::vec2 pos;
		pos.x = cos(angle) * radius;
		pos.y = sin(angle) * radius;
		m_registry.emplace<TransformComponent>(m_circleEntities.back(), he, pos, angle, false); // Add a transform to the block
		m_registry.emplace<RenderComponent>(m_circleEntities.back(), plainWhiteTexture, colour); // Add a render component
		m_registry.emplace<RigidBodyComponent>(m_circleEntities.back(), m_circleEntities.back());
		m_registry.emplace<BoxColliderComponent>(m_circleEntities.back(), m_circleEntities.back());
		angle += deltaAngle;
	}

	radius -= 1.5f;
	angle = 0;
	for (size_t i = 0; i < segmentCount; i++)
	{
		m_circleEntities.push_back(m_registry.create());
		glm::vec2 pos;
		pos.x = cos(angle) * radius;
		pos.y = sin(angle) * radius;
		m_registry.emplace<TransformComponent>(m_circleEntities.back(), he, pos, angle, false); // Add a transform to the block
		m_registry.emplace<RenderComponent>(m_circleEntities.back(), plainWhiteTexture, colour); // Add a render component
		m_registry.emplace<RigidBodyComponent>(m_circleEntities.back(), m_circleEntities.back());
		m_registry.emplace<BoxColliderComponent>(m_circleEntities.back(), m_circleEntities.back());
		angle += deltaAngle;
	}


	radius -= 1.5f;
	angle = deltaAngle * 0.5f;
	for (size_t i = 0; i < segmentCount; i++)
	{
		m_circleEntities.push_back(m_registry.create());
		glm::vec2 pos;
		pos.x = cos(angle) * radius;
		pos.y = sin(angle) * radius;
		m_registry.emplace<TransformComponent>(m_circleEntities.back(), he, pos, angle, false); // Add a transform to the block
		m_registry.emplace<RenderComponent>(m_circleEntities.back(), plainWhiteTexture, colour); // Add a render component
		m_registry.emplace<RigidBodyComponent>(m_circleEntities.back(), m_circleEntities.back());
		m_registry.emplace<BoxColliderComponent>(m_circleEntities.back(), m_circleEntities.back());
		angle += deltaAngle;
	}

	radius -= 1.5f;
	angle = 0.f;
	for (size_t i = 0; i < segmentCount; i++)
	{
		m_circleEntities.push_back(m_registry.create());
		glm::vec2 pos;
		pos.x = cos(angle) * radius;
		pos.y = sin(angle) * radius;
		m_registry.emplace<TransformComponent>(m_circleEntities.back(), he, pos, angle, false); // Add a transform to the block
		m_registry.emplace<RenderComponent>(m_circleEntities.back(), plainWhiteTexture, colour); // Add a render component
		m_registry.emplace<RigidBodyComponent>(m_circleEntities.back(), m_circleEntities.back());
		m_registry.emplace<BoxColliderComponent>(m_circleEntities.back(), m_circleEntities.back());
		angle += deltaAngle;
	}
	

	m_camera = m_registry.create();
	m_registry.emplace<TransformComponent>(m_camera, glm::vec2(10.24f,8.f), glm::vec2(0.f, 0.f), 0.f); // Add a transform to the block
	m_registry.emplace<CameraComponent>(m_camera, m_camera);
	m_registry.emplace<CameraControllerComponent>(m_camera);
	m_registry.emplace<NativeScriptComponent>(m_camera);
	m_registry.get<NativeScriptComponent>(m_camera).create<CamController>(m_camera);
}


void Scene::onUpdate(float timeStep)
{
	m_angle += m_rotationSpeed * timeStep;

	if (m_angle > glm::two_pi<float>()) m_angle -= glm::two_pi<float>();
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
	float x = - sin(-m_angle) * -9.81f;
	float y = + cos(-m_angle) * -9.81f;

	m_physicsWorld->SetGravity(b2Vec2(x, y));
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
	glm::mat4 world = glm::rotate(glm::mat4(1.f), m_angle, glm::vec3(0.f, 0.f, 1.f));
	auto& cam = m_registry.get<CameraComponent>(m_camera);
	Renderer::begin(cam.view, cam.proj, world);
	Renderer::clearScreen();

	auto& renderView = m_registry.view<TransformComponent, RenderComponent>();

	for (auto& entity : renderView)
	{
		auto& transform = m_registry.get<TransformComponent>(entity);
		auto& render = m_registry.get<RenderComponent>(entity);

		Renderer::drawQuad(Quad::createCentreHalfExtents(transform.position, transform.halfExtents, transform.angle, false), *render.texture, render.tint);
	}

	Renderer::end();
}
