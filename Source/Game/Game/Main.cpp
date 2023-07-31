#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"
#include "Framework/Actor.h"
#include "Player.h"
#include "Enemy.h"
#include "Audio/AudioSystem.h"
#include "Renderer/ModelManager.h"
#include "Framework/Scene.h"
#include "Renderer/Text.h"
#include "SpaceGame.h"
#include "Framework/Emitter.h"
#include <iostream>
#include <thread>

using namespace std;

class Star {
public:
	Star(kiko::vec2 pos, kiko::vec2 vel) : m_pos{ pos }, m_vel{ vel } {}
	void Update() {
		m_pos += m_vel * kiko::g_time.GetDeltaTime();
	}
	void Wrap(int w, int h) {
		if (m_pos.x < 0) {
			m_pos.x = w;
		}
		if (m_pos.x > w) {
			m_pos.x = 0;
		}
		if (m_pos.y < 0) {
			m_pos.y = h;
		}
		if (m_pos.y > h) {
			m_pos.y = 0;
		}
	}
public:
	kiko::vec2 m_pos;
	kiko::vec2 m_vel;
};

int main(int argc, char* argv[]) {
#pragma region Initialization
	kiko::MemoryTracker::Initialize();
	kiko::seedRandom((unsigned int)time(nullptr));
	kiko::setFilePath("assets");

	kiko::g_renderer.Initialize();
	
	kiko::g_inputSystem.Initialize();
	kiko::g_audioSystem.Initialize();


	unique_ptr<SpaceGame> game = make_unique<SpaceGame>();
	game->Initialize();
	
	

	vector<Star> stars;
	for (int i = 0; i < 1000; i++) {
		kiko::vec2 pos(kiko::vec2(kiko::random(kiko::g_renderer.GetWidth()), kiko::random(kiko::g_renderer.GetHeight())));
		kiko::vec2 vel(kiko::vec2(kiko::random(100, 200), kiko::random(2)));
		stars.push_back(Star(pos, vel));
	}


	kiko::Transform transform{ {400, 300}, 1.2f, 2 };

	kiko::vec2 position{ 400, 300 };
	float speed = 200;
	constexpr float turnRate = kiko::DegreesToRadians(180);

	kiko::Scene scene;
	
	
#pragma endregion

	bool quit = false;
	while (!quit) {
		kiko::g_time.Tick();
		kiko::g_inputSystem.Update();
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_ESCAPE)) {
			quit = true;
		}

		kiko::g_audioSystem.Update();
		
		float rotate = 0;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
		transform.rotation += rotate * turnRate * kiko::g_time.GetDeltaTime();

		float thrust = 0;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) {
			thrust = 1;
			//if (!kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_W)) {
			kiko::g_audioSystem.PlayOneShot("thrusters");
			//}
		}
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE)) kiko::g_audioSystem.PlayOneShot("laser");

		kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
		transform.position += forward * speed * thrust * kiko::g_time.GetDeltaTime();
		transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.GetWidth());
		transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_renderer.GetHeight());
		
		kiko::g_renderer.SetColor(0, 0, 0, 0);
		kiko::g_renderer.BeginFrame();

		kiko::vec2 vel(2, 1);
		for (auto& star : stars) {
			star.Update();
			star.Wrap(kiko::g_renderer.GetWidth(), kiko::g_renderer.GetHeight());
			kiko::g_renderer.SetColor(kiko::random(256), kiko::random(256), kiko::random(256), 255);
			kiko::g_renderer.DrawPoint(star.m_pos.x, star.m_pos.y);
		}

		kiko::g_renderer.SetColor(255, 255, 255, 255);
		game->Update(kiko::g_time.GetDeltaTime());
		kiko::g_particleSystem.Update(kiko::g_time.GetDeltaTime());
		game->Draw(kiko::g_renderer);
		kiko::g_particleSystem.Draw(kiko::g_renderer);
		//text->Draw(kiko::g_renderer, 400, 300);
		
		kiko::g_renderer.EndFrame();

		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	scene.RemoveAll();
	return 0;
}