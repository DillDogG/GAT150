#include "Item.h"

void Item::Update(float dt) {
	Actor::Update(dt);
}

void Item::OnCollision(Actor* other) {
	if (other->m_tag == "Player") {
		m_destroyed = true;
	}
}
