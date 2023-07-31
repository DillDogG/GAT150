#pragma once
#include "Framework/Actor.h"

class Item : public kiko::Actor {
public:
	Item(const kiko::Transform& transform, std::shared_ptr<kiko::Model> model, float lifeSpan) :
		Actor{ transform, model }
	{
		m_lifespan = lifeSpan;
	}
	void Update(float dt) override;
	void OnCollision(Actor* other) override;
};