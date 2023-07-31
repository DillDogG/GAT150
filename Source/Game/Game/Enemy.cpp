#include "Enemy.h"
#include "Framework/Scene.h"
#include "Player.h"
#include "SpaceGame.h"
#include "Weapon.h"
#include "Renderer/Renderer.h"
#include "Framework/Emitter.h"

void Enemy::Update(float dt) {
	Actor::Update(dt);


	kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(m_transform.rotation);
	float angle = 0;
	Player* player = m_scene->GetActor<Player>();
	if (player) {
		kiko::Vector2 direction = player->m_transform.position - m_transform.position;
		float turnAngle = kiko::vec2::SignedAngle(forward, direction.Normalized());
		m_transform.rotation += turnAngle * 3 * dt;

		angle = kiko::vec2::Angle(forward, direction.Normalized());
		
	}

	m_transform.position += forward * m_speed * kiko::g_time.GetDeltaTime();
	m_transform.position.x = kiko::Wrap(m_transform.position.x, (float)kiko::g_renderer.GetWidth());
	m_transform.position.y = kiko::Wrap(m_transform.position.y, (float)kiko::g_renderer.GetHeight());

	if (m_fireTimer < 0 && angle < kiko::DegreesToRadians(30.0f)) {
		kiko::Transform transform { m_transform.position, m_transform.rotation, 1 };
		std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(400.0f, m_transform, m_model);
		weapon->m_transform.scale /= 2;
		weapon->m_tag = "eWeapon";
		m_scene->Add(std::move(weapon));
		m_fireTimer = m_fireRate;
	}
	else {
		m_fireTimer -= dt;
	}
}

void Enemy::OnCollision(Actor* other) {
	if (other->m_tag == "pWeapon") {
		m_game->AddPoints(100);
		kiko::EmitterData data;
		data.burst = true;
		data.burstCount = 100;
		data.spawnRate = 200;
		data.angle = 0;
		data.angleRange = kiko::Pi;
		data.lifetimeMin = 0.5f;
		data.lifetimeMax = 1.5f;
		data.speedMin = 50;
		data.speedMax = 250;
		data.damping = 0.5f;
		data.color = kiko::Color{ 0, 1, 1, 1 };
		auto emitter = std::make_unique<kiko::Emitter>(m_transform, data);
		emitter->m_lifespan = 1.0f;
		m_scene->Add(std::move(emitter));
		m_destroyed = true;
	}
}
