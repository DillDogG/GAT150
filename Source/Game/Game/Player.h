#pragma once
#include "Framework/Actor.h"

class Player : public kiko::Actor {
public:
	enum epState {
		Normal,
		ExtraShot,
		Shield,
		RapidFire
	};
public:
	Player(float speed, float turnRate, const kiko::Transform& transform, std::shared_ptr<kiko::Model> model) :
		Actor{ transform, model },
		m_speed{ speed },
		m_turnRate{ turnRate}
	{
		m_fireRate = 1;
		m_fireTimer = m_fireRate;
		m_missileCount = 5;
		m_missileRate = 0.5f;
		m_missileTimer = m_missileRate;
		m_adrenaline = 50;
		m_immuneTime = 2.5f;
		m_immuneTimer = m_immuneTime;
		m_health = 2;
	}
	void Update(float dt) override;
	void OnCollision(Actor* other) override;
	int GetHealth() { return m_health; }
	int GetMissile() { return m_missileCount; }
	int GetAdrenaline() { return m_adrenaline; }

	epState m_pState = epState::Normal;
private:
	float m_speed = 0;
	float m_turnRate = 0;
	int m_health = 0;

	float m_fireRate = 0;
	float m_fireTimer = 0;
	int m_missileCount = 0;
	float m_missileRate = 0;
	float m_missileTimer = 0;
	float m_adrenaline = 0;
	float m_immuneTime = 0;
	float m_immuneTimer = 0;
	float m_powerTimer = 0;
};